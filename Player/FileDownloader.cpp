#include "FileDownloader.h"
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <QMutexLocker>

namespace mp {

const static QString Dot(".");

FileDownloader::FileDownloader(const QUrl& url, const QString& filePath, bool autoDelete)
	:m_aborted(false)
	,m_continueDownload(false)
	,m_url(url)
	,m_filePath(filePath)
	,m_manager(NULL)
	,m_reply(NULL)
	,m_file(NULL)
	,m_autoDelete(autoDelete)
{
}

bool FileDownloader::ContinueDownload() const
{
	return m_continueDownload;
}

void FileDownloader::SetContinueDownload(bool contiueDownload)
{
	m_continueDownload = contiueDownload;
}

void FileDownloader::SetNetworkAccessManager(QNetworkAccessManager* manager)
{
	m_manager = manager;
}

void FileDownloader::Do()
{
	if(!m_manager)
		m_manager = new QNetworkAccessManager(this);

	qDebug() << "Download file:" << m_filePath << " from url: " << m_url.toString() << " was started";

	QDir dir = QFileInfo(m_filePath).absoluteDir();
	if(!dir.exists())
		dir.mkpath(Dot);

	// Creates file
	m_file = new QFile(m_filePath);
	m_file->setPermissions(QFile::WriteOwner);

	bool opened = m_file->open(m_continueDownload ? QIODevice::Append : QIODevice::WriteOnly);

	if(!opened)
	{
		QString renameTo = m_filePath + "old";

		if(QFile::rename(m_filePath, renameTo))
		{
			opened = m_file->open(m_continueDownload ? QIODevice::Append : QIODevice::WriteOnly);
		}
	}

	if(opened)
	{
		QNetworkRequest request(m_url);

		if(m_continueDownload)
		{
			QString bytesData("bytes=" + QString::number(m_file->size()) + "-");
			request.setRawHeader("Range", bytesData.toLatin1());
		}

		m_reply = m_manager->get(request);
		connect(m_reply, SIGNAL(finished()), this, SLOT(DownloadFinished()));
		// Ignore SSL errors
		m_reply->ignoreSslErrors();
		connect(m_reply, SIGNAL(readyRead()), this, SLOT(DownloadReadyRead()));
		connect(m_reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(DownloadProgress(qint64,qint64)));

		m_aborted = false;
	}
	else
	{
		qDebug() << "Cant open file to write: " << m_filePath << " reason: " << m_file->errorString();
		m_errorString =  m_file->errorString();
		Cleanup();
		emit Finished(false, m_tag);
	}
}

void FileDownloader::Abort()
{
	if(!m_aborted)
	{
		m_aborted = true;

		if(m_reply) 
		{
			m_reply->abort();
			m_file->remove();
			Cleanup();
		}
	}
}

void FileDownloader::Cleanup()
{
	if(m_reply)
	{
		m_reply->deleteLater();
		m_reply = NULL;
	}

	if(m_file)
	{
		m_file->deleteLater();
		m_file = NULL;
	}
}

void FileDownloader::DownloadFinished()
{
	QNetworkReply::NetworkError err = m_reply->error();
	int fileSize = m_file->size();

	qDebug() << "File download:" << m_filePath << " size:" << fileSize <<" from url: " << m_url.toString() << " was finisehd";

	if(err == QNetworkReply::NoError && fileSize > 0)
	{
		bool success = true;

		QVariant contentLengthVar = m_reply->header(QNetworkRequest::ContentLengthHeader);

		if(!contentLengthVar.isNull())
		{
			qlonglong contentLength = m_reply->header(QNetworkRequest::ContentLengthHeader).toULongLong();

			if(contentLength != fileSize)
			{
				qDebug() << "Error occurred while download file:" << m_filePath
					<< " from url: " << m_url.toString()
					<< " error: Content-Lengh and actual file size isn't equal"
					<< " downloaded bytes: " << fileSize
					<< " Content-Lengh: " << contentLength;

				success = false;
			}
		}

		if(m_aborted)
		{
			m_file->remove();
		}

		m_file->flush();
		m_file->close();
		Cleanup();

		if(!success)
		{
			QString renameFrom = m_filePath + "old";
			QFile::rename(renameFrom, m_filePath);
		}

		emit Finished(success, m_tag);
	}
	else
	{
		qDebug() << "Error occurred while download file:" << m_filePath
			<< " from url: " << m_url.toString()
			<< " error: " << m_reply->errorString() 
			<< " downloaded bytes: " << fileSize;

		if(m_file)
		{
			m_file->remove();
		}
		
		if(m_reply)
		{
			m_errorString = m_reply->errorString();
		}

		Cleanup();

		QString renameFrom = m_filePath + "old";
		QFile::rename(renameFrom, m_filePath);

		emit Finished(false, m_tag);
	}

	if(m_autoDelete)
		deleteLater();
}

void FileDownloader::DownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
	if(!m_aborted)
	{
		emit ProgressChanged(bytesReceived, bytesTotal);
	}
}


void FileDownloader::DownloadReadyRead()
{
	if(!m_aborted && m_file && m_reply)
	{
		QByteArray bytes = m_reply->readAll();
		m_file->write(bytes);
		m_file->flush();
	}	
}

void FileDownloader::ErrorSlot( QNetworkReply::NetworkError code )
{
	QNetworkReply::NetworkError c = code;
}

}
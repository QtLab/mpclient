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
	,m_url(url)
	,m_filePath(filePath)
	,m_manager(NULL)
	,m_reply(0)
	,m_autoDelete(autoDelete)
{
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
    bool opened = m_file->open(QIODevice::WriteOnly);

	if(!opened)
	{
		QString renameTo = m_filePath + "old";

		if(QFile::rename(m_filePath, renameTo))
		{
			m_file->open(QIODevice::WriteOnly);
		}
	}

	QNetworkRequest request(m_url);
    m_reply = m_manager->get(request);

	connect(m_reply, SIGNAL(finished()), this, SLOT(DownloadFinished()));
	// Ignore SSL errors
	connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), m_reply, SLOT(ignoreSslErrors()));
	connect(m_reply, SIGNAL(readyRead()), this, SLOT(DownloadReadyRead()));
	connect(m_reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(DownloadProgress(qint64,qint64)));

	m_aborted = false;
}

void FileDownloader::Abort()
{
	if(!m_aborted)
	{
		m_aborted = true;

		if(m_reply) 
		{
			m_reply->abort();
			m_reply->deleteLater();
			m_file->remove();
			m_file->deleteLater();
			m_reply = NULL;
			m_file = NULL;
		}
	}
}

void FileDownloader::DownloadFinished()
{
	QNetworkReply::NetworkError err = m_reply->error();
	int fileSize = m_file->size();

	qDebug() << "File download:" << m_filePath << " size:" << fileSize <<" from url: " << m_url.toString() << " was finisehd";

	if(err == QNetworkReply::NoError && fileSize > 0)
	{
		if(m_aborted)
		{
			m_file->remove();
		}

		m_file->flush();
		m_file->close();
		m_reply->deleteLater();
		m_file->deleteLater();
		m_reply = NULL;
		m_file = NULL;

		emit Finished(m_filePath);
	}
	else
	{
		qDebug() << "Error occurred while download file:" << m_filePath
			<< " from url: " << m_url.toString()
			<< " error: " << m_reply->errorString() 
			<< " downloaded bytes: " << fileSize;

		emit Error(m_reply->errorString());
		emit Finished(QString::null);

		m_file->remove();
		m_reply->deleteLater();
		m_file->deleteLater();
		m_reply = NULL;
		m_file = NULL;
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
	}	
}

void FileDownloader::ErrorSlot( QNetworkReply::NetworkError code )
{
	QNetworkReply::NetworkError c = code;
}

}
#include "UpdateControler.h"
#include "DownlaodController.h"
#include "UrlModel.h"
#include "UpdateModel.h"

#include <QStringList>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <QApplication>
#include <QCryptographicHash>

namespace mp {

const int				DefualtUpdateInterval = 1000 * 60 * 60;//1 h
const QString			PlayerExeName("player.exe");

UpdateControler* UpdateControler::m_instance = 0;

UpdateControler& UpdateControler::Inst()
{
	if(!m_instance)
		m_instance = new UpdateControler();

	return *m_instance;
}

UpdateControler::UpdateControler()
	:m_inprocess(false)
	,m_downloaded(0)
{
}

void UpdateControler::Run(const QString& updateMd5)
{
	qDebug() << "UpdateControler::Run new md5: " << updateMd5 << " last md5: ";
	if(!m_inprocess)
	{
		UrlModel url = UrlModel::CreateUpdateUrl();
		qDebug() << "Download update json: " << url.Url().toString();
		DownlaodController::Inst().Get(url, this, SLOT(FileDownloaded(QNetworkReply*)));
	}
	else
	{
		qDebug() <<"Update already in process";
	}
}

void UpdateControler::ProcessUpdateReply(QNetworkReply* reply)
{
	QVariant filePathAttr = reply->request().attribute(QNetworkRequest::User);

	int replyZise = reply->size();
	if(reply->error() != QNetworkReply::NoError || replyZise == 0)
	{
			qDebug() << "Network error, url: "
				<< reply->request().url().toString() 
				<<  ", error: " << reply->errorString() 
				<< ", size" << replyZise;
	}
	else
	{
		bool restarRequired = false;

		m_filesToUpdate.ParseJson(reply->readAll());

		FileToUpdatePtr file;
		foreach (file, m_filesToUpdate.Items())
		{
			if(file->FileName().contains(PlayerExeName))
			{
				//TODO: update with loader
				emit UpdateFinished(true);
				break;
			}

			DownlaodController::Inst().DownloadFile(file->Url(), 
				UrlModel::CreateCurrentDirUrl(file->FileName()).ToUrl(), this, SLOT(ProcessUpdateFileReply(QNetworkReply*)));
			
		}
	}

	reply->deleteLater();
}

void UpdateControler::ProcessUpdateFileReply(QNetworkReply* reply)
{
	m_downloaded++;

	if(m_downloaded == m_filesToUpdate.rowCount())
	{
		emit UpdateFinished(false);
	}

	reply->deleteLater();
}

}
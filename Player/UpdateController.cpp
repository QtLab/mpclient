#include "UpdateController.h"
#include "DownlaodManager.h"
#include "RequestModel.h"
#include "UpdateModel.h"

#include <QDebug>
#include <QtNetwork/QNetworkReply>

namespace mp {

const int				DefualtUpdateInterval = 1000 * 60 * 60;//1 h
const QString			PlayerExeName("player.exe");

UpdateController::UpdateController()
{
}

bool UpdateController::InProcess() const
{
	bool inProcess = m_filesToUpdate.rowCount() > 0;
	return inProcess;
}

void UpdateController::Run(const QString& updateMd5)
{
	qDebug() << "UpdateControler::Run new md5: " << updateMd5 << " last md5: ";

	if(!InProcess())
	{
		RequestModel url = RequestModel::CreateUpdateUrl();

		qDebug() << "Download update json: " << url.Url().toString();

		m_networkAccess.Get(url, this, SLOT(ProcessUpdateReply()));
	}
	else
	{
		qDebug() << "Update already in process";
	}
}

void UpdateController::ProcessUpdateList()
{
	QNetworkReply * reply = qobject_cast<QNetworkReply*>(sender());

	if(reply->error() != QNetworkReply::NoError)
	{
			qDebug() << "Network error, url: "
				<< reply->request().url().toString() 
				<<  ", error: " << reply->errorString()  << ", size" << reply->size();
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
				emit UpdateFinished(true);
				break;
			}

			//m_networkAccess.DownloadFile(file->Url(), file->Url(), 
			//DownlaodManager::Inst().DownloadFile( 
				//RequestModel::CreateCurrentDirUrl(file->FileName()).ToUrl(), this, SLOT(ProcessUpdateFileReply(QNetworkReply*)));
			
		}
	}

	reply->deleteLater();
}

void UpdateController::FileDownloaded(/*QNetworkReply* reply*/)
{
}

}
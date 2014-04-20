#include "UpdateController.h"
#include "DownlaodManager.h"
#include "UpdateModel.h"
#include "MPRequest.h"

#include <QDebug>
#include <QtNetwork/QNetworkReply>

namespace mp {

const int DefualtUpdateInterval = 1000 * 60 * 60;//1 h

UpdateController::UpdateController()
	:m_filesToUpdate(0)
{
}

bool UpdateController::InProcess() const
{
	bool inProcess = m_filesToUpdate > 0;
	return inProcess;
}

void UpdateController::Run()
{
	if(!InProcess())
	{
		MPRequest request = MPRequest::CreateUpdateRequest();

		qDebug() << "Download update json: " << request.Url().toString();

		m_networkAccess.Get(request, this, SLOT(ProcessUpdateList()));
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

		// Files to update
		UpdateModel	updateModel;

		updateModel.ParseJson(reply->readAll());

#ifndef _DEBUG
		if(updateModel.RequiredPlayerUpdate())
		{
			//TODO: check to laoder exists
			emit UpdateFinished(true);
		}
		else
#endif
		{
			FileToUpdateList filesToUpdate = updateModel.Items();
			foreach (FileToUpdatePtr file, filesToUpdate)
			{
				QString filePath = file->FullPath();
				qDebug() << "Downlaod file: " << filePath << " from: " << file->Url();

				m_networkAccess.DownloadFile(file->Url(), file->FullPath(), this, SLOT(FileDownloaded(const QString&)));			
			}

			m_filesToUpdate = filesToUpdate.count();
		}
	}

	reply->deleteLater();
}

void UpdateController::FileDownloaded(const QString& path)
{
	if(path.isEmpty())
		qDebug() << "File downlaoded: " << path;

	m_filesToUpdate--;

	if(m_filesToUpdate <= 0)
	{
		emit UpdateFinished(false);
	}
}

}
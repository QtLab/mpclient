#include "UpdateController.h"
#include "DownlaodManager.h"
#include "UpdateModel.h"
#include "MPRequest.h"

#include <QDebug>
#include <QtNetwork/QNetworkReply>

namespace mp {

const int DefualtUpdateInterval = 1000 * 60 * 60 * 5; //5 h
const int ParallelFilesUpdating = 4;

UpdateController::UpdateController()
	:m_filesInProcess(0)
{
	n_updateTimer.setInterval(DefualtUpdateInterval);
	connect(&n_updateTimer, SIGNAL(timeout()), SLOT(CheckForUpdate()));
	n_updateTimer.start();
}

bool UpdateController::InProcess() const
{
	bool inProcess = m_updateModel.rowCount() > 0 || m_filesInProcess > 0;
	return inProcess;
}

void UpdateController::CheckForUpdate()
{
#ifndef _DEBUG
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
#endif
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
		Cleanup();

		qDebug() << "Update started, RequirePlayerUpdate is:: " << m_updateModel.RequirePlayerUpdate();
		
		m_updateModel.ParseJson(reply->readAll());

		qDebug() << "Update mode parsing, RequirePlayerUpdate is: " << m_updateModel.RequirePlayerUpdate();

		while(m_updateModel.rowCount() > 0 && m_filesInProcess < ParallelFilesUpdating)
		{
			ProcessNextFile();
			m_filesInProcess++;
		}

		if(m_filesInProcess == 0)
		{
			emit UpdateFinished(false);
			Cleanup();
		}
	}

	reply->deleteLater();
}

void UpdateController::FileDownloaded(const QString& path)
{
	m_filesInProcess--;

	if(!ProcessNextFile())
	{
		qDebug() << "Update finished, RequirePlayerUpdate is: " << m_updateModel.RequirePlayerUpdate();

		emit UpdateFinished(m_updateModel.RequirePlayerUpdate());
		Cleanup();
	}
}

bool UpdateController::ProcessNextFile()
{
	FileToUpdatePtr fileToUpdate = m_updateModel.PopBack();

	if(!fileToUpdate.isNull())
	{
		QString filePath = fileToUpdate->FullPath();
		m_networkAccess.DownloadFile(fileToUpdate->Url(), filePath, this, SLOT(FileDownloaded(const QString&)));
		m_filesInProcess++;
		return true;
	}

	return false;
}

void UpdateController::Cleanup()
{
	m_filesInProcess = 0;
	m_updateModel.Cleanup();
}

}
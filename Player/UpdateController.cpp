#include "UpdateController.h"
#include "DownlaodManager.h"
#include "UpdateModel.h"
#include "UrlBuilder.h"
#include "Config.h"
#include <QTimer>

#include <QDebug>
#include <QtNetwork/QNetworkReply>

namespace mp {
namespace controller {

const int DefualtUpdateInterval		= 1000 * 60 * 60 * 5; //5 h
const int ParallelFilesUpdating		= 4;
const int MaxTriesDownload			= 2;

UpdateController::UpdateController()
	:m_filesInProcess(0)
	,m_activatedByUser(false)
	,m_aborted(false)
	,m_downloadMgr(new DownlaodManager())
{
	m_updateTimer = new QTimer();
	m_updateTimer->setInterval(DefualtUpdateInterval);
	connect(m_updateTimer, SIGNAL(timeout()), SLOT(CheckForUpdate()));
	m_updateTimer->start();
}

bool UpdateController::InProcess() const
{
	bool inProcess = m_updateModel.Count() > 0 || m_filesInProcess > 0;
	return inProcess;
}

void UpdateController::CheckForUpdate(bool activatedByUser)
{
#ifndef _DEBUG
	if(!InProcess())
	{
		m_activatedByUser = activatedByUser;
		m_aborted =  false;

		QUrl url = UrlBuilder::CreateUpdate(Config::Inst().UserId());

		m_downloadMgr->Get(url, this, SLOT(ProcessUpdateList()));
	}
	else
	{

		qDebug() << "Update already in process";
	}
#else
	m_activatedByUser = activatedByUser;
	emit UpdateFinished(UpdateResult(true, false, m_activatedByUser));
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

		bool activatedByUser = m_activatedByUser;
		Cleanup();
		emit UpdateFinished(UpdateResult(false, false, activatedByUser));
	}
	else
	{
		Cleanup();
		

		m_updateModel.ParseJson(reply->readAll());

		qDebug() << "Update model processed, require player restart is: " << m_updateModel.RequirePlayerUpdate();

		while(m_updateModel.Count() > 0 && m_filesInProcess < ParallelFilesUpdating)
		{
			ProcessNextFile();
		}

		if(m_filesInProcess == 0)
		{
			bool activatedByUser = m_activatedByUser;
			Cleanup();
			emit UpdateFinished(UpdateResult(true, false, activatedByUser));
		}
	}

	reply->deleteLater();
}

void UpdateController::FileDownloaded(bool success, QVariant tag)
{
	if(!m_aborted)
	{
		model::FileToUpdate file = tag.value<model::FileToUpdate>();

		if(file.Exists())
		{
			if(!ProcessNextFile() && m_filesInProcess <= 1)
			{
				qDebug() << "Update finished, player restart required is: " << m_updateModel.RequirePlayerUpdate();

				bool rstartRequired = m_updateModel.RequirePlayerUpdate();
				bool activatedByUser = m_activatedByUser;

				Cleanup();
				emit UpdateFinished(UpdateResult(true, rstartRequired, activatedByUser));
			}
			else
			{
				m_filesInProcess--;

				qDebug() << "Update - files in process : " << m_filesInProcess  << ", also files to update"  << m_updateModel.Count();
			}
		}
		else
		{
			if(file.DownloadTries() >= MaxTriesDownload)
			{
				m_downloadMgr->AbortAll();
				m_aborted = true;

				bool activatedByUser = m_activatedByUser;
				Cleanup();
				emit UpdateFinished(UpdateResult(false, false, activatedByUser));
			}
			else
			{
				file.IncrementDownlaodTries();

				m_downloadMgr->DownloadFile(file.Url(), file.FullPath(), 
											false, qVariantFromValue(file), 
											this, SLOT(FileDownloaded(bool, QVariant)));
			}
		}
	}
}

bool UpdateController::ProcessNextFile()
{
	model::FileToUpdate fileToUpdate;
	
	if(m_updateModel.PopBack(fileToUpdate))
	{
		fileToUpdate.IncrementDownlaodTries();

		m_downloadMgr->DownloadFile(fileToUpdate.Url(), fileToUpdate.FullPath(), 
			false, qVariantFromValue(fileToUpdate), 
									this, SLOT(FileDownloaded(bool, QVariant)));
		m_filesInProcess++;
		return true;
	}

	return false;
}

void UpdateController::Cleanup()
{
	m_activatedByUser = false;
	m_filesInProcess = 0;
	m_updateModel.Cleanup();
}

}
}
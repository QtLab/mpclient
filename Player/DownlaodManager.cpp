#include "DownlaodManager.h"
#include "FileDownloader.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QNetworkSession>

#include <QDebug>

namespace mp {

DownlaodManager * DownlaodManager::m_globalInstance = 0;

DownlaodManager::DownlaodManager()
	:m_manager(new QNetworkAccessManager(this))
	,m_session(new QNetworkSession(m_manager->configuration(), this))
{
}

DownlaodManager& DownlaodManager::Global()
{
	if(!m_globalInstance)
	{
		m_globalInstance = new DownlaodManager();
	}

	return *m_globalInstance;
}

void DownlaodManager::DownloadFile(const QUrl& url, const QString& filePath,
					bool continueDonload, const QVariant& tag,
					QObject* finishListner, const char* finishSlot, 
					QObject* progressListner, const char* progressSlot)
{
	FileDownloader * downlaoder = new FileDownloader(url, filePath);

	downlaoder->SetNetworkAccessManager(m_manager);
	downlaoder->SetContinueDownload(continueDonload);
	downlaoder->SetTag(tag);

	if(finishListner && finishSlot)
	{
		connect(downlaoder, SIGNAL(Finished(bool, QVariant)), finishListner, finishSlot);
	}
	
	if(progressListner && progressSlot)
	{
		connect(downlaoder, SIGNAL(ProgressChanged(qint64, qint64)), progressListner, progressSlot);
	}

	downlaoder->Do();
}

void DownlaodManager::Get(const QUrl& url ,QObject* listenerFinish, const char* slot)
{
	qDebug() << "DownlaodManager::Get from: " << url.toString();

	QNetworkRequest request(url);
	QNetworkReply* reply = m_manager->get(request);
	
	if(listenerFinish == NULL)
	{
		connect(reply, SIGNAL(finished()), this , SLOT(ReplyFinished()));
	}
	else
	{
		connect(reply, SIGNAL(finished()), listenerFinish, slot);
	}

	reply->ignoreSslErrors();
}
	
void DownlaodManager::Post(const QUrl& url, const QByteArray& body, QObject* listenerFinish, const char* slot)
{
	qDebug() << "DownlaodManager::Post from: " << url.toString();

	QNetworkRequest request(url);
	QNetworkReply* reply = m_manager->post(request, body);

	if(listenerFinish == NULL)
	{
		connect(reply, SIGNAL(finished()), this , SLOT(ReplyFinished()));
	}
	else
	{
		connect(reply, SIGNAL(finished()), listenerFinish, slot);
	}

	reply->ignoreSslErrors();
}

void DownlaodManager::AbortAll()
{
	m_session->stop();
}

void DownlaodManager::HttpReplyFinished()
{
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

	if(reply)
		reply->deleteLater();
}

}
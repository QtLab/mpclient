#include "DownlaodManager.h"
#include "FileDownloader.h"
#include "RequestModel.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include <QDebug>

namespace mp {

DownlaodManager::DownlaodManager()
	:m_manager(new QNetworkAccessManager())
{
}

void DownlaodManager::DownloadFile(const QString& url, const QString& filePath, 
												QObject* listFinish, const char* slot)
{
	FileDownloader * downlaoder = new FileDownloader(QUrl(url), filePath);
	downlaoder->SetNetworkAccessManager(m_manager);
	connect(downlaoder, SIGNAL(Finished(const QString&)), listFinish, slot);
	downlaoder->Do();
}

void DownlaodManager::Get(const RequestModel& requestUrl ,QObject* listenerFinish, const char* slot)
{
	QNetworkRequest request(requestUrl.Url());
	QNetworkReply* reply = m_manager->get(request);
	
	if(listenerFinish == NULL)
	{
		connect(reply, SIGNAL(finished()), this , SLOT(ReplyFinished()));
	}
	else
	{
		connect(reply, SIGNAL(finished()), listenerFinish, slot);
	}

	// Ignore the errors associated with the SSL certificate
	connect(reply, SIGNAL(sslErrors(QList<QSslError>)), reply, SLOT(ignoreSslErrors()));
}
	
void DownlaodManager::Post(const RequestModel& requestUrl ,QObject* listenerFinish, const char* slot)
{
	QNetworkRequest request(requestUrl.Url());

	QByteArray * data = requestUrl.ReqBodyRef();
	QNetworkReply* reply = m_manager->post(request, *data);

	if(listenerFinish == NULL)
	{
		connect(reply, SIGNAL(finished()), this , SLOT(ReplyFinished()));
	}
	else
	{
		connect(reply, SIGNAL(finished()), listenerFinish, slot);
	}

	// Ignore the errors associated with the SSL certificate
	connect(reply, SIGNAL(sslErrors(QList<QSslError>)), reply, SLOT(ignoreSslErrors()));
}

void DownlaodManager::HttpReplyFinished()
{
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

	if(reply)
		reply->deleteLater();
}

}
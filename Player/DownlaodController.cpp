#include "DownlaodController.h"
#include "FileDownloader.h"

#include <QDebug>

namespace mp {


DownlaodController* DownlaodController::m_instance = 0;

DownlaodController& DownlaodController::Inst()
{
	if(!m_instance)
		m_instance = new DownlaodController();

	return *m_instance;
}

DownlaodController::DownlaodController()
{

}

void DownlaodController::DownloadFile(const QString& url, const QString& filePath, 
												QObject* listFinish, const char* slot)
{
	FileDownloader * downlaoder = new FileDownloader(QUrl(url), filePath);
	downlaoder->SetNetworkAccessManager(&m_manager);
	connect(downlaoder, SIGNAL(Finished(const QString&)), listFinish, slot);
	downlaoder->Do();
}

void DownlaodController::Get(const UrlModel& requestUrl ,QObject* listFinish, const char* slot)
{
	QNetworkRequest request(requestUrl.Url());
	QNetworkReply* reply = m_manager.get(request);
	
	if(listFinish == NULL)
	{
		connect(reply, SIGNAL(finished()), this , SLOT(FileDownloaded()));
	}
	else
	{
		connect(reply, SIGNAL(finished()), listFinish, slot);
	}

	// Ignore the errors associated with the SSL certificate
	connect(reply, SIGNAL(sslErrors(QList<QSslError>)), reply, SLOT(ignoreSslErrors()));
}
	
void DownlaodController::Post(const UrlModel& requestUrl ,QObject* listFinish, const char* slot)
{
	QNetworkRequest request(requestUrl.Url());

	QByteArray * data = requestUrl.ReqBodyRef();
	QNetworkReply* reply = m_manager.post(request, *data);

	if(listFinish == NULL)
	{
		connect(reply, SIGNAL(finished()), this , SLOT(FileDownloaded()));
	}
	else
	{
		connect(reply, SIGNAL(finished()), listFinish, slot);
	}

	// Ignore the errors associated with the SSL certificate
	connect(reply, SIGNAL(sslErrors(QList<QSslError>)), reply, SLOT(ignoreSslErrors()));
}

void DownlaodController::FileDownloaded(QNetworkReply* reply)
{
	reply->deleteLater();
}

}
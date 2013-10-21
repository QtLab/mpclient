#ifndef MP_DOWNLOAD_CONTROLLER_H
#define MP_DOWNLOAD_CONTROLLER_H

#include "UrlModel.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

namespace mp {

class FileDownloader;

class DownlaodController : public QObject
{
	Q_OBJECT

public:
	static DownlaodController& Inst();

	void DownloadFile(const QString& url, const QString& filePath, 
						QObject* listFinish = NULL, const char* slot = NULL);

	void Get(const UrlModel& requestUrl
		,QObject* listFinish = NULL, const char* slot = NULL);
	
	void Post(const UrlModel& requestUrl
		,QObject* listFinish = NULL, const char* slot = NULL);

private slots:
	void ReplyFinished(QNetworkReply* reply);

private:
	DownlaodController();

	QNetworkAccessManager			m_manager;
	static DownlaodController *		m_instance;
};

}

#endif

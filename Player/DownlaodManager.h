#ifndef MP_DOWNLOAD_MANAGER_H
#define MP_DOWNLOAD_MANAGER_H

#include "Prerequirements.h"

namespace mp {

class FileDownloader;

class DownlaodManager : public QObject
{
	Q_OBJECT

public:
	DownlaodManager();

	void DownloadFile(const QString& url, const QString& filePath, 
						QObject* listFinish = NULL, const char* slot = NULL);

	void Get(const MPRequest& request
		,QObject* listenerFinish = NULL, const char* slot = NULL);
	
	void Post(const MPRequest& request
		,QObject* listenerFinish = NULL, const char* slot = NULL);

private slots:
	void HttpReplyFinished();

private:
	QNetworkAccessManager *			m_manager;
};

}

#endif

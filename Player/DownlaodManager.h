#ifndef MP_DOWNLOAD_MANAGER_H
#define MP_DOWNLOAD_MANAGER_H

#include "Prerequirements.h"
#include <QVariant>

namespace mp {

class FileDownloader;

class DownlaodManager : public QObject
{
	Q_OBJECT

public:
	DownlaodManager();

	static DownlaodManager& Global();

	void DownloadFile(const QUrl& url, const QString& filePath,
						bool continueDonload = false, const QVariant& tag = QVariant(),
						QObject* finishListner = NULL, const char* finishSlot = NULL, 
						QObject* progressListner = NULL, const char* progressSlot = NULL);

	void Get(const QUrl& url ,QObject* listenerFinish = NULL, const char* slot = NULL);
	
	void Post(const QUrl& url, const QByteArray& body, QObject* listenerFinish = NULL, const char* slot = NULL);

private slots:
	void HttpReplyFinished();

private:
	QNetworkAccessManager *			m_manager;
	static DownlaodManager *		m_globalInstance;
};

}

#endif

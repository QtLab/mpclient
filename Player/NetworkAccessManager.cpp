#include "NetworkAccessManager.h"
#include "Path.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkDiskCache>
#include <QSslError>

namespace mp {

NetworkAccessManager::NetworkAccessManager(QObject* parent)
	:QNetworkAccessManager(parent)
{
	QNetworkDiskCache *diskCache = new QNetworkDiskCache(this);
	diskCache->setCacheDirectory(Path::HttpCache());
	setCache(diskCache);

	connect(this, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError> & )), this, SLOT(HandleSslErrors(QNetworkReply*, const QList<QSslError> & )));
}

NetworkAccessManager::~NetworkAccessManager()
{
}

QNetworkReply * NetworkAccessManager::createRequest(Operation op, const QNetworkRequest & req, QIODevice * outgoingData)
{
#ifdef _DEBUG
	//qDebug() << req.url().toString();
#endif

	QNetworkRequest newReq = req;
	newReq.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
	QNetworkReply * reply = QNetworkAccessManager::createRequest(op, newReq, outgoingData);
	reply->ignoreSslErrors();
	return reply;
}

void NetworkAccessManager::HandleSslErrors(QNetworkReply * reply, const QList<QSslError> & errors)
{
	reply->ignoreSslErrors();
}

}
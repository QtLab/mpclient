#ifndef MP_NETWORK_ACCESS_MANAGER_H
#define MP_NETWORK_ACCESS_MANAGER_H

#include <QNetworkAccessManager>

namespace mp {

class NetworkAccessManager : public QNetworkAccessManager
{
	Q_OBJECT

public:
	NetworkAccessManager(QObject* parent = NULL);
	virtual ~NetworkAccessManager();

protected:
	virtual QNetworkReply * createRequest ( Operation op, const QNetworkRequest & req, QIODevice * outgoingData = 0 );

public slots:
	virtual void HandleSslErrors( QNetworkReply * reply, const QList<QSslError> & errors );
};

}

#endif


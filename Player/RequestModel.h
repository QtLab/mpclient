#ifndef MP_URL_MODEL_H
#define MP_URL_MODEL_H

#include <QObject>
#include <QMetaType>
#include <QUrl>
#include <QString>
#include <QByteArray>

namespace mp {

// Url for request, he may have a link to the body of the request
class RequestModel : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString domain READ Domain WRITE SetDomain)
	Q_PROPERTY(QString tr READ TrafferId WRITE SetTrafferId)
	Q_PROPERTY(QString url READ ToUrl WRITE SetUrl)

public:
	RequestModel(const QString& url);
	RequestModel(const QUrl& url);
	RequestModel();
	~RequestModel();

	QString Domain() const;
	void SetDomain(const QString& domain);

	QString TrafferId() const;
	void SetTrafferId(const QString& id);

	//TODO:
	//QString TrafferId() const;
	//void SetTrafferId(const QString& id);

	void SetReqBodyRef(QByteArray * reqBody);
	QByteArray* ReqBodyRef() const;

	QString ToUrl() const;
	void SetUrl(const QString& url);

	QUrl Url() const;

	static RequestModel CreateUpdateUrl();
	static RequestModel CreateCurrentDirUrl(const QString& relativePath);

private:
	QUrl					m_url;
	QByteArray *			m_reqBodyRef;
};

}

#endif
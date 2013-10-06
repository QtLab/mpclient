#ifndef MP_URL_MODEL_H
#define MP_URL_MODEL_H

#include <QMetaType>
#include <QUrl>
#include <QString>
#include <QByteArray>

namespace mp {

// Url for request, he may have a link to the body of the request
class UrlModel : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString domain READ Domain WRITE SetDomain)
	Q_PROPERTY(QString tr READ TrafferId WRITE SetTrafferId)
	Q_PROPERTY(QString url READ ToUrl WRITE SetUrl)

public:
	UrlModel(const QString& url);
	UrlModel(const QUrl& url);
	UrlModel();
	~UrlModel();

	QString Domain() const;
	void SetDomain(const QString& domain);

	QString TrafferId() const;
	void SetTrafferId(const QString& id);
	
	void SetReqBodyRef(QByteArray * reqBody);
	QByteArray* ReqBodyRef() const;

	QString ToUrl() const;
	void SetUrl(const QString& url);

	QUrl Url() const;

	static UrlModel CreateUpdateUrl();
	static UrlModel CreateCurrentDirUrl(const QString& relativePath);

private:
	QUrl					m_url;
	QByteArray *			m_reqBodyRef;
};

}

#endif
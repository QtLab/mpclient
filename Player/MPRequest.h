#ifndef MP_REQUEST_H
#define MP_REQUEST_H

#include <QObject>
#include <QMetaType>
#include <QUrl>
#include <QString>
#include <QByteArray>

namespace mp {

// Url for request, he may have a link to the body of the request
class MPRequest
{

public:
	MPRequest(const QString& url);
	MPRequest(const QUrl& url);
	MPRequest();
	~MPRequest();

	QString Domain() const;
	void SetDomain(const QString& domain);

	void SetQuery(const QString& query);
	QString Query() const;

	void SetQueryParameter(const QString& key, const QString& value);
	QString QueryParameter(const QString& key) const;

	QString UserId() const;
	void SetUserId(const QString& id);

	// Traffic source
	QString Source() const;
	void SetSource(const QString& id);

	void SetBody(const QByteArray& reqBody);
	const QByteArray& Body() const;

	QUrl Url() const;

	static MPRequest CreateUpdateRequest();
	static MPRequest CreateCurrentDirUrl(const QString& relativePath);

private:
	void SetQueryParameterImpl(const QString& key, const QString& value);
	QString QueryParameterImpl(const QString& key) const; 

private:
	QUrl					m_url;
	QByteArray				m_requestBody;
};

}

#endif
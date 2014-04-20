#include "MPRequest.h"
#include "Common.h"
#include "Config.h"
#include <QDir>
#include <QUrlQuery>
#include <QDebug>

namespace mp {

static const QString TRAFFIC_SOURCE_PK("Source");
static const QString USERID_PK("UserId");
static const QString DEFAULT_HOST_N(DEFAULT_HOST);
static const QString HTTP_SCHEME("http");

MPRequest::MPRequest(const QString& url)
	:m_url(url)
{
}

MPRequest::MPRequest(const QUrl& url)
	:m_url(url)
{
}

MPRequest::MPRequest()
{
	m_url.setScheme(HTTP_SCHEME);
	SetDomain(DEFAULT_HOST_N);
}

MPRequest::~MPRequest()
{
}

QString MPRequest::Domain() const
{
	return m_url.host();
}

void MPRequest::SetDomain(const QString& domain)
{
	m_url.setHost(domain);
}

QString MPRequest::UserId() const
{
	return QueryParameterImpl(USERID_PK);
}

void MPRequest::SetUserId(const QString& id)
{
	SetQueryParameterImpl(USERID_PK, id);
	qDebug() << m_url.toString();
}

void MPRequest::SetQuery(const QString& query)
{
	//m_url.setQuery(query);
	m_url.setPath(query);
	qDebug() << m_url.toString();
}

QString MPRequest::Query() const
{
	return m_url.query();
}

void MPRequest::SetQueryParameter(const QString& key, const QString& value)
{
	SetQueryParameterImpl(key, value);
}

QString MPRequest::QueryParameter(const QString& key) const
{
	return QueryParameterImpl(key);
}

QString MPRequest::Source() const
{
	return QueryParameterImpl(TRAFFIC_SOURCE_PK);
}

void MPRequest::SetSource(const QString& source)
{
	SetQueryParameterImpl(TRAFFIC_SOURCE_PK, source);
}

void MPRequest::SetBody(const QByteArray& reqBody) 
{  
	m_requestBody = reqBody; 
}

const QByteArray& MPRequest::Body() const 
{ 
	return  m_requestBody; 
}

QUrl MPRequest::Url() const
{
	return m_url;
}

void MPRequest::SetQueryParameterImpl(const QString& key, const QString& value)
{
	QUrlQuery urlQuery(m_url);
	urlQuery.removeQueryItem(key);
	urlQuery.addQueryItem(key, value);
	m_url.setQuery(urlQuery);
}

QString MPRequest::QueryParameterImpl(const QString& key) const
{
	QUrlQuery urlQuery(m_url);
	QString value = urlQuery.queryItemValue(key);
	return value;
}

MPRequest MPRequest::CreateUpdateRequest()
{
	MPRequest req;
	req.SetQuery("/Handlers/GetDataFile.ashx");
	req.SetUserId(Config::Inst().UserId());
	
	return req;
}

MPRequest MPRequest::CreateCurrentDirUrl(const QString& relativePath)
{
	QString current = QDir::current().filePath(relativePath);
	return MPRequest(current);
}

}
#include "UrlModel.h"

#include <QDir>

namespace mp {

UrlModel::UrlModel(const QString& url)
	:m_url(url)
{
}

UrlModel::UrlModel(const QUrl& url)
	:m_url(url)
{
}

UrlModel::UrlModel()
{
}

UrlModel::~UrlModel()
{
}

QString UrlModel::Domain() const
{
	return m_url.queryItemValue("domain");
}

void UrlModel::SetDomain(const QString& domain)
{
	m_url.removeQueryItem("domain");
	m_url.addQueryItem("domain", domain);
}

QString UrlModel::TrafferId() const
{
	return m_url.queryItemValue("tr");
}

void UrlModel::SetTrafferId(const QString& id)
{
	m_url.removeQueryItem("tr");
	m_url.addQueryItem("tr", id);
}

void UrlModel::SetReqBodyRef(QByteArray * reqBody) 
{  
	m_reqBodyRef = reqBody; 
}

QByteArray* UrlModel::ReqBodyRef() const 
{ 
	return  m_reqBodyRef; 
}

QUrl UrlModel::Url() const
{
	return m_url;
}

QString UrlModel::ToUrl() const
{
	return m_url.toString();
}

void UrlModel::SetUrl(const QString& url) 
{
	m_url = QUrl(url);
}

UrlModel UrlModel::CreateUpdateUrl()
{
	return UrlModel("somedomen.com");
}

UrlModel UrlModel::CreateCurrentDirUrl(const QString& relativePath)
{
	QString current = QDir::current().filePath(relativePath);
	return UrlModel(current);
}

}
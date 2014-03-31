#include "RequestModel.h"

#include <QDir>

namespace mp {

RequestModel::RequestModel(const QString& url)
	:m_url(url)
{
}

RequestModel::RequestModel(const QUrl& url)
	:m_url(url)
{
}

RequestModel::RequestModel()
{
}

RequestModel::~RequestModel()
{
}

QString RequestModel::Domain() const
{
	//return m_url.queryItemValue("domain");

	return "";
}

void RequestModel::SetDomain(const QString& domain)
{
	//m_url.removeQueryItem("domain");
	//m_url.addQueryItem("domain", domain);
}

QString RequestModel::TrafferId() const
{
	//return m_url.queryItemValue("tr");
	return "";
}

void RequestModel::SetTrafferId(const QString& id)
{
	//m_url.removeQueryItem("tr");
	//m_url.addQueryItem("tr", id);
}

void RequestModel::SetReqBodyRef(QByteArray * reqBody) 
{  
	//m_reqBodyRef = reqBody; 
}

QByteArray* RequestModel::ReqBodyRef() const 
{ 
	return  m_reqBodyRef; 
}

QUrl RequestModel::Url() const
{
	return m_url;
}

QString RequestModel::ToUrl() const
{
	return m_url.toString();
}

void RequestModel::SetUrl(const QString& url) 
{
	m_url = QUrl(url);
}

RequestModel RequestModel::CreateUpdateUrl()
{
	return RequestModel("somedomen.com");
}

RequestModel RequestModel::CreateCurrentDirUrl(const QString& relativePath)
{
	QString current = QDir::current().filePath(relativePath);
	return RequestModel(current);
}

}
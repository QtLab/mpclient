#include "BannersModel.h"

#include <QHash>
#include <QDebug>
#include <QJsonDocument>
#include <QMetaProperty>
#include <QUrl>
#include <QFileInfo>

namespace mp {
namespace model {

BannerInfo::BannerInfo()
	:m_tvSourceId(-1)
{				
}

BannerInfo::~BannerInfo()
{
}

int BannerInfo::Id() const
{
	return m_id;
}

void BannerInfo::SetId(int id)
{
	m_id = id;
}

QString BannerInfo::Logo() const
{
	return m_logo;
}

void BannerInfo::SetLogo(const QString& logo)
{
	if(!logo.startsWith("http", Qt::CaseInsensitive))
	{
		m_logo = QUrl::fromLocalFile(QFileInfo(logo).absoluteFilePath()).toString();
	}
	else
	{
		m_logo = logo;
	}
}

QString BannerInfo::ExternalUrl() const
{ 
	return m_externalUrl; 
}

void BannerInfo::SetExternalUrl(const QString& url)
{ 
	m_externalUrl = url; 
}

int BannerInfo::TVSourceId() const
{
	return m_tvSourceId;
}

void BannerInfo::SetTVSourceId(int id)
{
	m_tvSourceId = id;
}

BannersModel::BannersModel(QObject* parent)
	:BaseListModel<BannerInfo>(parent)
{
}

BannersModel::~BannersModel()
{
}

BannerInfoPtr BannersModel::FindById(int id) const
{
	foreach(BannerInfoPtr banner, m_items)
	{
		if(banner->Id() == id)
		{
			return banner;
		}
	}

	return BannerInfoPtr();
}

QVariant BannersModel::data(const QModelIndex & index, int role) const 
{
	if (index.row() < 0 || index.row() > m_items.count())
		return QVariant();

	const BannerInfoPtr contact = m_items.at(index.row());
	
	QVariant result;

	switch (role) 
	{
		case Id:
			result = QVariant(contact->Id());
			break;
		case ExtrnalUrl:
			result = QVariant(contact->ExternalUrl());
			break;
		case TSourceId:
			result = QVariant(contact->TVSourceId());
			break;
	}

	return result;
}

int BannersModel::rowCount(const QModelIndex &parent) const
{
	return m_items.count();
}

QHash<int, QByteArray>	BannersModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[Id] = "Id";
	roles[ExtrnalUrl] = "ExtrnalUrl";
	roles[TSourceId] = "TSourceId";

	return roles;
}

} //namespace model
} //namespace mp

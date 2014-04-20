#include "ChannelSourceModel.h"

#include <QFile>
#include <QDir>
#include <QHash>
#include <QDebug>

namespace mp {

ChannelSource::ChannelSource()
{
}

ChannelSource::~ChannelSource()
{
}

QString ChannelSource::Name() const
{ 
	return m_name; 
}

void ChannelSource::SetName(const QString& name)
{ 
	m_name = name; 
}

QString ChannelSource::Id() const
{
	return m_id;
}

void ChannelSource::SetId(const QString& id)
{
	m_id = id;
}

QString ChannelSource::Logo() const
{
	return m_logo;
}

void ChannelSource::SetLogo(const QString& logo)
{
	m_logo = logo;
}

QString ChannelSource::Url() const
{
	return m_url;
}

void ChannelSource::SetUrl(const QString& url)
{
	m_url = url;
}

QString ChannelSource::GenreId() const
{
	return m_genreId;
}

void ChannelSource::SetGenreId(const QString& genreId)
{
	m_genreId = genreId;
}

GenreItemPtr ChannelSource::Genre() const
{
	return m_genre;
}

void ChannelSource::SetGenre(const GenreItemPtr& genre)
{
	m_genre = genre;
}

ChannelSourceModel::ChannelSourceModel()
{
}

ChannelSourceModel::~ChannelSourceModel() 
{
}

ChannelSourceList ChannelSourceModel::Items() const
{
	return m_items;
}

ChannelSourcePtr ChannelSourceModel::FindById(const QString& id)
{
	//QReadLocker locker(&m_lock);

	foreach(ChannelSourcePtr channel, m_items)
	{
		if(channel->Id() == id)
			return channel;
	}

	return ChannelSourcePtr();
}

QVariant ChannelSourceModel::data(const QModelIndex & index, int role) const 
{
	//QReadLocker locker(&m_lock);

	if (index.row() < 0 || index.row() > m_items.count())
		return QVariant();

	const ChannelSourcePtr channel = m_items.at(index.row());
	
	QVariant result;

	switch (role) {
	case Name:
		result = QVariant(channel->Name());
		break;
	case Id:
		result = QVariant(channel->Id());
		break;
	case Logo:
		result = QVariant(channel->Logo());
		break;
	case Url:
		result = QVariant(channel->Url());
		break;
	}

	return result;
}

int ChannelSourceModel::rowCount(const QModelIndex &parent) const
{
	//if(m_lock.tryLockForRead())
	{
		int count = m_items.count();
		//m_lock.unlock();
		return count;
	}
	//else
	{
		return m_items.count();
	}
}

QHash<int, QByteArray>	ChannelSourceModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[Name] = "Name";
	roles[Id] = "Id";
	roles[Logo] = "Logo";
	roles[Url] = "Url";

	return roles;
}

}
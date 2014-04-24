#include "ChannelSourceModel.h"

#include <QFile>
#include <QDir>
#include <QHash>
#include <QDebug>

namespace mp {

ChannelSource::ChannelSource()
	:m_playCount(0)
	,m_lastPlayTimestamp(0)
{
}

ChannelSource::~ChannelSource()
{
}

const QString& ChannelSource::Name() const
{ 
	return m_name; 
}

void ChannelSource::SetName(const QString& name)
{ 
	m_name = name; 
}

int ChannelSource::Id() const
{
	return m_id;
}

void ChannelSource::SetId(int id)
{
	m_id = id;
}

const QString& ChannelSource::Url() const
{
	return m_url;
}

void ChannelSource::SetUrl(const QString& url)
{
	m_url = url;
}

int ChannelSource::GenreId() const
{
	return m_genreId;
}

void ChannelSource::SetGenreId(int genreId)
{
	m_genreId = genreId;
}

uint ChannelSource::PlayCount() const
{
	return m_playCount;
}

void ChannelSource::SetPlayCount(uint count)
{
	m_playCount = count;
}

uint ChannelSource::LastPlayTimestamp() const
{
	return m_lastPlayTimestamp;
}

void ChannelSource::SetLastPlayTimestamp(uint ts)
{
	m_lastPlayTimestamp = ts;
}

void ChannelSource::SetLastPlayNow()
{
	QDateTime dt = QDateTime::currentDateTime();
	m_lastPlayTimestamp = dt.toTime_t();
}

ChannelSourceModel::ChannelSourceModel()
{
}

ChannelSourceModel::~ChannelSourceModel() 
{
}

void ChannelSourceModel::LoadWithStats(const QString& filePath)
{
	BaseListModel::Load(filePath, PropertiesSet() << "id" << "name" << "url" << "genreid");

	ChannelSourceModel channelsStats;
	channelsStats.Load(filePath + "_stats", PropertiesSet() << "id"  << "playcount" << "lastplay");

	MergeWithStats(channelsStats);
}

bool ChannelSourceModel::SaveStats(const QString& filePath)
{
	Save(filePath + "_stats", PropertiesSet() << "id" << "playcount" << "lastplay");
	return true;
}

ChannelSourceList ChannelSourceModel::Items() const
{
	return m_items;
}

ChannelSourcePtr ChannelSourceModel::Find(int channelId, int genreId) const
{
	foreach(ChannelSourcePtr channel, m_items)
	{
		if(channel->Id() == channelId && (genreId < 0 || channel->GenreId() == genreId))
			return channel;
	}

	return ChannelSourcePtr();
}

void ChannelSourceModel::MergeWithStats(const ChannelSourceModel& channelsWithStats)
{
	foreach(ChannelSourcePtr channel, m_items)
	{
		ChannelSourcePtr channelWithAdditional = channelsWithStats.Find(channel->Id());

		if(!channelWithAdditional.isNull())
		{
			channel->SetPlayCount(channelWithAdditional->PlayCount());
			channel->SetLastPlayTimestamp(channelWithAdditional->LastPlayTimestamp());
		}
	}
}

QVariant ChannelSourceModel::data(const QModelIndex & index, int role) const 
{
	if (index.row() < 0 || index.row() > m_items.count())
		return QVariant();

	const ChannelSourcePtr channel = m_items.at(index.row());
	
	QVariant result;

	switch (role) 
	{
		case Name:
			result = QVariant(channel->Name());
			break;
		case Id:
			result = QVariant(channel->Id());
			break;
		case Url:
			result = QVariant(channel->Url());
			break;
		case GenreId:
			result = QVariant(channel->GenreId());
			break;
	}

	return result;
}

int ChannelSourceModel::rowCount(const QModelIndex &parent) const
{
	int count = m_items.count();
	return count;
}

QHash<int, QByteArray>	ChannelSourceModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[Name] = "Name";
	roles[Id] = "Id";
	roles[Url] = "Url";
	roles[GenreId] = "GenreId";
	
	return roles;
}

}
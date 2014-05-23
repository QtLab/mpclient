#include "ChannelSourceModel.h"

#include <QFile>
#include <QDir>
#include <QHash>
#include <QDebug>

namespace mp {

static const QString IdKeyName				= "id";
static const QString NameKeyName			= "name";
static const QString UrlKeyName				= "url";
static const QString CategoriesKeyName		= "categories";
static const QString PlayCountKeyName		= "playcount";
static const QString LastPlayKeyName		= "lastplay";

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

void ChannelSource::AddCategoryId(uint id)
{
	m_categories.insert(id);
}

CategoryIds ChannelSource::Categories() const
{
	return m_categories;
}

ChannelSourceModel::ChannelSourceModel()
{
}

ChannelSourceModel::~ChannelSourceModel() 
{
}

void ChannelSourceModel::LoadWithStats(const QString& filePath)
{
	QByteArray fileBody;
	if(FileUtils::LoadFileToByteArray(filePath, fileBody))
	{
		ParseChannelsJson(fileBody);

		ChannelSourceModel channelsStats;
		channelsStats.Load(filePath + "st", PropertiesSet() << IdKeyName  << PlayCountKeyName << LastPlayKeyName);

		MergeWithStats(channelsStats);
	}
}

bool ChannelSourceModel::SaveStats(const QString& filePath)
{
	Save(filePath + "st", PropertiesSet() << IdKeyName << PlayCountKeyName << LastPlayKeyName);
	return true;
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
		case FirstCategoryId:
			{
				uint id = (*channel->Categories().begin());
				result = QVariant(id);
				break;
			}
		case Categories:
			result = QVariant(qVariantFromValue(channel->Categories()));
			break;
		case PlayCount:
			result = QVariant(qVariantFromValue(channel->PlayCount()));
			break;
		case LastPlayTimestamp:
			result = QVariant(qVariantFromValue(channel->LastPlayTimestamp()));
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
	roles[FirstCategoryId] = "FirstCategoryId";
	roles[Categories] = "Categories";
	roles[LastPlayTimestamp] = "LastPlayTimestamp";
	roles[PlayCount] = "PlayCount";
		
	return roles;
}

void ChannelSourceModel::ParseChannelsJson(const QByteArray& json)
{
	QJsonParseError parseResult;
	QJsonDocument d = QJsonDocument::fromJson(json, &parseResult);

	if(parseResult.error == QJsonParseError::NoError)
	{
		QJsonArray list = d.array();
		foreach(QJsonValue record, list) 
		{
			QJsonObject object = record.toObject();

			QJsonValue id = object.value(IdKeyName);
			QJsonValue name = object.value(NameKeyName);
			QJsonValue url = object.value(UrlKeyName);
			QJsonValue categories = object.value(CategoriesKeyName);
			
			if(!id.isNull() && !name.isNull() && !url.isNull() && !categories.isNull())
			{
				ChannelSourcePtr item(new ChannelSource);
				
				QString _id = id.toString();

				item->SetId(id.toInt());
				item->SetName(name.toString());
				item->SetUrl(url.toString());

				QJsonArray categoriesJsonArray = categories.toArray();
				QJsonArray::const_iterator iter = categoriesJsonArray.begin();
				while(iter != categoriesJsonArray.end())
				{
					QJsonValue value = (*iter);
					int categoryId = value.toInt(); 
					item->AddCategoryId(categoryId);
					iter++;
				}

				Add(item);
			}
		}

		if(!m_items.empty())
			emit dataChanged(createIndex(0,0),createIndex(m_items.size(),0));
	}
	else
	{
		qDebug() << "ChannelSourceModel json parse error: " << parseResult.errorString();
	}
}

}
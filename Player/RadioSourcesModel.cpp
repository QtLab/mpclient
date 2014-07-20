#include "RadioSourcesModel.h"

#include <QDebug>

namespace mp {
namespace model {

static const QString IdKeyName				= "id";
static const QString NameKeyName			= "name";
static const QString UrlKeyName				= "url";
static const QString CategoriesKeyName		= "categories";
static const QString PlayCountKeyName		= "playcount";
static const QString LastPlayKeyName		= "lastplay";

RadioSource::RadioSource()
	:m_playCount(0)
	,m_lastPlayTimestamp(0)
{
}

RadioSource::~RadioSource()
{
}

const QString& RadioSource::Name() const
{ 
	return m_name; 
}

void RadioSource::SetName(const QString& name)
{ 
	m_name = name; 
}

int RadioSource::Id() const
{
	return m_id;
}

void RadioSource::SetId(int id)
{
	m_id = id;
}

const QString& RadioSource::Url() const
{
	return m_url;
}

void RadioSource::SetUrl(const QString& url)
{
	m_url = url;
}

uint RadioSource::PlayCount() const
{
	return m_playCount;
}

void RadioSource::SetPlayCount(uint count)
{
	m_playCount = count;
}

uint RadioSource::LastPlayTimestamp() const
{
	return m_lastPlayTimestamp;
}

void RadioSource::SetLastPlayTimestamp(uint ts)
{
	m_lastPlayTimestamp = ts;
}

void RadioSource::SetLastPlayNow()
{
	QDateTime dt = QDateTime::currentDateTime();
	m_lastPlayTimestamp = dt.toTime_t();
}

void RadioSource::AddCategoryId(uint id)
{
	m_categories.insert(id);
}

RadioCategoryIds RadioSource::Categories() const
{
	return m_categories;
}

RadioSourcesModel::RadioSourcesModel(QObject* parent)
	:BaseListModel<RadioSource>(parent)
{
}

RadioSourcesModel::~RadioSourcesModel() 
{
}

void RadioSourcesModel::LoadWithStats(const QString& filePath)
{
	QByteArray fileBody;
	if(FileUtils::LoadFileToByteArray(filePath, fileBody))
	{
		ParseChannelsJson(fileBody);

		RadioSourcesModel channelsStats;
		channelsStats.Load(filePath + "st", PropertiesSet() << IdKeyName  << PlayCountKeyName << LastPlayKeyName);

		MergeWithStats(channelsStats);
	}
}

bool RadioSourcesModel::SaveStats(const QString& filePath)
{
	Save(filePath + "st", PropertiesSet() << IdKeyName << PlayCountKeyName << LastPlayKeyName);
	return true;
}

RadioSourcePtr RadioSourcesModel::Find(int channelId) const
{
	foreach(RadioSourcePtr channel, m_items)
	{
		if(channel->Id() == channelId)
			return channel;
	}

	return RadioSourcePtr();
}

void RadioSourcesModel::MergeWithStats(const RadioSourcesModel& channelsWithStats)
{
	foreach(RadioSourcePtr channel, m_items)
	{
		RadioSourcePtr channelWithAdditional = channelsWithStats.Find(channel->Id());

		if(!channelWithAdditional.isNull())
		{
			channel->SetPlayCount(channelWithAdditional->PlayCount());
			channel->SetLastPlayTimestamp(channelWithAdditional->LastPlayTimestamp());
		}
	}
}

QVariant RadioSourcesModel::data(const QModelIndex & index, int role) const 
{
	if (index.row() < 0 || index.row() > m_items.count())
		return QVariant();

	const RadioSourcePtr channel = m_items.at(index.row());
	
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

int RadioSourcesModel::rowCount(const QModelIndex &parent) const
{
	int count = m_items.count();
	return count;
}

QHash<int, QByteArray>	RadioSourcesModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[Id] = "Id";
	roles[Name] = "Name";
	roles[Url] = "Url";
	roles[FirstCategoryId] = "FirstCategoryId";
	roles[Categories] = "Categories";
	roles[LastPlayTimestamp] = "LastPlayTimestamp";
	roles[PlayCount] = "PlayCount";
		
	return roles;
}

void RadioSourcesModel::ParseChannelsJson(const QByteArray& json)
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
				RadioSourcePtr item(new RadioSource);
				
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
		qDebug() << "RadioSourcesModel json parse error: " << parseResult.errorString();
	}
}

} //namespace model
} //namespace mp

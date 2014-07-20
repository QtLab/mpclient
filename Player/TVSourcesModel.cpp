#include "TVSourcesModel.h"
#include "TVGenresModel.h"

#include <QUrl>
#include <QFile>
#include <QFileInfo>
#include <QDebug>

namespace mp {
namespace model {

static const QString IdKeyName				= "id";
static const QString NameKeyName			= "name";
static const QString UrlKeyName				= "url";
static const QString LogoKeyName			= "logo";
static const QString GenresKeyName			= "genres";
static const QString PartnerIdKeyName		= "partnerid";

TVSource::TVSource()
	:m_categoryId(-1)
{
}

TVSource::~TVSource()
{
}

const QString& TVSource::Name() const
{ 
	return m_name; 
}

void TVSource::SetName(const QString& name)
{ 
	m_name = name; 
}

int TVSource::Id() const
{
	return m_id;
}

void TVSource::SetId(int id)
{
	m_id = id;
}

const QString& TVSource::Url() const
{
	return m_url;
}

void TVSource::SetUrl(const QString& url)
{
	m_url = url;
}

const QString& TVSource::Logo() const
{
	return m_logo;
}

void TVSource::SetLogo(const QString& logo)
{
	m_logo = logo;
}

int TVSource::PartnerId() const
{
	return m_partnerId;
}

void TVSource::SetPartnerId(int id)
{
	m_partnerId = id;
}

void TVSource::AddGenreId(int id)
{
	m_genreIds.insert(id);
}

TVGenreIdsSet TVSource::GenreIds() const
{
	return m_genreIds;
}

int TVSource::CategoryId() const
{
	return m_categoryId;
}

void TVSource::SetCategoryId(int id)
{
	m_categoryId = id;
}

void TVSource::SetSelector(const QString& selector)
{
	m_selector = selector;
}

const QString& TVSource::Selector() const
{
	return m_selector;
}

TVSourcesModel::TVSourcesModel(QObject* parent)
	:BaseListModel<TVSource>(parent)
{
}

TVSourcesModel::~TVSourcesModel() 
{
}

void TVSourcesModel::Load(const QString& filePath, const GenreIdsToCategoryIdMap& genresToCategoriesBinding)
{
	QByteArray fileBody;
	if(FileUtils::LoadFileToByteArray(filePath, fileBody))
	{
		ParseChannelsJson(fileBody, genresToCategoriesBinding);
	}
}

TVSourcePtr TVSourcesModel::Find(int channelId) const
{
	foreach(TVSourcePtr channel, m_items)
	{
		if(channel->Id() == channelId)
			return channel;
	}

	return TVSourcePtr();
}

QVariant TVSourcesModel::data(const QModelIndex & index, int role) const 
{
	if (index.row() < 0 || index.row() > m_items.count())
		return QVariant();

	const TVSourcePtr tvSource = m_items.at(index.row());
	
	QVariant result;

	switch (role) 
	{
		case Name:
			result = QVariant(tvSource->Name());
			break;
		case Id:
			result = QVariant(tvSource->Id());
			break;
		case Url:
			result = QVariant(tvSource->Url());
			break;
		case GenreIds:
			result = QVariant(qVariantFromValue(tvSource->GenreIds()));
			break;
		case CategoryId:
			result = QVariant(tvSource->CategoryId());
			break;
		case FirstGenreId:
			{
				uint id = (*tvSource->GenreIds().begin());
				result = QVariant(id);
				break;
			}
		case Logo:
			result = QVariant(tvSource->Logo());
			break;
		case PartnerId:
			result = QVariant(qVariantFromValue(tvSource->PartnerId()));
			break;
	}

	return result;
}

QHash<int, QByteArray>	TVSourcesModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[Id] = "Id";
	roles[Name] = "Name";
	roles[Url] = "Url";
	roles[Logo] = "Logo";
	roles[PartnerId] = "PartnerId";
	roles[FirstGenreId] = "FirstGenreId";
	roles[CategoryId] = "CategoryId";
	roles[GenreIds] = "GenreIds";
		
	return roles;
}

void TVSourcesModel::ParseChannelsJson(const QByteArray& json, const GenreIdsToCategoryIdMap& genresToCategoriesBinding)
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
			QJsonValue logo = object.value(LogoKeyName);
			QJsonValue parnterId = object.value(PartnerIdKeyName);
			QJsonValue genres = object.value(GenresKeyName);
			
			if(!id.isNull() && !name.isNull() && !url.isNull() && !logo.isNull() && !genres.isNull())
			{
				QString imgSource = logo.toString();
				if(!imgSource.startsWith("http", Qt::CaseInsensitive))
				{
					QFileInfo fi(imgSource);

					if(!fi.exists())
					{
						continue;
					}

					imgSource = QUrl::fromLocalFile(fi.absoluteFilePath()).toString();
				}

				TVSourcePtr item(new TVSource);
				
				QString _id = id.toString();

				item->SetId(id.toInt());
				item->SetName(name.toString());
				item->SetUrl(url.toString());
				item->SetLogo(imgSource);

				QJsonArray genresJsonArray = genres.toArray();
				QJsonArray::const_iterator iter = genresJsonArray.begin();
				while(iter != genresJsonArray.end())
				{

					QJsonValue value = (*iter);
					int genreId = value.toInt();

					if(item->CategoryId() < 0)
					{
						GenreIdsToCategoryIdMap::const_iterator iter = genresToCategoriesBinding.find(item->Id());
						if(iter != genresToCategoriesBinding.end())
						{
							item->SetCategoryId(iter.value());
						}
					}

					item->AddGenreId(genreId);
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
		qDebug() << "TVSourcesModel json parse error: " << parseResult.errorString();
	}
}

} //namespace model
} //namespace mp

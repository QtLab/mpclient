#include "TVCategoriesModel.h"

#include <QHash>
#include <QDebug>
#include <QJsonDocument>
#include <QMetaProperty>
#include <QFileInfo>
#include <QUrl>
#include <QFont>
#include <QFontMetrics>

namespace mp {
namespace model {

static const QString IdKeyName					= "id";
static const QString NameKeyName				= "name";
static const QString BannersKeyName				= "banners";
static const QString TopSourcesKeyName			= "topsources";

TVCategory::TVCategory()
	:m_topVisible(false)
{
}

TVCategory::~TVCategory()
{
}

int TVCategory::Id() const
{
	return m_id;
}

void TVCategory::SetId(int id)
{
	m_id = id;
}

QString TVCategory::Name() const
{ 
	return m_name; 
}

void TVCategory::SetName(const QString& name)
{ 
	m_name = name; 
}

bool TVCategory::TopVisible() const
{
	return m_topVisible;
}

void TVCategory::SetTopVisible(bool topVisible)
{
	m_topVisible = topVisible;
}

TVSourceIdsSet TVCategory::TVTopSourceIds() const
{
	return m_tvTopSourceIds;
}

void TVCategory::AddTVTopSourceId(int id)
{
	m_tvTopSourceIds.insert(id);
}

int TVCategory::NextBannerId() const
{
	// TODO: return different ids
	return m_bannerIds.first();
}

BannerInfoIds TVCategory::BannerIds() const
{
	return m_bannerIds;
}

void TVCategory::AddBannerId(int id)
{
	m_bannerIds.append(id);
}

TVCategoriesModel::TVCategoriesModel(QObject* parent)
	:BaseListModel<TVCategory>(parent)
	,m_lastTopVIsibleIndex(0)
{
}

TVCategoriesModel::~TVCategoriesModel()
{
}

void TVCategoriesModel::Load(const QString& filePath)
{
	QByteArray fileBody;
	if(FileUtils::LoadFileToByteArray(filePath, fileBody))
	{
		ParseCatygoriesJson(fileBody);
	}
}

void TVCategoriesModel::ParseCatygoriesJson(const QByteArray& json)
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
			QJsonValue topSources = object.value(TopSourcesKeyName);
			QJsonValue banners = object.value(BannersKeyName);
			
			if(!id.isNull() && !name.isNull() && !topSources.isNull() && !banners.isNull())
			{
				TVCategoryPtr item(new TVCategory);
				
				item->SetId(id.toInt());
				item->SetName(name.toString());

				{
					QJsonArray topSourcesJsonArray = topSources.toArray();
					QJsonArray::const_iterator iter = topSourcesJsonArray.begin();
					while(iter != topSourcesJsonArray.end())
					{
						QJsonValue value = (*iter);
						int sourceId = value.toInt(); 
						item->AddTVTopSourceId(sourceId);
						iter++;
					}
				}

				{
					QJsonArray bannersJsonArray = banners.toArray();
					QJsonArray::const_iterator bannrIter = bannersJsonArray.begin();
					while(bannrIter != bannersJsonArray.end())
					{
						QJsonValue value = (*bannrIter);
						int bannerId = value.toInt(); 
						item->AddBannerId(bannerId);
						bannrIter++;
					}
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



int TVCategoriesModel::UpdateTopVisibleCategories(int maxWidth, const QString& fontFamily, int pointSize, int spacing)
{
	if(maxWidth < 0)
	{
		return 0;
	}

	int actualVListWidh = 0;
	int lastTopVIsibleIndex = -1;

	QFont font(fontFamily, pointSize);
	QFontMetrics fm(font);

	int index = 0;

	bool allNextCategoriesInvisible = false;

	foreach(TVCategoryPtr category, m_items)
	{
		if(allNextCategoriesInvisible)
		{
			category->SetTopVisible(false);
		}
		else
		{
			int textWidth = fm.width(category->Name());

			if((actualVListWidh + (textWidth + spacing)) <= maxWidth)
			{
				actualVListWidh += (textWidth + spacing);
				category->SetTopVisible(true);
				lastTopVIsibleIndex = index;
			}
			else
			{
				allNextCategoriesInvisible = true;
				category->SetTopVisible(false);
			}
		}

		index++;
	}

	if(m_lastTopVIsibleIndex != lastTopVIsibleIndex)
	{
		m_lastTopVIsibleIndex = lastTopVIsibleIndex;
		EmitDataChanged();
	}

	return actualVListWidh;
}

void TVCategoriesModel::InsertFirstTopVisibleCategory(int id, int maxWidth, const QString& fontFamily, int pointSize, int spacing)
{
	for(int i = 0; i < m_items.count(); i++ )
	{
		TVCategoryPtr category = m_items[i];

		if(category->Id() == id)
		{
			if(!category->TopVisible())
			{
				m_items.move(i, 0);
			}
			
			break;
		}
	}

	UpdateTopVisibleCategories(maxWidth, fontFamily, pointSize, spacing);
}

void TVCategoriesModel::InsertLastTopVisibleCategory(int id, int maxWidth, const QString& fontFamily, int pointSize, int spacing)
{
	TVCategoryPtr insertedCategory;

	for(int i = 0; i < m_items.count(); i++ )
	{
		TVCategoryPtr category = m_items[i];

		if(category->Id() == id)
		{
			if(category->TopVisible())
			{
				return;
			}

			insertedCategory = category;
			m_items.removeAt(i);
			break;
		}
	}

	if(insertedCategory)
	{
		int actualVListWidh = UpdateTopVisibleCategories(maxWidth, fontFamily, pointSize, spacing);

		QFont font(fontFamily, pointSize);
		QFontMetrics fm(font);

		int insertedCategoryTextWidth = fm.width(insertedCategory->Name());

		for(int i = m_lastTopVIsibleIndex; i >=0; i--)
		{
			if((actualVListWidh + (insertedCategoryTextWidth + spacing)) <= maxWidth)
			{
				insertedCategory->SetTopVisible(true);
				actualVListWidh += (insertedCategoryTextWidth + spacing);
				m_items.insert(i + 1, insertedCategory);
				break;
			}
			else
			{
				TVCategoryPtr category = m_items[i];
				category->SetTopVisible(false);
				int textWidth = fm.width(category->Name());
				actualVListWidh -= (textWidth + spacing);
			}
		}

		EmitDataChanged();
	}
}

int TVCategoriesModel::RowIndexById(int id) const
{
	int row = 0;
	foreach(TVCategoryPtr category, m_items)
	{
		if(category->Id() == id)
		{
			int rowIndex = index(row).row();
			return rowIndex;
		}

		row++;
	}
	
	return -1;
}

TVCategoryPtr TVCategoriesModel::FindById(int id) const
{
	foreach(TVCategoryPtr category, m_items)
	{
		if(category->Id() == id)
		{
			return category;
		}
	}

	return TVCategoryPtr();
}

QVariant TVCategoriesModel::data(const QModelIndex & index, int role) const 
{
	if (index.row() < 0 || index.row() > m_items.count())
		return QVariant();

	const TVCategoryPtr contact = m_items.at(index.row());
	
	QVariant result;

	switch (role) 
	{
		case Name:
			result = QVariant(contact->Name());
			break;
		case Id:
			result = QVariant(contact->Id());
			break;
		case TopVisible:
			result = QVariant(contact->TopVisible());
			break;
		case TVTopSourceIds:
			result = QVariant(qVariantFromValue(contact->TVTopSourceIds()));
			break;
		case BannerIds:
			result = QVariant(qVariantFromValue(contact->BannerIds()));
			break;
	}

	return result;
}

int TVCategoriesModel::rowCount(const QModelIndex &parent) const
{
	return m_items.count();
}

QHash<int, QByteArray>	TVCategoriesModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[Name] = "Name";
	roles[Id] = "Id";
	roles[TopVisible] = "TopVisible";
	roles[TVTopSourceIds] = "TVTopSourceIds";
	roles[BannerIds] = "BannerIds";

	return roles;
}

void TVCategoriesModel::Cleanup()
{
	BaseListModel<TVCategory>::Cleanup();
	m_lastTopVIsibleIndex = 0;
}

} //namespace model
} //namespace mp

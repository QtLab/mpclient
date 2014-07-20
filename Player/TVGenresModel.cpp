#include "TVGenresModel.h"

#include <QHash>
#include <QDebug>
#include <QJsonDocument>
#include <QMetaProperty>

namespace mp {
namespace model {

static const QString IdKeyName					= "id";
static const QString NameKeyName				= "name";
static const QString CategoryIdKeyName			= "categoryid";

TVGenre::TVGenre()
{
}

TVGenre::~TVGenre()
{
}

int TVGenre::Id() const
{
	return m_id;
}

void TVGenre::SetId(int id)
{
	m_id = id;
}

QString TVGenre::Name() const
{ 
	return m_name; 
}

void TVGenre::SetName(const QString& name)
{ 
	m_name = name; 
}

int TVGenre::CategoryId() const
{
	return m_categoryId;
}

void TVGenre::SetCategoryId(int id)
{
	m_categoryId = id;
}

TVGenresModel::TVGenresModel(QObject* parent)
	:BaseListModel<TVGenre>(parent)
{
}

TVGenresModel::~TVGenresModel()
{
}


int TVGenresModel::RowIndexById(int id) const
{
	int row = 0;
	foreach(TVGenrePtr category, m_items)
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

TVGenrePtr TVGenresModel::FindById(int id) const
{
	foreach(TVGenrePtr category, m_items)
	{
		if(category->Id() == id)
		{
			return category;
		}
	}

	return TVGenrePtr();
}

const GenreIdsToCategoryIdMap& TVGenresModel::GenreIdsToCategoryIdBindingMap() const
{
	return m_genreIdsToCategoryIdToMap;
}

void TVGenresModel::Cleanup()
{
	BaseListModel<TVGenre>::Cleanup();
	m_genreIdsToCategoryIdToMap.clear();
}

QVariant TVGenresModel::data(const QModelIndex & index, int role) const 
{
	if (index.row() < 0 || index.row() > m_items.count())
		return QVariant();

	const TVGenrePtr contact = m_items.at(index.row());
	
	QVariant result;

	switch (role) 
	{
		case Name:
			result = QVariant(contact->Name());
			break;
		case Id:
			result = QVariant(contact->Id());
			break;
		case CategoryId:
			result = QVariant(contact->CategoryId());
			break;
	}

	return result;
}

QHash<int, QByteArray>	TVGenresModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[Name] = "Name";
	roles[Id] = "Id";
	roles[CategoryId] = "CategoryId";

	return roles;
}

void TVGenresModel::ParseJson(const QByteArray& json, const PropertiesSet& properties)
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
			QJsonValue categoryId = object.value(CategoryIdKeyName);

			if(!id.isNull() && !name.isNull() && !categoryId.isNull())
			{

				TVGenrePtr item(new TVGenre);
				
				QString _id = id.toString();

				item->SetId(id.toInt());
				item->SetName(name.toString());
				item->SetCategoryId(categoryId.toInt());

				m_genreIdsToCategoryIdToMap.insert(item->Id(), item->CategoryId());

				Add(item);
			}
		}

		
		if(rowCount() > 0)
			emit dataChanged(createIndex(0,0),createIndex(m_items.size(),0));
	}
	else
	{
		qDebug() << "TVGenresModel json parse error: " << parseResult.errorString();
	}
}

} //namespace model
} //namespace mp

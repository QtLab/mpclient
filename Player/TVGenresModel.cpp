#include "TVGenresModel.h"

#include <QHash>
#include <QDebug>
#include <QJsonDocument>
#include <QMetaProperty>

namespace mp {
namespace model {

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

int TVGenresModel::rowCount(const QModelIndex &parent) const
{
	return m_items.count();
}

QHash<int, QByteArray>	TVGenresModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[Name] = "Name";
	roles[Id] = "Id";
	roles[CategoryId] = "CategoryId";

	return roles;
}

} //namespace model
} //namespace mp

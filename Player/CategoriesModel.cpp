#include "CategoriesModel.h"

#include <QFile>
#include <QDir>
#include <QHash>
#include <QDebug>
#include <QJsonDocument>
#include <QMetaProperty>

namespace mp {
namespace model {

Category::Category()
{
}

Category::~Category()
{
}

int Category::Id() const
{
	return m_id;
}

void Category::SetId(int id)
{
	m_id = id;
}

QString Category::Name() const
{ 
	return m_name; 
}

void Category::SetName(const QString& name)
{ 
	m_name = name; 
}

CategoriesModel::CategoriesModel()
{
}

CategoriesModel::~CategoriesModel() 
{
}

int CategoriesModel::RowIndexById(int id) const
{
	int row = 0;
	foreach(CategoryPtr category, m_items)
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

CategoryPtr CategoriesModel::FindById(int id) const
{
	foreach(CategoryPtr category, m_items)
	{
		if(category->Id() == id)
		{
			return category;
		}
	}

	return CategoryPtr();
}

QVariant CategoriesModel::data(const QModelIndex & index, int role) const 
{
	if (index.row() < 0 || index.row() > m_items.count())
		return QVariant();

	const CategoryPtr contact = m_items.at(index.row());
	
	QVariant result;

	switch (role) {
	case Name:
		result = QVariant(contact->Name());
		break;
	case Id:
		result = QVariant(contact->Id());
		break;
	}

	return result;
}

int CategoriesModel::rowCount(const QModelIndex &parent) const
{
	return m_items.count();
}

QHash<int, QByteArray>	CategoriesModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[Name] = "Name";
	roles[Id] = "Id";

	return roles;
}

} //End namespace model
} //End namespace mp
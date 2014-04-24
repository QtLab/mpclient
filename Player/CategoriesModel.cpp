#include "CategoriesModel.h"

#include <QFile>
#include <QDir>
#include <QHash>
#include <QDebug>
#include <QJsonDocument>
#include <QMetaProperty>

namespace mp {

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

CategoryList CategoriesModel::Items() const
{
	return m_items;
}

CategoryPtr CategoriesModel::FindById(int id)
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
	//QReadLocker locker(&m_lock);

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
	//if(m_lock.tryLockForRead())
	{
		int count = m_items.count();
		//m_lock.unlock();
		return count;
	}

	return m_items.count();
}

QHash<int, QByteArray>	CategoriesModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[Name] = "Name";
	roles[Id] = "Id";

	return roles;
}

}
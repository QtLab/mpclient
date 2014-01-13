#include "GenreModel.h"

#include <QFile>
#include <QDir>
#include <QHash>
#include <QDebug>
#include <QJsonDocument>
#include <QMetaProperty>

namespace mp {

GenreItem::GenreItem()
{
}

GenreItem::~GenreItem()
{
}

QString GenreItem::Name() const
{ 
	return m_name; 
}

void GenreItem::SetName(const QString& name)
{ 
	m_name = name; 
}

QString GenreItem::Id() const
{
	return m_id;
}

void GenreItem::SetId(const QString& id)
{
	m_id = id;
}

GenreModel::GenreModel()
{
}

GenreModel::~GenreModel() 
{
}

GenreItemList GenreModel::Items() const
{
	return m_items;
}

GenreItemPtr GenreModel::FindById(const QString& id)
{
	GenreItemPtr genre;
	foreach(genre, m_items)
	{
		if(genre->Id() == id)
		{
			return genre;
		}
	}

	return GenreItemPtr();
}

QVariant GenreModel::data(const QModelIndex & index, int role) const 
{
	//QReadLocker locker(&m_lock);

	if (index.row() < 0 || index.row() > m_items.count())
		return QVariant();

	const GenreItemPtr contact = m_items.at(index.row());
	
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

int GenreModel::rowCount(const QModelIndex &parent) const
{
	//if(m_lock.tryLockForRead())
	{
		int count = m_items.count();
		//m_lock.unlock();
		return count;
	}

	return m_items.count();
}

QHash<int, QByteArray>	GenreModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[Name] = "Name";
	roles[Id] = "Id";

	return roles;
}

}
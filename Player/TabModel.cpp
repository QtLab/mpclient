#include "TabModel.h"

#include <QFile>
#include <QDir>
#include <QHash>
#include <QDebug>
#include <QCryptographicHash>

namespace mp {

TabModelItem::TabModelItem()
{
}

TabModelItem::~TabModelItem()
{
}

QString TabModelItem::Id() const
{
	return m_id;
}

void TabModelItem::SetId(const QString& id)
{
	m_id = id;
}

QString TabModelItem::Name() const
{
	return m_name;
}

void TabModelItem::SetName(const QString& n)
{
	m_name = n;
}

QString TabModelItem::Arguments() const
{
	return m_arguments;
}

void TabModelItem::SetArguments(const QString& n)
{
	m_arguments = n;
}

TabModel::TabModel()
{
}

TabModel::~TabModel()
{
}

TabModelItemList TabModel::Items() const
{
	return m_tabs;
}

QVariant TabModel::data(const QModelIndex &index, int role) const
{
	QReadLocker locker(&m_lock);

	if (index.row() < 0 || index.row() > m_tabs.count())
		return QVariant();

	const TabModelItemPtr item = m_tabs.at(index.row());
	
	QVariant result;

	switch (role) {
	case Id:
		result = QVariant(item->Id());
		break;
	case Name:
		result = QVariant(item->Name());
		break;
		break;
	}

	return result;
}

int TabModel::rowCount(const QModelIndex &parent) const
{
	if(m_lock.tryLockForRead())
	{
		int count = m_tabs.count();
		m_lock.unlock();
		return count;
	}
	else
	{
		return m_tabs.count();
	}
}

QHash<int, QByteArray>	TabModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[Id] = "Id";
	roles[Name] = "Name";
	roles[Arguments] = "Args";

	return roles;
}

void TabModel::Add(TabModelItemPtr contact, bool notifiChanged)
{
}

void TabModel::Load(const QString& filePath)
{
	QFile file(filePath);
	if(file.open(QIODevice::ReadOnly))
	{
		QByteArray arr = file.readAll();
#ifdef _DEBUG
		//QString json = QString::fromAscii(arr.data(), arr.size());
#endif
		Parse(arr);
	}
	else
	{
		qDebug() << "UpdateModel::Load can't open file: " << filePath;
	}
}


void TabModel::Parse(const QByteArray& json)
{
	/*
	QJson::Parser parser;
	bool ok;

	QVariantList result = parser.parse(json, &ok).toList();

	if(ok)
	{
		QWriteLocker locker(&m_lock);

		m_tabs.clear();
		QMap<QString, QVariant>::iterator i;

		foreach(QVariant record, result) 
		{
			TabModelItemPtr tab(new TabModelItem());

			QJson::QObjectHelper::qvariant2qobject(record.toMap(), tab.data());

			m_tabs.append(TabModelItemPtr(tab));
		}

		if(!m_tabs.empty())
			emit dataChanged(createIndex(0,0),createIndex(m_tabs.size(),0));
	}
	else
	{
		QString error = parser.errorString();
		qDebug() << "UpdateModel::Parse error: " << error;
	}
	*/
}

}
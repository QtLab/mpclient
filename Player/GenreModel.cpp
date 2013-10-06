#include "GenreModel.h"

#include <QFile>
#include <QDir>
#include <QHash>
#include <QDebug>
#include <QJson/Parser>
#include <QJson/Serializer>
#include <QJson/QObjectHelper>

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
	QHash<int, QByteArray> roles;
	roles[Name] = "Name";
	roles[Id] = "Id";
	setRoleNames(roles);
}

GenreModel::~GenreModel() 
{
}

void GenreModel::Add(GenreItemPtr channel, bool notifiChanged)
{
	beginInsertRows(QModelIndex(), m_genres.length(), m_genres.length());
	m_genres.append(channel);
	endInsertRows();

	if(notifiChanged)
	{
		//говорим вью, что данные изменились
		emit dataChanged(createIndex(0,0),createIndex(m_genres.size(),0));
	}
}

void GenreModel::Load(const QString& path)
{
	QFile file(path);
	if(file.open(QIODevice::ReadOnly))
	{
		QByteArray arr = file.readAll();
#ifdef _DEBUG
		QString json = QString::fromAscii(arr.data(), arr.size());
#endif
		Parse(arr);
	}
	else
	{
		qDebug() << "GenreModel::Load can't open file: " << path;
	}
}

//[
//{"id": "1", "name": "PSYCHEDELIK"},
//{"Id": "1", "name": "PSYCHEDELIK2"}
//]

void GenreModel::Parse(const QByteArray& json)
{
	QJson::Parser parser;
	bool ok;

	QVariantList result = parser.parse(json, &ok).toList();

	if(ok)
	{
		QWriteLocker locker(&m_lock);

		QMap<QString, QVariant>::iterator i;

		foreach(QVariant record, result) 
		{
			GenreItem * genre = new GenreItem();

			QJson::QObjectHelper::qvariant2qobject(record.toMap(), genre);
			Add(GenreItemPtr(genre));
		}
	}
	else
	{
		QString error = parser.errorString();
		qDebug() << "GenreModel::Parse error: " << error;
	}
}

void GenreModel::Cleanup()
{
	QWriteLocker locker(&m_lock);
	m_genres.clear();
}

const GenreItemList& GenreModel::Items() const
{
	return m_genres;
}

GenreItemPtr GenreModel::FindById(const QString& id)
{
	GenreItemPtr genre;
	foreach(genre, m_genres)
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
	QReadLocker locker(&m_lock);

	if (index.row() < 0 || index.row() > m_genres.count())
		return QVariant();

	const GenreItemPtr contact = m_genres.at(index.row());
	
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
	if(m_lock.tryLockForRead())
	{
		int count = m_genres.count();
		m_lock.unlock();
		return count;
	}
	else
	{
		return m_genres.count();
	}
}

}
#include "ChannelSourceModel.h"
#include "UrlModel.h"

#include <QFile>
#include <QDir>
#include <QHash>
#include <QDebug>
#include <QJson/Parser>
#include <QJson/Serializer>
#include <QJson/QObjectHelper>

namespace mp {

ChannelSource::ChannelSource()
{
}

ChannelSource::~ChannelSource()
{
}

QString ChannelSource::Name() const
{ 
	return m_name; 
}

void ChannelSource::SetName(const QString& name)
{ 
	m_name = name; 
}

QString ChannelSource::Id() const
{
	return m_id;
}

void ChannelSource::SetId(const QString& id)
{
	m_id = id;
}

QString ChannelSource::Logo() const
{
	return m_logo;
}

void ChannelSource::SetLogo(const QString& logo)
{
	m_logo = logo;
}

QString ChannelSource::Url() const
{
	return m_url;
}

void ChannelSource::SetUrl(const QString& url)
{
	m_url = url;
}

QString ChannelSource::GenreId() const
{
	return m_genreId;
}

void ChannelSource::SetGenreId(const QString& genreId)
{
	m_genreId = genreId;
}

GenreItemPtr ChannelSource::Genre() const
{
	return m_genre;
}

void ChannelSource::SetGenre(const GenreItemPtr& genre)
{
	m_genre = genre;
}

ChannelSourceModel::ChannelSourceModel()
{
	QHash<int, QByteArray> roles;
	roles[Name] = "Name";
	roles[Id] = "Id";
	roles[Logo] = "Logo";
	roles[Url] = "Url";
	setRoleNames(roles);
}

ChannelSourceModel::~ChannelSourceModel() 
{
}

void ChannelSourceModel::Add(ChannelSourcePtr channel, bool notifiChanged)
{
	beginInsertRows(QModelIndex(), m_channels.length(), m_channels.length());
	m_channels.append(channel);
	endInsertRows();

	if(notifiChanged)
	{
		//говорим вью, что данные изменились
		emit dataChanged(createIndex(0,0),createIndex(m_channels.size(),0));
	}
}

void ChannelSourceModel::Load(const QString& path)
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
		qDebug() << "ChannelSourceModel::Load can't open file: " << path;
	}
}

void ChannelSourceModel::SetGenres(const GenreModel& genres)
{
	m_genres.Cleanup();
	m_genres.insertRows(0, genres.Items().count(), genres.index(0));
}

//[
//{"id": "1", "name": "PSYCHEDELIK", "logo": "", "url": "http://88.191.104.69:8002/"},
//{"Id": "1", "name": "PSYCHEDELIK2", "logo": "", "url": "http://88.191.104.69:8002/"}
//]

void ChannelSourceModel::Parse(const QByteArray& json)
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
			ChannelSource * channel = new ChannelSource();

			QJson::QObjectHelper::qvariant2qobject(record.toMap(), channel);

			GenreItemPtr genre = m_genres.FindById(channel->GenreId());
			channel->SetGenre(genre);

			// Convert to absolute path
			channel->SetLogo(UrlModel::CreateCurrentDirUrl(channel->Logo()).ToUrl());
			Add(ChannelSourcePtr(channel));
		}

		if(!m_channels.empty())
			emit dataChanged(createIndex(0,0),createIndex(m_channels.size(),0));
	}
	else
	{
		QString error = parser.errorString();
		qDebug() << "ChannelSourceModel::Parse error: " << error;
	}
}

void ChannelSourceModel::Cleanup()
{
	QWriteLocker locker(&m_lock);
	m_channels.clear();
}

const ChannelSourceList& ChannelSourceModel::Items() const
{
	return m_channels;
}

ChannelSourcePtr ChannelSourceModel::FindById(const QString& id)
{
	QReadLocker locker(&m_lock);

	foreach(ChannelSourcePtr channel, m_channels)
	{
		if(channel->Id() == id)
			return channel;
	}
}

QVariant ChannelSourceModel::data(const QModelIndex & index, int role) const 
{
	QReadLocker locker(&m_lock);

	if (index.row() < 0 || index.row() > m_channels.count())
		return QVariant();

	const ChannelSourcePtr contact = m_channels.at(index.row());
	
	QVariant result;

	switch (role) {
	case Name:
		result = QVariant(contact->Name());
		break;
	case Id:
		result = QVariant(contact->Id());
		break;
	case Logo:
		result = QVariant(contact->Logo());
		break;
	case Url:
		result = QVariant(contact->Url());
		break;
	}

	return result;
}

int ChannelSourceModel::rowCount(const QModelIndex &parent) const
{
	if(m_lock.tryLockForRead())
	{
		int count = m_channels.count();
		m_lock.unlock();
		return count;
	}
	else
	{
		return m_channels.count();
	}
}

}
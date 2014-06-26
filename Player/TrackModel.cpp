#include "TrackModel.h"
#include "Config.h"
#include "Path.h"

#include <QJsonDocument>
#include <QDomDocument>
#include <QUrl>

namespace mp {
namespace model {

static const QString IdKeyName				= "id";
static const QString ArtistKeyName			= "artist";
static const QString TitleKeyName			= "title";
static const QString UrlKeyName				= "url";
static const QString DurationKeyName		= "duration";

Track::Track()
	:m_downloading(false)
{
}

Track::~Track()
{
}

int Track::Id() const
{
	return m_id;
}

void Track::SetId(int id)
{
	m_id = id;
}

const QString& Track::Artist() const
{
	return m_artist;
}

void Track::SetArtist(const QString& artist)
{
	m_artist = artist;
}

const QString& Track::Title() const
{
	return m_title;
}

void Track::SetTitle(const QString& title)
{
	m_title = title;
}

const QString& Track::Url() const
{
	return m_url;
}

void Track::SetUrl(const QString& url)
{
	m_url = url;
}

int Track::Duration() const
{
	return m_duration;
}

void Track::SetDuration(int duration)
{
	m_duration = duration;
}

QString Track::FilePath() const
{
	if(m_path.isEmpty())
	{
		m_path = Path::TrackFile(m_title);
	}

	return m_path;
}

void Track::SetFilePath(const QString& path)
{
	m_path = path;
}

bool Track::FileExists() const
{
	return Path::Exists(FilePath());
}

bool Track::Downloading() const
{
	return m_downloading;
}

void Track::SetDownloading(bool state)
{
	m_downloading = state;
}

TrackModel::TrackModel()
{
}

TrackModel::~TrackModel()
{
}

TrackPtr TrackModel::Find(int trackId) const
{
	foreach(TrackPtr track, m_items)
	{
		if(track->Id() == trackId)
			return track;
	}

	return TrackPtr();
}

bool TrackModel::Remove(int id)
{
	for(int i = 0; i < m_items.count(); i++)
	{
		TrackPtr track = m_items.at(i);

		if(track->Id() == id)
		{
			beginRemoveRows(QModelIndex(), i, i);
			m_items.removeAt(i);
			endRemoveRows();

			return true;
		}
	}

	return false;
	/*
	ItemList::iterator iter = m_items.begin();
	while(iter != m_items.end())
	{
		if((*iter)->Id() == id)
		{
			//c
			m_items.erase(iter);
			return true;
		}
	}
	*/

	return false;
}

bool TrackModel::LoadFrom(const QByteArray& data, TrackModel::SourceType source)
{
	bool result = false;

	switch(source)
	{
		case ST_Inixmedia:
			result = ParseInixmedia(data);
			break;
		case ST_Mp3xml:
			result = ParseMp3xml(data);
			break;
	}

	return result;
}

QVariant TrackModel::data(const QModelIndex &index, int role) const
{
	if (index.row() < 0 || index.row() > m_items.count())
		return QVariant();

	const TrackPtr track = m_items.at(index.row());
	
	QVariant result;

	switch (role) 
	{
		case Id:
			result = QVariant(track->Id());
			break;
		case Artist:
			result = QVariant(track->Artist());
			break;
		case Title:
			result = QVariant(track->Title());
			break;
		case Url:
			result = QVariant(track->Url());
			break;
		case Duration:
			result = QVariant(track->Duration());
			break;
		case FileExists:
			result = QVariant(track->FileExists());
			break;
		case Downloading:
			result = QVariant(track->Downloading());
			break;
	}

	return result;
}

bool TrackModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if (index.row() < 0 || index.row() > m_items.count())
		return false;

	if(role == Downloading)
	{
		const TrackPtr track = m_items.at(index.row());
		track->SetDownloading(value.toBool());
		return true;
	}
	else
	{
		return BaseListModel<Track>::setData(index, value, role);
	}
}

QHash<int, QByteArray> TrackModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[Id] = "Id";
	roles[Artist] = "Artist";
	roles[Title] = "Title";
	roles[Url] = "Url";
	roles[Duration] = "Duration";
	roles[FileExists] = "FileExists";
	roles[Downloading] = "Downloading";

	return roles;
}

bool TrackModel::ParseInixmedia(const QByteArray& json)
{
	bool success = false;

	QJsonParseError parseResult;
	QJsonDocument doc = QJsonDocument::fromJson(json, &parseResult);

	if(parseResult.error == QJsonParseError::NoError)
	{
		QJsonObject rootObj = doc.object();
		QJsonObject::const_iterator iter = rootObj.find("response");
		
		if(iter != rootObj.end())
		{
			QJsonArray tracks = iter.value().toArray();

			foreach(QJsonValue record, tracks) 
			{
				QJsonObject trackJson = record.toObject();

				int id = trackJson.value("aid").toInt();
				if(id > 0)
				{
					TrackPtr track(new Track());
				
					track->SetId(id);
					track->SetArtist(trackJson.value("artist").toString());
					track->SetTitle(trackJson.value("title").toString());
					track->SetDuration(trackJson.value("duration").toInt());
					track->SetUrl(trackJson.value("url").toString());

					Add(track);
				}
			}

			if(tracks.count() > 0)
				emit dataChanged(createIndex(0,0),createIndex(m_items.size(),0));

			success = true;
		}
	}
	else
	{
		qDebug() << "TrackModel::ParseInixmedia json parse error: " << parseResult.errorString();
	}

	return success;
}

bool TrackModel::ParseMp3xml(const QByteArray& xml)
{
	bool success = false;

	QString error;

	QDomDocument dom;
	dom.setContent(xml, &error);

	if(error.isEmpty())
	{
		QDomNodeList tracksNodes = dom.namedItem("data").namedItem("items").childNodes();

		QTime timeZero(0, 0, 0, 0);

		for(int i =0; i < tracksNodes.size(); i++)
		{
			QDomNode trackNode = tracksNodes.at(i);

			TrackPtr track(new Track());

			track->SetId(trackNode.namedItem("song_id").firstChild().nodeValue().toInt());
			track->SetArtist(trackNode.namedItem("artist").firstChild().nodeValue());
			track->SetTitle(trackNode.namedItem("song").firstChild().nodeValue());

			QString duration = trackNode.namedItem("duration").firstChild().nodeValue();

			// TODO: parsing for hours
			QTime time = QTime::fromString(duration, "mm:ss");
			int seconds = timeZero.secsTo(time);

			track->SetDuration(seconds);
			track->SetUrl(trackNode.namedItem("orig_url").firstChild().nodeValue());

			Add(track);
		}

		if(tracksNodes.size() > 0)
			emit dataChanged(createIndex(0,0),createIndex(m_items.size(),0));

		success = true;

	}
	else
	{
		qDebug() << "TrackModel::ParseMp3xml xml parse error: " << error;
	}

	return success;
}

} //End namespace model
} //End namespace mp
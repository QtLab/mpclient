#ifndef MP_TRACKS_MODEL_H
#define MP_TRACKS_MODEL_H

#include "Prerequirements.h"
#include "BaseListModel.h"

namespace mp {
namespace model {

class Track : public QObject
{
	Q_OBJECT
	Q_PROPERTY(int id READ Id WRITE SetId)
	Q_PROPERTY(QString artist READ Artist WRITE SetArtist)
	Q_PROPERTY(QString title READ Title WRITE SetTitle)
	Q_PROPERTY(QString url READ Url WRITE SetUrl)
	Q_PROPERTY(int path READ Duration WRITE SetDuration)
	Q_PROPERTY(int duration READ Duration WRITE SetDuration)

public:

	Track();
	virtual ~Track();

	int Id() const;
	void SetId(int id);

	const QString& Artist() const;
	void SetArtist(const QString& artist);

	const QString& Title() const;
	void SetTitle(const QString& title);

	// Url to channel
	const QString& Url() const;
	void SetUrl(const QString& url);

	int Duration() const;
	void SetDuration(int duration);

	QString FilePath() const;
	void SetFilePath(const QString& path);
	bool FileExists() const;

	bool Downloading() const;
	void SetDownloading(bool state);

public:
	int						m_id;
	QString					m_artist;
	QString					m_title;
	QString					m_url;
	mutable QString			m_path;
	int						m_duration;
	bool					m_downloading;
	Q_DISABLE_COPY(Track)
};

class TrackModel : public BaseListModel<Track>
{
	Q_OBJECT

public:
	enum TrackRoles
	{
		Id = Qt::UserRole + 1,
		Artist,
		Title,
		Url,
		Duration,
		FileExists,
		Downloading
	};

	enum SourceType
	{
		ST_Inixmedia,
		ST_Mp3xml
	};

	TrackModel();
	virtual ~TrackModel();

	TrackPtr Find(int trackId) const;

	bool Remove(int id);

	bool LoadFrom(const QByteArray& data, SourceType source);
	

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

	QHash<int, QByteArray>	roleNames() const;

private:
	bool ParseInixmedia(const QByteArray& json); 
	bool ParseMp3xml(const QByteArray& xml);

public:
	friend QAbstractListModel;
	Q_DISABLE_COPY(TrackModel)
};

} //End namespace model
} //End namespace mp

#endif

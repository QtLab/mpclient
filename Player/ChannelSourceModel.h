#ifndef MP_CHANNEL_SOURCE_H
#define MP_CHANNEL_SOURCE_H

#include <QMetaType>
#include <QSharedPointer>
#include <QAbstractListModel>
#include <QDeclarativeListProperty>
#include <QReadWriteLock>

#include "GenreModel.h"

namespace mp {


class ChannelSource : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString name READ Name WRITE SetName)
	Q_PROPERTY(QString id READ Id WRITE SetId)
	Q_PROPERTY(QString logo READ Logo WRITE SetLogo)
	Q_PROPERTY(QString url READ Url WRITE SetUrl)
	Q_PROPERTY(QString genreid READ GenreId WRITE SetGenreId)
	Q_PROPERTY(GenreItemPtr genre READ Genre)

public:
	ChannelSource();
	~ChannelSource();

	QString Id() const;
	void SetId(const QString& id);

	QString Name() const;
	void SetName(const QString& name);

	// Preview
	QString Logo() const;
	void SetLogo(const QString& logo);

	// Url to channel
	QString Url() const;
	void SetUrl(const QString& url);

	QString GenreId() const;
	void SetGenreId(const QString& genreId);

	GenreItemPtr Genre() const;
	void SetGenre(const GenreItemPtr& genre);

public:
	QString					m_id;
	QString					m_name;
	QString					m_logo;
	QString					m_url;
	GenreItemPtr			m_genre;
	QString					m_genreId;

	Q_DISABLE_COPY(ChannelSource)
};

typedef QSharedPointer<ChannelSource> ChannelSourcePtr;
typedef QList<ChannelSourcePtr> ChannelSourceList;
typedef QDeclarativeListProperty<ChannelSource*> DeclarativeChannels;

class ChannelSourceModel : public QAbstractListModel
{
	Q_OBJECT
	//Q_PROPERTY(DeclarativeChannels Items READ DeclarativeItems CONSTANT)

public:
	enum ChannelSourceRoles
	{
		Name = Qt::UserRole + 1,
		Id,
		Logo,
		Url,
		GenreId
	};

	ChannelSourceModel();
	virtual ~ChannelSourceModel();

	// Don't use in prod. It isn't thread-safe
	//DeclarativeChannels DeclarativeItems() const;
	ChannelSourceList Items() const;

	ChannelSourcePtr FindById(const QString& id);

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	void Add(ChannelSourcePtr contact, bool notifiChanged = false);
	void Load(const QString& filePath);
	void SetGenres(const GenreModel& genres);
	//[
	//{"id": "1", "name": "PSYCHEDELIK", "logo": "", "url": "http://88.191.104.69:8002/"},
	//{"Id": "1", "name": "PSYCHEDELIK2", "logo": "", "url": "http://88.191.104.69:8002/"}
	//]
	void Parse(const QByteArray& json);
	void Cleanup();

public:
	friend class TabPagesController;
	friend class RadioCompositeModel;
	friend class RadioPageController;

	ChannelSourceList			m_channels;
	GenreModel					m_genres;
	//mutable QReadWriteLock		m_lock;

	Q_DISABLE_COPY(ChannelSourceModel)
};

//typedef QSharedPointer<ChannelSourceModel> ChannelSourceModelPtr;

//typedef ChannelSourceModel * ChannelSourceModelPtr;
//Q_DECLARE_METATYPE(ChannelSourceModelPtr);
//Q_DECLARE_METATYPE(ChannelSourcePtr);
}

Q_DECLARE_METATYPE(mp::ChannelSource);
Q_DECLARE_METATYPE(mp::ChannelSourceModel);

#endif
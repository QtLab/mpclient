#ifndef MP_CHANNEL_SOURCE_H
#define MP_CHANNEL_SOURCE_H

#include <QMetaType>
#include <QSharedPointer>
#include <QAbstractListModel>
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
	Q_PROPERTY(QString url READ Url WRITE SetUrl)
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

	GenreModelPtr Genre() const;
	const QString& GenreId() const;
	void SetGenreId(const QString& id);

private:
	QString					m_id;
	QString					m_name;
	QString					m_logo;
	QString					m_url;
	QString					m_genreId;
	GenreModelPtr			m_genreModel;			
	Q_DISABLE_COPY(ChannelSource)
};

typedef QSharedPointer<ChannelSource> ChannelSourcePtr;
typedef QList<ChannelSourcePtr> ChannelSourceList;

class ChannelSourceModel : public QAbstractListModel
{
	Q_OBJECT

public:
	enum ChannelSourceRoles
	{
		Name = Qt::UserRole + 1,
		Id,
		Logo,
		Url
	};

	ChannelSourceModel();
	~ChannelSourceModel();

	// Don't use in prod. It isn't thread-safe
	const ChannelSourceList& Items() const;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;

private:
	void Add(ChannelSourcePtr contact, bool notifiChanged = false);
	void Load(const QString& filePath);
	//[
	//{"id": "1", "name": "PSYCHEDELIK", "logo": "", "url": "http://88.191.104.69:8002/"},
	//{"Id": "1", "name": "PSYCHEDELIK2", "logo": "", "url": "http://88.191.104.69:8002/"}
	//]
	void Parse(const QByteArray& json);
	void Cleanup();

private:
	friend class TabPagesController;

	ChannelSourceList			m_channels;
	mutable QReadWriteLock		m_lock;

	Q_DISABLE_COPY(ChannelSourceModel)
};

typedef ChannelSourceModel * ChannelSourceModelPtr;
//Q_DECLARE_METATYPE(ChannelSourceModelPtr);
//Q_DECLARE_METATYPE(ChannelSourcePtr);

}

#endif
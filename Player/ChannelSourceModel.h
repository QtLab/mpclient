#ifndef MP_CHANNEL_SOURCE_H
#define MP_CHANNEL_SOURCE_H

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

class ChannelSourceModel : public BaseListModel<ChannelSource>
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
	QHash<int, QByteArray>	roleNames() const;

public:
	friend class TabPagesController;
	friend class RadioCompositeModel;
	friend class RadioPageController;
	friend QAbstractListModel;

	Q_DISABLE_COPY(ChannelSourceModel)
};

}

Q_DECLARE_METATYPE(mp::ChannelSource);
Q_DECLARE_METATYPE(mp::ChannelSourceModel);

#endif
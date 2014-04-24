#ifndef MP_CHANNEL_SOURCE_H
#define MP_CHANNEL_SOURCE_H

#include "Prerequirements.h"
#include "BaseListModel.h"

namespace mp {

class ChannelSource : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString name READ Name WRITE SetName)
	Q_PROPERTY(int id READ Id WRITE SetId)
	Q_PROPERTY(QString url READ Url WRITE SetUrl)
	Q_PROPERTY(int genreid READ GenreId WRITE SetGenreId)
	Q_PROPERTY(uint playcount READ PlayCount WRITE SetPlayCount)
	Q_PROPERTY(uint lastplay READ LastPlayTimestamp WRITE SetLastPlayTimestamp)

public:
	ChannelSource();
	~ChannelSource();

	int Id() const;
	void SetId(int id);

	const QString& Name() const;
	void SetName(const QString& name);

	// Url to channel
	const QString& Url() const;
	void SetUrl(const QString& url);

	int GenreId() const;
	void SetGenreId(int genreId);

	uint PlayCount() const;
	void SetPlayCount(uint count);

	uint LastPlayTimestamp() const;
	void SetLastPlayTimestamp(uint ts);
	void SetLastPlayNow();

public:
	int						m_id;
	QString					m_name;
	QString					m_url;
	int						m_genreId;
	int						m_playCount;
	// Unix timestamp
	uint					m_lastPlayTimestamp;

	Q_DISABLE_COPY(ChannelSource)
};

class ChannelSourceModel : public BaseListModel<ChannelSource>
{
	Q_OBJECT

public:
	enum ChannelSourceRoles
	{
		Name = Qt::UserRole + 1,
		Id,
		Url,
		GenreId
	};

	ChannelSourceModel();
	virtual ~ChannelSourceModel();

	void LoadWithStats(const QString& filePath);
	bool SaveStats(const QString& filePath);

	ChannelSourceList Items() const;
	ChannelSourcePtr Find(int channelId, int genreId = -1) const;
	// Merge with items that contains additional information about use it by current user
	void MergeWithStats(const ChannelSourceModel& channelsWithStats);

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QHash<int, QByteArray>	roleNames() const;

public:
	friend QAbstractListModel;
	Q_DISABLE_COPY(ChannelSourceModel)
};

}

Q_DECLARE_METATYPE(mp::ChannelSource);
Q_DECLARE_METATYPE(mp::ChannelSourceModel);

#endif
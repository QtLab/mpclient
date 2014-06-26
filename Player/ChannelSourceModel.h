#ifndef MP_CHANNEL_SOURCE_H
#define MP_CHANNEL_SOURCE_H

#include "Prerequirements.h"
#include "BaseListModel.h"

namespace mp {
namespace model {

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
	virtual ~ChannelSource();

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

	void AddCategoryId(uint id);
	CategoryIds Categories() const;

public:
	int						m_id;
	QString					m_name;
	QString					m_url;
	int						m_genreId;
	int						m_playCount;
	// Unix timestamp
	uint					m_lastPlayTimestamp;
	CategoryIds				m_categories;

	Q_DISABLE_COPY(ChannelSource)
};

class ChannelSourceModel : public BaseListModel<ChannelSource>
{
	Q_OBJECT

public:
	enum ChannelSourceRoles
	{
		Id = Qt::UserRole + 1,
		Name,
		Url,
		FirstCategoryId,
		Categories,
		GenreId,
		LastPlayTimestamp,
		PlayCount
	};

	ChannelSourceModel();
	virtual ~ChannelSourceModel();

	void LoadWithStats(const QString& filePath);
	bool SaveStats(const QString& filePath);

	ChannelSourcePtr Find(int channelId, int genreId = -1) const;

	// Merge with items that contains additional information about use it by current user
	void MergeWithStats(const ChannelSourceModel& channelsWithStats);

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QHash<int, QByteArray>	roleNames() const;

private:
	void ParseChannelsJson(const QByteArray& json);

public:
	friend QAbstractListModel;
	Q_DISABLE_COPY(ChannelSourceModel)
};

} //End namespace model
} //End namespace mp

Q_DECLARE_METATYPE(mp::model::ChannelSource);
Q_DECLARE_METATYPE(mp::model::ChannelSourceModel);

#endif
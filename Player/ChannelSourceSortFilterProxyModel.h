#ifndef MP_CHANNEL_SOURCE_SORT_FILTER_PROXY_SOURCE_H
#define MP_CHANNEL_SOURCE_SORT_FILTER_PROXY_SOURCE_H

#include <QSortFilterProxyModel>

namespace mp {

class ChannelSourceSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:

	enum SortType
	{
		ByName,
		ByTop,
		ByLastPlayTime
	};

    ChannelSourceSortFilterProxyModel(QObject *parent = 0);

	int GenreId() const;
	void SetGenreIdFilter(int genreId);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

public:
	friend QAbstractListModel;
	Q_DISABLE_COPY(ChannelSourceSortFilterProxyModel)

private:
	int				m_genreId;
	SortType		m_sortDirection;
};

}

Q_DECLARE_METATYPE(mp::ChannelSourceSortFilterProxyModel);

#endif
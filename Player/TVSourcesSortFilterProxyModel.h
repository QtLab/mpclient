#ifndef MP_TV_SOURCES_SORT_FILTER_PROXY_SOURCE_H
#define MP_TV_SOURCES_SORT_FILTER_PROXY_SOURCE_H

#include <QSortFilterProxyModel>
#include "Prerequirements.h"

namespace mp {
namespace model {

class TVSourcesSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    TVSourcesSortFilterProxyModel(QObject *parent = 0);

	const TVSourceIdsSet& TvSourceIdsFilter() const;
	void SetTvSourceIdsFilter(const TVSourceIdsSet& filter);

	int GenreIdFilter() const;
	void SetGenreIdFilter(int genreId);

	const QString& NameFilter() const;
	void SetNameFilter(const QString& nameFilter);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

public:
	friend QAbstractListModel;
	Q_DISABLE_COPY(TVSourcesSortFilterProxyModel)

private:
	int					m_genreIdFilter;
	QString				m_nameFilter;
	TVSourceIdsSet		m_tvSourceIdsFilter;
};

} //namespace model
} //namespace mp

Q_DECLARE_METATYPE(mp::model::TVSourcesSortFilterProxyModel);

#endif
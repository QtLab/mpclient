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

	int CategoryIdFilter() const;
	void SetCategoryIdFilter(int categoryId);

	int GenreIdFilter() const;
	void SetGenreIdFilter(int genreId);

	const QString& NameFilter() const;
	void SetNameFilter(const QString& nameFilter);

	const TVSourceIdsSet& TvSourceIdsFilter() const;
	void SetTvSourceIdsFilter(const TVSourceIdsSet& filter);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

public:
	friend QAbstractListModel;
	Q_DISABLE_COPY(TVSourcesSortFilterProxyModel)

private:
	int					m_categoryIdFilter;
	int					m_genreIdFilter;
	QString				m_nameFilter;
	TVSourceIdsSet		m_tvSourceIdsFilter;
};

} //namespace model
} //namespace mp

#ifdef BUILD_WITH_METATYPES
Q_DECLARE_METATYPE(mp::model::TVSourcesSortFilterProxyModel);
#endif

#endif
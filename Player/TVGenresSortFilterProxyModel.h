#ifndef MP_TV_GENRES_SORT_FILTER_PROXY_SOURCE_H
#define MP_TV_GENRES_SORT_FILTER_PROXY_SOURCE_H

#include <QSortFilterProxyModel>

namespace mp {
namespace model {

class TVGenresSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    TVGenresSortFilterProxyModel(QObject *parent = 0);

	int CategoryIdFilter() const;
	void SetCategoryIdFilter(int categoryId);

	const QString& NameFilter() const;
	void SetNameFilter(const QString& nameFilter);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

public:
	friend QAbstractListModel;
	Q_DISABLE_COPY(TVGenresSortFilterProxyModel)

private:
	int				m_categoryIdFilter;
	QString			m_nameFilter;
};

} //namespace model
} //namespace mp

#ifdef BUILD_WITH_METATYPES
Q_DECLARE_METATYPE(mp::model::TVGenresSortFilterProxyModel);
#endif

#endif
#include "TVSourcesSortFilterProxyModel.h"
#include "TVSourcesModel.h"

namespace mp {
namespace model {

TVSourcesSortFilterProxyModel::TVSourcesSortFilterProxyModel(QObject *parent)
	: QSortFilterProxyModel(parent)
	,m_genreIdFilter(-1)
{
	setDynamicSortFilter(true);
}

const TVSourceIdsSet& TVSourcesSortFilterProxyModel::TvSourceIdsFilter() const
{
	return m_tvSourceIdsFilter;
}

void TVSourcesSortFilterProxyModel::SetTvSourceIdsFilter(const TVSourceIdsSet& filter)
{
	m_tvSourceIdsFilter = filter;
	invalidate();
}

int TVSourcesSortFilterProxyModel::GenreIdFilter() const
{
	return m_genreIdFilter;
}

void TVSourcesSortFilterProxyModel::SetGenreIdFilter(int genreId)
{
	m_genreIdFilter = genreId;
	invalidate();
}

const QString& TVSourcesSortFilterProxyModel::NameFilter() const
{
	return m_nameFilter;
}

void TVSourcesSortFilterProxyModel::SetNameFilter(const QString& nameFilter)
{
	if(m_nameFilter != nameFilter)
	{
		m_nameFilter = nameFilter;
		invalidateFilter();
	}
}

bool TVSourcesSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
	if(m_genreIdFilter < 0 && m_nameFilter.isEmpty() && m_tvSourceIdsFilter.count() <= 0)
	{
		return true;
	}

	QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

	if(!m_tvSourceIdsFilter.empty())
	{
		int id = index.data(TVSourcesModel::Id).toInt();
		if(!m_tvSourceIdsFilter.contains(id))
		{
			return false;
		}
	}

	if(!m_nameFilter.isEmpty())
	{
		QString channelName = sourceModel()->data(index, TVSourcesModel::Name).toString();

		if(!channelName.contains(m_nameFilter, Qt::CaseInsensitive))
			return false;
	}

	if(m_genreIdFilter >= 0)
	{
		TVGenreIdsSet genreIds = sourceModel()->data(index, TVSourcesModel::GenreIds).value<TVGenreIdsSet>();

		if(!genreIds.contains(m_genreIdFilter))
		{
			return false;
		}
	}

   return true;
}

bool TVSourcesSortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	QString nameLeft = left.data(TVSourcesModel::Name).toString();
	QString nameRight = right.data(TVSourcesModel::Name).toString();

	return nameLeft > nameRight;
}

} //namespace model
} //namespace mp

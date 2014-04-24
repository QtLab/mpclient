#include "ChannelSourceSortFilterProxyModel.h"
#include "ChannelSourceModel.h"

namespace mp {

ChannelSourceSortFilterProxyModel::ChannelSourceSortFilterProxyModel(QObject *parent)
	: QSortFilterProxyModel(parent)
	,m_genreId(-1)
	,m_sortDirection(ByName)
{
}

int ChannelSourceSortFilterProxyModel::GenreId() const
{
	return m_genreId;
}

void ChannelSourceSortFilterProxyModel::SetGenreIdFilter(int genreId)
{
	if(m_genreId != genreId)
	{
		m_genreId = genreId;
		invalidateFilter();
	}
}

bool ChannelSourceSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
	if(m_genreId < 0)
	{
		return true;
	}

	QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
	
	int genreId = sourceModel()->data(index, ChannelSourceModel::GenreId).toInt();

	if(genreId != m_genreId)
	{
		return false;
	}

   return true;
}

bool ChannelSourceSortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	QString nameLeft = left.data(ChannelSourceModel::Name).toString();
	QString nameRight = right.data(ChannelSourceModel::Name).toString();

	return nameLeft > nameRight;
}

}
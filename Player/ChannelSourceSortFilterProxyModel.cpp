#include "ChannelSourceSortFilterProxyModel.h"
#include "ChannelSourceModel.h"

namespace mp {
namespace model {

ChannelSourceSortFilterProxyModel::ChannelSourceSortFilterProxyModel(QObject *parent)
	: QSortFilterProxyModel(parent)
	,m_categoryIdFilter(-1)
	,m_sortBy(ByName)
{
	setDynamicSortFilter(true);
}

int ChannelSourceSortFilterProxyModel::CategoryIdFilter() const
{
	return m_categoryIdFilter;
}

void ChannelSourceSortFilterProxyModel::SetCategoryIdFilter(int categoryId)
{
	m_categoryIdFilter = categoryId;
}

const QString& ChannelSourceSortFilterProxyModel::NameFilter() const
{
	return m_nameFilter;
}

void ChannelSourceSortFilterProxyModel::SetNameFilter(const QString& nameFilter)
{
	if(m_nameFilter != nameFilter)
	{
		m_nameFilter = nameFilter;
		invalidateFilter();
	}
}

ChannelSourceSortFilterProxyModel::SortT ChannelSourceSortFilterProxyModel::SortType() const
{
	return m_sortBy;
}

void ChannelSourceSortFilterProxyModel::SetSortType(SortT sortBy )
{
	m_sortBy = sortBy;
	invalidateFilter();
}

bool ChannelSourceSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
	if(m_categoryIdFilter < 0 && m_nameFilter.isEmpty())
	{
		return true;
	}

	QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

	if(!m_nameFilter.isEmpty())
	{
		QString channelName = sourceModel()->data(index, ChannelSourceModel::Name).toString();

		if(!channelName.contains(m_nameFilter, Qt::CaseInsensitive))
			return false;
	}

	if(m_categoryIdFilter >= 0)
	{
		RadioCategoryIds ids = sourceModel()->data(index, ChannelSourceModel::Categories).value<RadioCategoryIds>();

		if(!ids.contains(m_categoryIdFilter))
		{
			return false;
		}
	}

   return true;
}

bool ChannelSourceSortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	switch(m_sortBy)
	{
		case ByTop:
		{
			uint playCountLeft = left.data(ChannelSourceModel::PlayCount).toUInt();
			uint playCountRight = right.data(ChannelSourceModel::PlayCount).toUInt();
				
			return playCountLeft > playCountRight;
		}
		break;
		case ByLastPlayTime:
		{
			uint lastPlayTSLeft = left.data(ChannelSourceModel::LastPlayTimestamp).toUInt();
			uint lastPlayTSRight = right.data(ChannelSourceModel::LastPlayTimestamp).toUInt();
				
			return lastPlayTSLeft > lastPlayTSRight;
		}
		break;
		default:
		case ByName:
		{
			QString nameLeft = left.data(ChannelSourceModel::Name).toString();
			QString nameRight = right.data(ChannelSourceModel::Name).toString();

			return nameLeft > nameRight;
		}
		break;
	};
}

} //End namespace model
} //End namespace mp
#include "RadioSourcesSortFilterProxyModel.h"
#include "RadioSourcesModel.h"

namespace mp {
namespace model {

RadioSourcesSortFilterProxyModel::RadioSourcesSortFilterProxyModel(QObject *parent)
	: QSortFilterProxyModel(parent)
	,m_categoryIdFilter(-1)
	,m_sortBy(ByName)
{
	setDynamicSortFilter(true);
}

int RadioSourcesSortFilterProxyModel::CategoryIdFilter() const
{
	return m_categoryIdFilter;
}

void RadioSourcesSortFilterProxyModel::SetCategoryIdFilter(int categoryId)
{
	m_categoryIdFilter = categoryId;
}

const QString& RadioSourcesSortFilterProxyModel::NameFilter() const
{
	return m_nameFilter;
}

void RadioSourcesSortFilterProxyModel::SetNameFilter(const QString& nameFilter)
{
	if(m_nameFilter != nameFilter)
	{
		m_nameFilter = nameFilter;
		invalidateFilter();
	}
}

RadioSourcesSortFilterProxyModel::SortT RadioSourcesSortFilterProxyModel::SortType() const
{
	return m_sortBy;
}

void RadioSourcesSortFilterProxyModel::SetSortType(SortT sortBy )
{
	m_sortBy = sortBy;
	invalidateFilter();
}

bool RadioSourcesSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
	if(m_categoryIdFilter < 0 && m_nameFilter.isEmpty())
	{
		return true;
	}

	QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

	if(!m_nameFilter.isEmpty())
	{
		QString channelName = sourceModel()->data(index, RadioSourcesModel::Name).toString();

		if(!channelName.contains(m_nameFilter, Qt::CaseInsensitive))
			return false;
	}

	if(m_categoryIdFilter >= 0)
	{
		RadioCategoryIds ids = sourceModel()->data(index, RadioSourcesModel::Categories).value<RadioCategoryIds>();

		if(!ids.contains(m_categoryIdFilter))
		{
			return false;
		}
	}

   return true;
}

bool RadioSourcesSortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	switch(m_sortBy)
	{
		case ByTop:
		{
			uint playCountLeft = left.data(RadioSourcesModel::PlayCount).toUInt();
			uint playCountRight = right.data(RadioSourcesModel::PlayCount).toUInt();
				
			return playCountLeft > playCountRight;
		}
		break;
		case ByLastPlayTime:
		{
			uint lastPlayTSLeft = left.data(RadioSourcesModel::LastPlayTimestamp).toUInt();
			uint lastPlayTSRight = right.data(RadioSourcesModel::LastPlayTimestamp).toUInt();
				
			return lastPlayTSLeft > lastPlayTSRight;
		}
		break;
		default:
		case ByName:
		{
			QString nameLeft = left.data(RadioSourcesModel::Name).toString();
			QString nameRight = right.data(RadioSourcesModel::Name).toString();

			return nameLeft > nameRight;
		}
		break;
	};
}

} //namespace model
} //namespace mp

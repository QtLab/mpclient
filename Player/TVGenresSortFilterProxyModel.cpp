#include "TVGenresSortFilterProxyModel.h"
#include "TVGenresModel.h"

namespace mp {
namespace model {

TVGenresSortFilterProxyModel::TVGenresSortFilterProxyModel(QObject *parent)
	: QSortFilterProxyModel(parent)
	,m_categoryIdFilter(-1)
{
	setDynamicSortFilter(true);
}

int TVGenresSortFilterProxyModel::CategoryIdFilter() const
{
	return m_categoryIdFilter;
}

void TVGenresSortFilterProxyModel::SetCategoryIdFilter(int categoryId)
{
	m_categoryIdFilter = categoryId;
	invalidateFilter();
}

const QString& TVGenresSortFilterProxyModel::NameFilter() const
{
	return m_nameFilter;
}

void TVGenresSortFilterProxyModel::SetNameFilter(const QString& nameFilter)
{
	if(m_nameFilter != nameFilter)
	{
		m_nameFilter = nameFilter;
		invalidateFilter();
	}
}

bool TVGenresSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
	if(m_categoryIdFilter < 0 && m_nameFilter.isEmpty())
	{
		return true;
	}

	QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

	if(!m_nameFilter.isEmpty())
	{
		QString channelName = sourceModel()->data(index, TVGenresModel::Name).toString();

		if(!channelName.contains(m_nameFilter, Qt::CaseInsensitive))
			return false;
	}

	if(m_categoryIdFilter >= 0)
	{
		int categoryId = sourceModel()->data(index, TVGenresModel::CategoryId).value<int>();

		if(categoryId != m_categoryIdFilter)
		{
			return false;
		}
	}

   return true;
}

bool TVGenresSortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	QString nameLeft = left.data(TVGenresModel::Name).toString();
	QString nameRight = right.data(TVGenresModel::Name).toString();

	return nameLeft > nameRight;
}

} //namespace model
} //namespace mp

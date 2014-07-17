#include "RadioCategoriesModel.h"

#include <QHash>
#include <QDebug>
#include <QJsonDocument>
#include <QMetaProperty>
#include <QFont>
#include <QFontMetrics>

namespace mp {
namespace model {

RadioCategory::RadioCategory()
	:m_topVisible(false)
{
}

RadioCategory::~RadioCategory()
{
}

int RadioCategory::Id() const
{
	return m_id;
}

void RadioCategory::SetId(int id)
{
	m_id = id;
}

QString RadioCategory::Name() const
{ 
	return m_name; 
}

void RadioCategory::SetName(const QString& name)
{ 
	m_name = name; 
}

bool RadioCategory::TopVisible() const
{
	return m_topVisible;
}

void RadioCategory::SetTopVisible(bool topVisible)
{
	m_topVisible = topVisible;
}

RadioCategoriesModel::RadioCategoriesModel()
	:m_lastTopVIsibleIndex(0)
{
}

RadioCategoriesModel::~RadioCategoriesModel() 
{
}

int RadioCategoriesModel::UpdateTopVisibleCategories(int maxWidth, const QString& fontFamily, int pointSize, int spacing)
{
	if(maxWidth < 0)
	{
		return 0;
	}

	int actualVListWidh = 0;
	int lastTopVIsibleIndex = -1;

	QFont font(fontFamily, pointSize);
	QFontMetrics fm(font);

	int index = 0;

	bool allNextCategoriesInvisible = false;

	foreach(RadioCategoryPtr category, m_items)
	{
		if(allNextCategoriesInvisible)
		{
			category->SetTopVisible(false);
		}
		else
		{
			int textWidth = fm.width(category->Name());

			if((actualVListWidh + (textWidth + spacing)) <= maxWidth)
			{
				actualVListWidh += (textWidth + spacing);
				category->SetTopVisible(true);
				lastTopVIsibleIndex = index;
			}
			else
			{
				allNextCategoriesInvisible = true;
				category->SetTopVisible(false);
			}
		}

		index++;
	}

	if(m_lastTopVIsibleIndex != lastTopVIsibleIndex)
	{
		m_lastTopVIsibleIndex = lastTopVIsibleIndex;
		EmitDataChanged();
	}

	return actualVListWidh;
}

void RadioCategoriesModel::InsertFirstTopVisibleCategory(int id, int maxWidth, const QString& fontFamily, int pointSize, int spacing)
{
	for(int i = 0; i < m_items.count(); i++ )
	{
		RadioCategoryPtr category = m_items[i];

		if(category->Id() == id)
		{
			if(!category->TopVisible())
			{
				m_items.move(i, 0);
			}
			
			break;
		}
	}

	UpdateTopVisibleCategories(maxWidth, fontFamily, pointSize, spacing);
}

void RadioCategoriesModel::InsertLastTopVisibleCategory(int id, int maxWidth, const QString& fontFamily, int pointSize, int spacing)
{
	RadioCategoryPtr insertedCategory;

	for(int i = 0; i < m_items.count(); i++ )
	{
		RadioCategoryPtr category = m_items[i];

		if(category->Id() == id)
		{
			if(category->TopVisible())
			{
				return;
			}

			insertedCategory = category;
			m_items.removeAt(i);
			break;
		}
	}

	if(insertedCategory)
	{
		int actualVListWidh = UpdateTopVisibleCategories(maxWidth, fontFamily, pointSize, spacing);

		QFont font(fontFamily, pointSize);
		QFontMetrics fm(font);

		int insertedCategoryTextWidth = fm.width(insertedCategory->Name());

		for(int i = m_lastTopVIsibleIndex; i >=0; i--)
		{
			if((actualVListWidh + (insertedCategoryTextWidth + spacing)) <= maxWidth)
			{
				insertedCategory->SetTopVisible(true);
				actualVListWidh += (insertedCategoryTextWidth + spacing);
				m_items.insert(i + 1, insertedCategory);
				break;
			}
			else
			{
				RadioCategoryPtr category = m_items[i];
				category->SetTopVisible(false);
				int textWidth = fm.width(category->Name());
				actualVListWidh -= (textWidth + spacing);
			}
		}

		EmitDataChanged();
	}
}

int RadioCategoriesModel::RowIndexById(int id) const
{
	int row = 0;
	foreach(RadioCategoryPtr category, m_items)
	{
		if(category->Id() == id)
		{
			int rowIndex = index(row).row();
			return rowIndex;
		}

		row++;
	}
	
	return -1;
}

RadioCategoryPtr RadioCategoriesModel::FindById(int id) const
{
	foreach(RadioCategoryPtr category, m_items)
	{
		if(category->Id() == id)
		{
			return category;
		}
	}

	return RadioCategoryPtr();
}

QVariant RadioCategoriesModel::data(const QModelIndex & index, int role) const 
{
	if (index.row() < 0 || index.row() > m_items.count())
		return QVariant();

	const RadioCategoryPtr contact = m_items.at(index.row());
	
	QVariant result;

	switch (role) 
	{
		case Name:
			result = QVariant(contact->Name());
			break;
		case Id:
			result = QVariant(contact->Id());
			break;
		case TopVisible:
			result = QVariant(contact->TopVisible());
			break;
	}

	return result;
}

int RadioCategoriesModel::rowCount(const QModelIndex &parent) const
{
	return m_items.count();
}

QHash<int, QByteArray>	RadioCategoriesModel::roleNames() const
{
	QHash<int, QByteArray> roles;
	roles[Name] = "Name";
	roles[Id] = "Id";
	roles[TopVisible] = "TopVisible";

	return roles;
}

void RadioCategoriesModel::Cleanup()
{
	BaseListModel<RadioCategory>::Cleanup();
	m_lastTopVIsibleIndex = 0;
}

} //namespace model
} //namespace mp

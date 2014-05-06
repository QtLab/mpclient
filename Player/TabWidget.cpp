#include "TabWidget.h"
#include "TabPage.h"

#include <QEvent>

namespace mp {

TabWidget::TabWidget(QWidget * parent, const QString& name, const QString& tabBarName)
	:QTabWidget(parent)
	,m_lastCurrentTabIndex(-1)
{
	setObjectName(name);

	tabBar()->setObjectName(tabBarName);
	tabBar()->setCursor(Qt::PointingHandCursor);

	connect(this, SIGNAL(currentChanged(int)), SLOT(CurretntTabIndexChanged(int)));
	setUsesScrollButtons(false);
}

int TabWidget::AddPage(TabPage * page)
{
	int index = addTab(page, page->Name());
	m_pages.insert(index, page);

	return index;
}

TabPage * TabWidget::PageAtIndex(int index)
{
	TabPage * page = qobject_cast<TabPage*>(widget(index));
	return page;
}

void TabWidget::RetranslateUI()
{
	TabPages::const_iterator i = m_pages.constBegin();
	while (i != m_pages.constEnd()) 
	{
		setTabText(i.key(), i.value()->Name());
		++i;
	}
}

void TabWidget::changeEvent(QEvent *event)
{
	if (event->type() == QEvent::LanguageChange) 
	{
		RetranslateUI();
	}

	QWidget::changeEvent(event);
}

void TabWidget::CurretntTabIndexChanged(int newIdnex)
{
	if(m_lastCurrentTabIndex >= 0)
	{
		TabPage * page = PageAtIndex(m_lastCurrentTabIndex);

		if(page)
			page->Leave();
	}

	TabPage * page = PageAtIndex(newIdnex);

	if(page)
		page->Enter();

	m_lastCurrentTabIndex = newIdnex;
}

}
#include "TabWidget.h"
#include "TabPage.h"

#include <QEvent>

namespace mp {
namespace view {

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

	page->SetTabIndex(index);

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
		int index = i.key();
		TabPagePtr tabPage = i.value();
		setTabText(index, tabPage->Name());
		setTabToolTip(index, tabPage->ToolTip());
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
	TabPage * oldPage = NULL;

	if(m_lastCurrentTabIndex >= 0)
	{
		 oldPage = PageAtIndex(m_lastCurrentTabIndex);

		if(oldPage)
			oldPage->Leave();
	}

	TabPage * newPage = PageAtIndex(newIdnex);

	if(newPage)
		newPage->Enter();

	emit CurrentPageChanged(newPage, oldPage);

	m_lastCurrentTabIndex = newIdnex;
}

} // end namespace view
} // end namespace mp
#include "TabWidget.h"
#include "TabPage.h"

namespace mp {

TabWidget::TabWidget(QWidget * parent, const QString& name, const QString& tabBarName)
	:QTabWidget(parent)
{
	setObjectName(name);

	tabBar()->setObjectName(tabBarName);
	tabBar()->setCursor(Qt::PointingHandCursor);

	setUsesScrollButtons(false);
}

int TabWidget::AddPage(TabPage * page, const QString& name)
{
	int index = addTab(page, name);
	return index;
}

void TabWidget::SetTvPage(TabPage * page)
{
	m_tvPage = page;
	m_tvPageIndex = addTab(m_tvPage, tr("TV"));
}

void TabWidget::SetRadioPage(TabPage * page)
{
	m_radioPage = page;
	m_radioPageIndex = addTab(m_radioPage, tr("RADIO"));
}

}
#include "TabWidget.h"
#include "TabPage.h"

namespace mp {

TabBar::TabBar(QWidget * parent)
		:QTabBar(parent)
{
}

QSize TabBar::tabSizeHint(int indx) const
{
	QSize defaultSize = QTabBar::tabSizeHint(indx);
	return defaultSize;
}

void TabBar::setFont( const QFont& font )
{

	QFontInfo fontInfo(font);
	if(fontInfo.fixedPitch())
		QTabBar::setFont(font);

	// Otherwise ignore..
}

TabWidget::TabWidget(QWidget * parent, const QString& name, const QString& tabBarName, bool useCustomTabBar)
	:QTabWidget(parent)
{
	setObjectName(name);

	if(useCustomTabBar)
	{
		mp::TabBar * tabBar = new mp::TabBar(this);
		setTabBar(tabBar);
	}

	tabBar()->setObjectName(tabBarName);
	setUsesScrollButtons(false);
}

QTabBar* TabWidget::TabBar()
{
	return tabBar();
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
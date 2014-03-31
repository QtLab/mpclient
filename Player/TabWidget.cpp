#include "TabWidget.h"
#include "TabPage.h"

#include <QEvent>

namespace mp {

TabWidget::TabWidget(QWidget * parent, const QString& name, const QString& tabBarName)
	:QTabWidget(parent)
{
	setObjectName(name);

	tabBar()->setObjectName(tabBarName);
	tabBar()->setCursor(Qt::PointingHandCursor);

	setUsesScrollButtons(false);
}

int TabWidget::AddPage(TabPage * page)
{
	int index = addTab(page, page->Name());
	m_pages.insert(index, page);

	return index;
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

}
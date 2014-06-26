#include "TabPage.h"

#include <QDebug>
#include <QEvent>

namespace mp {
namespace view {

TabPage::TabPage(QWidget* parent)
	:QWidget(parent)
{
}

TabPage::~TabPage()
{
}

void TabPage::Enter()
{
}

bool TabPage::Leave()
{
	return true;
}

void TabPage::RetranslateUI()
{
}

void TabPage::changeEvent(QEvent * evt)
{
	if (evt->type() == QEvent::LanguageChange) 
	{
		RetranslateUI();
	}
	else 
	{
		QWidget::changeEvent(evt);
	}
}

QMargins TabPage::Margins()
{
	return QMargins(1, 0, 1, 0);
}

int TabPage::TabIndex() const
{
	return m_tabIndex;
}

void TabPage::SetTabIndex(int index)
{
	m_tabIndex = index;
}

}
}
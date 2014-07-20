#include "TabPage.h"

#include <QDebug>
#include <QEvent>

namespace mp {
namespace view {

TabPage::TabPage(QWidget* parent)
	:QWidget(parent)
	,m_isActived(false)
{
}

TabPage::~TabPage()
{
}

bool TabPage::IsActived() const
{
	return m_isActived;
}

void TabPage::Enter()
{
	m_isActived = true;
}

bool TabPage::Leave()
{
	m_isActived = false;
	return true;
}

void TabPage::RetranslateUI()
{
}

QString TabPage::ToolTip() const
{
	return "asdasd";
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

int TabPage::TabIndex() const
{
	return m_tabIndex;
}

void TabPage::SetTabIndex(int index)
{
	m_tabIndex = index;
}

} // end namespace view
} // end namespace mp
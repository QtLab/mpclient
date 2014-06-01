#include "SystemTrayContextMenu.h"
#include "WidgetUtils.h"

namespace mp {

SystemTrayContextMenu::SystemTrayContextMenu(QWidget * parent)
	:QMenu(parent)
{
	//WidgetUtils::LoadStyleSheets(this, "ZaxarMenu.qss");
	Retranslate();
}

void SystemTrayContextMenu::changeEvent(QEvent *event)
{
	if (event->type() == QEvent::LanguageChange) 
	{
		Retranslate();
	}
	else
	{
		QMenu::changeEvent(event);
	}
}

void SystemTrayContextMenu::Retranslate()
{
	clear();
	addSeparator();
	addAction(tr("Update"), this, SIGNAL(UpdateApplication()));
	addSeparator();
	addAction(tr("About"), this, SIGNAL(About()));
	addSeparator();
	addAction(tr("Close"), this, SIGNAL(CloseApplication()));
}

}
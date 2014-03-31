#include "SystemTray.h"
#include "SystemTrayContextMenu.h"

#include <QMainWindow>
#include <QPainter>
#include <QApplication>

namespace mp {

SystemTray::SystemTray(QMainWindow * parent)
	:QSystemTrayIcon(parent)
	,m_parent(parent)
{
	setIcon(QIcon(":/mp/Resources/Player.ico"));

	SystemTrayContextMenu * menu = new SystemTrayContextMenu(m_parent);
	connect(menu, SIGNAL(CloseApplication()), this, SIGNAL(ShowtdownApplicationReuest()));
	setContextMenu(menu);

	connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this, SLOT(Activated(QSystemTrayIcon::ActivationReason)));
}

void SystemTray::Activated(QSystemTrayIcon::ActivationReason reason)
{
	if(reason == QSystemTrayIcon::Trigger)
	{
		bool minimized = m_parent->isMinimized();
		bool show = false;

		if(m_parent->isHidden() || minimized)
		{
			if(m_parent->isMaximized())
			{
				m_parent->showMaximized();
			}
			else
			{
				m_parent->showNormal();
			}

			m_parent->activateWindow();
		}
		else
		{
			m_parent->hide();
		}
	}
}

}
#include "SystemTray.h"
#include "SystemTrayContextMenu.h"
#include "MessageBoxView.h"
#include "StateTooltip.h"

#include <QMainWindow>
#include <QPainter>
#include <QApplication>

namespace mp {
namespace view {

SystemTray::SystemTray(QMainWindow * parent)
	:QSystemTrayIcon(parent)
	,m_parent(parent)
{
	setIcon(QIcon(":/mp/Resources/Player.ico"));

	setToolTip(QString("UnisonBox %0").arg(PLAYER_VERSION));

	SystemTrayContextMenu * menu = new SystemTrayContextMenu(m_parent);
	connect(menu, SIGNAL(CloseApplication()), this, SIGNAL(ShowtdownApplicationReuest()));
	connect(menu, SIGNAL(UpdateApplication()), this, SIGNAL(UpdateReuest()));
	connect(menu, SIGNAL(AboutApplication()), this, SLOT(ShowAbout()));

	setContextMenu(menu);

	installEventFilter(this);
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

void SystemTray::ShowAbout()
{
	MessageBoxView::ShowAbout(m_parent);
}

bool SystemTray::eventFilter(QObject *object, QEvent *event)
{
	if (event->type() != QEvent::ToolTip)
	{
		(new StateTooltip(NULL))->Show(QPoint());
	}

	return QSystemTrayIcon::eventFilter(object, event);
}

bool SystemTray::event(QEvent * e)
{
	if (e->type() != QEvent::ToolTip)
	{
		(new StateTooltip(NULL))->Show(QPoint());
	}
	else
	{
		return QSystemTrayIcon::event(e);
	}
}

} // end namespace view
} // end namespace mp
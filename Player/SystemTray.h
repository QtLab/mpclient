#ifndef MP_SYSTEM_TRAY_H
#define MP_SYSTEM_TRAY_H

#include <QSystemTrayIcon>

class QMainWindow;

namespace mp {

class SystemTray : public QSystemTrayIcon
{
	Q_OBJECT

public:
	SystemTray(QMainWindow * parent);

private slots:
	void Activated(QSystemTrayIcon::ActivationReason reason);

signals:
	void ShowtdownApplicationReuest();

private:
	QMainWindow *		m_parent;
};

}

#endif // MP_SYSTEM_TRAY_H

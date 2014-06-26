#ifndef MP_SYSTEM_TRAY_H
#define MP_SYSTEM_TRAY_H

#include <QSystemTrayIcon>

class QMainWindow;

namespace mp {
namespace view {

class SystemTray : public QSystemTrayIcon
{
	Q_OBJECT

public:
	SystemTray(QMainWindow * parent);

private slots:
	void Activated(QSystemTrayIcon::ActivationReason reason);

signals:
	void ShowtdownApplicationReuest();
	void UpdateReuest();

private:
	QMainWindow *		m_parent;
};

}
}

#endif // MP_SYSTEM_TRAY_H

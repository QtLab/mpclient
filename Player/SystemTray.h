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
	void ShowAbout();

public:
	bool eventFilter(QObject *object, QEvent *event);
	virtual bool event(QEvent * e);
	
signals:
	void ShowtdownApplicationReuest();
	void UpdateReuest();

private:
	QMainWindow *		m_parent;
};

} // end namespace view
} // end namespace mp

#endif // MP_SYSTEM_TRAY_H

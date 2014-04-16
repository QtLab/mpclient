#ifndef MP_APP_CONTROLLER_H
#define MP_APP_CONTROLLER_H

#include "Prerequirements.h"
#include <QApplication>

namespace mp {

class AppController : public QApplication
{
	Q_OBJECT

public:
	AppController(int argc, char *argv[]);
	~AppController();

	void CreateView();
	void InitSignalsSlots();

	static AppController& Inst();
	
private:
	bool notify(QObject* receiver, QEvent* even);

private slots:
	void Showtdown(); 
	void UpdateFinished(bool restartRequired);
	void UserIdleStateChanged(bool isIdle);

private:
	MainWindow *				m_mainWidow;
	SystemTray *				m_trayIcon;

	RadioPageController *		m_radioPageController;
	TVPageController *			m_tvPageController;
	UpdateController *			m_updateController;

	UserIdle *					m_userIdle;
};

}

#endif
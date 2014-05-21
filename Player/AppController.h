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

	void SetLang(const QString& lang);
	void CreateView();
	void InitSignalsSlots();
	
	static AppController& Inst();

private slots:
	void Showtdown(int exitCode = 0);
	void UpdateStarted();
	void UpdateFinished(bool restartRequired);
	void UserIdleStateChanged(bool isIdle);
	void CurrentTabChanged(int tabIndex);

private:
	bool notify(QObject* receiver, QEvent* even);

private:
	MainWindow *				m_mainWidow;
	SystemTray *				m_trayIcon;

	RadioPageController *		m_radioPageController;
	int							m_radioPageIndex;

	TVPageController *			m_tvPageController;
	int							m_tvPageIndex;

	UpdateController *			m_updateController;

	UserIdle *					m_userIdle;
};

}

#endif
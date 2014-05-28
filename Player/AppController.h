#ifndef MP_APP_CONTROLLER_H
#define MP_APP_CONTROLLER_H

#include "Prerequirements.h"
#include "SingleApplication.h"

namespace mp {

class AppController : public SingleApplication
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
	void FlashInstalled();

private:
	void HandleMssageFromAnotherInst(const QString& message);
	bool notify(QObject* receiver, QEvent* even);

private:
	MainWindow *				m_mainWidow;
	SystemTray *				m_trayIcon;

	RadioPageController *		m_radioPageController;
	int							m_radioPageIndex;

	TVPageController *			m_tvPageController;
	int							m_tvPageIndex;

	UpdateController *			m_updateController;

	PluginManager * 			m_pluginManager;

	UserIdle *					m_userIdle;
};

}

#endif
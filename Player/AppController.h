#ifndef MP_APP_CONTROLLER_H
#define MP_APP_CONTROLLER_H

#include "Prerequirements.h"
#include "SingleApplication.h"

namespace mp {
namespace controller {

class AppController : public SingleApplication
{
	Q_OBJECT

public:
	AppController(int argc, char *argv[]);
	~AppController();

	void SetLang(const QString& lang);
	void CreateChildControllers();
	void CreateView(bool silent);
	void InitSignalsSlots();

	static AppController& Inst();

private slots:
	void Showtdown(int exitCode = 0);
	void UpdateStarted();
	void UpdateFinished(bool success, bool restartRequired);
	void UserIdleStateChanged(bool isIdle);
	void CurrentPageChanged(view::TabPage * newPages, view::TabPage * oldPage);
	void FlashInstalled();
	void SearchTracks(QString filter);
	void PauseAllControllers();

private:
	void HandleMssageFromAnotherInst(const QString& message);
	bool notify(QObject* receiver, QEvent* even);

private:
	view::MainWindow *			m_mainWidow;
	view::SystemTray *			m_trayIcon;

	// Page controllers
	IPageControllerPtr			m_radioPageController;
	IPageControllerPtr			m_playerPageController;
	IPageControllerPtr			m_tvPageController;

	UpdateController *			m_updateController;
	PluginManager * 			m_pluginManager;
	UserIdle *					m_userIdle;
};

} //namespace controller
} //namespace mp

#endif
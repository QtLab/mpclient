#include "AppController.h"
#include "UpdateController.h"
#include "RadioPageController.h"
#include "TVPageController.h"
#include "Common.h"
#include "UserIdle.h"
#include "MainWindow.h"
#include "TabWidget.h"
#include "Titlebar.h"
#include "SystemTray.h"

namespace mp {

AppController::AppController(int argc, char *argv[])
	:QApplication(argc, argv)
	,m_radioPageController(new RadioPageController())
	,m_tvPageController(new TVPageController())
	,m_updateController(new UpdateController())
	,m_userIdle(new UserIdle())
{
	//TODO: QTimer to launch update m_updateController->Run();
}

AppController::~AppController()
{
	m_updateController->deleteLater();
}

void AppController::CreateView()
{
	m_mainWidow = new MainWindow();

	m_trayIcon = new SystemTray(m_mainWidow);
	m_trayIcon->show();

	m_mainWidow->AddTab(m_radioPageController->View());
	m_mainWidow->AddTab(m_tvPageController->View());

	m_mainWidow->show();
}

void AppController::InitSignalsSlots()
{
	connect(m_trayIcon, SIGNAL(ShowtdownApplicationReuest()), SLOT(Showtdown()));
	connect(m_updateController, SIGNAL(UpdateFinished(bool)), SLOT(UpdateFinished(bool)));
	connect(m_userIdle, SIGNAL(IdleStateChanged(bool)), SLOT(UserIdleStateChanged(bool)));
}

AppController& AppController::Inst()
{
	AppController * inst  = static_cast<AppController*>(qApp);
	return *inst;
}

bool AppController::notify(QObject* receiver, QEvent* even)
{
#ifdef Q_OS_WIN32
	__try
	{
		return QApplication::notify(receiver, even);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		NotifyErrorLog(typeid(*even).name(), receiver);
	}

#else
	try
	{
		return QApplication::notify(receiver, even);
	}
	catch(std::exception& ex)
	{
		qDebug("Error <unknown> sending event %s to object %s (%s), reason: %s", 
			eventName, qPrintable(receiver->objectName()), typeid(*receiver).name(), ex.what());
	}
#endif

	return true;
}

void AppController::Showtdown(int exitCode)
{
	m_mainWidow->deleteLater();
	m_trayIcon->deleteLater();

	exit(exitCode);
}

void AppController::UpdateFinished(bool restartRequired)
{
	if(!restartRequired)
	{
		m_radioPageController->ReLoadData();
		m_tvPageController->ReLoadData();
	}
	else
	{
		//TODO: try to start loader in watch mode
		if(m_mainWidow->isHidden())
		{
			Showtdown(SILNET_UPDATE_EXIT_CODE);
		}
		else
		{
			//TODO: message box about restart required
			Showtdown(UPDATE_EXIT_CODE);
		}
	}
}

void AppController::UserIdleStateChanged(bool isIdle)
{
}

void NotifyErrorLog(const char* eventName, QObject* receiver)
{
	qDebug("Error <unknown> sending event %s to object %s (%s)", 
		eventName, qPrintable(receiver->objectName()), typeid(*receiver).name());
}

}

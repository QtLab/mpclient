#include "AppController.h"
#include "UpdateController.h"
#include "RadioPageController.h"
#include "TVPageController.h"

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
	// Metadata
	//connect(&AudioStreamController::Inst(), SIGNAL(MetadataUpdated(const ChannelMetadata&)), 
		//m_mainWidow->TitleBar(), SLOT(MetadataUpdated(const ChannelMetadata&)));

	connect(m_trayIcon, SIGNAL(ShowtdownApplicationReuest()), SLOT(Showtdown()));

	connect(m_updateController, SIGNAL(UpdateFinished(bool)), SLOT(UpdateFinished(bool)));
	connect(m_userIdle, SIGNAL(IdleStateChanged(bool)), SLOT(UserIdleStateChanged(bool)));
}

AppController& AppController::Inst()
{
	AppController * inst  = static_cast<AppController*>(qApp);
	return *inst;
}

void Log(const char* eventName, QObject* receiver)
{
	qDebug("Error <unknown> sending event %s to object %s (%s)", 
		eventName, qPrintable(receiver->objectName()), typeid(*receiver).name());
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
		Log(typeid(*even).name(), receiver);
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

void AppController::Showtdown()
{
	m_mainWidow->deleteLater();
	m_trayIcon->deleteLater();

	exit(777777);
	//exit(7777777);
	//quit();
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
	}
}

void AppController::UserIdleStateChanged(bool isIdle)
{
}

}

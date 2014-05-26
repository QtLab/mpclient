#include "AppController.h"
#include "UpdateController.h"
#include "RadioPageController.h"
#include "TVPageController.h"
#include "CommandLine.h"
#include "Common.h"
#include "Config.h"
#include "UserIdle.h"
#include "MainWindow.h"
#include "SystemTray.h"

#include <QMessageBox>

namespace mp {

AppController::AppController(int argc, char *argv[])
	:SingleApplication(argc, argv, "{5D8D9A8F-31A1-49E7-B730-E8396556366A}")
	,m_radioPageController(new RadioPageController())
	,m_tvPageController(new TVPageController())
	,m_updateController(new UpdateController())
	,m_userIdle(new UserIdle())
{
	QDir::setCurrent(QCoreApplication::applicationDirPath());
}

AppController::~AppController()
{
	m_updateController->deleteLater();
}

void AppController::SetLang(const QString& lang)
{
	QTranslator * translator = new QTranslator();
	bool loaded = translator->load(lang, ":/mp/Translations");
	if(loaded)
	{
		installTranslator(translator);
	}
}

void AppController::CreateView()
{
	SetLang("ru");

	m_mainWidow = new MainWindow();

	m_trayIcon = new SystemTray(m_mainWidow);
	m_trayIcon->show();

	m_radioPageIndex = m_mainWidow->AddTab(m_radioPageController->View());
	m_tvPageIndex = m_mainWidow->AddTab(m_tvPageController->View());
	
	CurrentTabChanged(m_radioPageIndex);

	CommandLine cmd;

	if(cmd.IsSilent())
	{
		m_mainWidow->hide();
	}
	else
	{
		m_mainWidow->show();
	}
}

void AppController::InitSignalsSlots()
{
	connect(m_trayIcon, SIGNAL(ShowtdownApplicationReuest()), SLOT(Showtdown()));
	connect(m_trayIcon, SIGNAL(UpdateReuest()), SLOT(UpdateStarted()));
	 
	connect(m_updateController, SIGNAL(UpdateFinished(bool)), SLOT(UpdateFinished(bool)));
	connect(m_userIdle, SIGNAL(IdleStateChanged(bool)), SLOT(UserIdleStateChanged(bool)));

	connect(m_mainWidow, SIGNAL(CurrentTabChanged(int)), SLOT(CurrentTabChanged(int)));
}

AppController& AppController::Inst()
{
	AppController * inst  = static_cast<AppController*>(qApp);
	return *inst;
}

void AppController::Showtdown(int exitCode)
{
	m_mainWidow->deleteLater();
	m_trayIcon->deleteLater();	

	qDebug() << "Showtdown exit code: " << exitCode;

	exit(exitCode);
}

void AppController::UpdateStarted()
{
#ifndef _DEBUG
	m_updateController->CheckForUpdate();
#endif
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
#ifdef Q_OS_WIN32
#ifndef _DEBUG
		QString cmd = QString("Launcher.exe%0").arg(m_mainWidow->isHidden() ? QString(" /S") : QString::null);
		QProcess::startDetached(cmd);
#endif
#endif
		if(m_mainWidow->isHidden())
		{
			Showtdown(SILENT_UPDATE_EXIT_CODE);
		}
		else
		{
			QMessageBox::information(m_mainWidow, tr("Updating"), tr("Found a new version"));
			Showtdown(UPDATE_EXIT_CODE);
		}
	}
}

void AppController::UserIdleStateChanged(bool isIdle)
{
}

void AppController::CurrentTabChanged(int tabIndex)
{
	if(tabIndex == m_radioPageIndex)
	{
		Config::Inst().SetTVTabWindowSize(m_mainWidow->Size());

		m_mainWidow->SetSize(Config::Inst().RadioTabWindowSize());
		m_mainWidow->SetResizable(false);
	}
	else
	{
		m_mainWidow->SetSize(Config::Inst().TVTabWindowSize());
		m_mainWidow->SetResizable(true);
	}
}

void AppController::HandleMssageFromAnotherInst(const QString& message)
{
	if(message.compare("show") == 0)
	{
#ifdef Q_OS_WIN32
		::ShowWindow((HWND)m_mainWidow->winId(), SW_RESTORE);
#endif
		m_mainWidow->showNormal();
	}
}

void NotifyErrorLog(const char* eventName, QObject* receiver)
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

}

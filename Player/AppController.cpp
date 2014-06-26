#include "AppController.h"
#include "Common.h"
#include "UpdateController.h"
#include "IPageController.h"
#include "PluginManager.h"
#include "UserIdle.h"
#include "Config.h"
#include "MainWindow.h"
#include "SystemTray.h"
#include "TabPage.h"
#include <QMessageBox>

namespace mp {
namespace controller {

AppController::AppController(int argc, char *argv[])
	:SingleApplication(argc, argv, "{5D8D9A8F-31A1-49E7-B730-E8396556366A}")
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
		qDebug() << "AppController::SetLang:" << lang << " result: " << installTranslator(translator);
	}
}

void AppController::CreateChildControllers()
{
	m_radioPageController = CreatePageController("radio");
	m_playerPageController = CreatePageController("palyer");
	m_tvPageController = CreatePageController("tv");
	m_updateController = new UpdateController();
	m_pluginManager = new PluginManager();
	m_userIdle = new UserIdle();
}

void AppController::CreateView(bool silent)
{
	m_mainWidow = new view::MainWindow();

	m_trayIcon = new view::SystemTray(m_mainWidow);
	m_trayIcon->show();

	m_mainWidow->AddTab(m_radioPageController->View());
	m_mainWidow->AddTab(m_playerPageController->View());
	m_mainWidow->AddTab(m_tvPageController->View());

	CurrentPageChanged(m_radioPageController->View(), NULL);

	if(silent)
	{
		m_mainWidow->hide();
	}
	else
	{
		m_mainWidow->show();
	}

	m_pluginManager->StartStartupPlugin();
}

void AppController::InitSignalsSlots()
{
	connect(m_trayIcon, SIGNAL(ShowtdownApplicationReuest()), SLOT(Showtdown()));
	connect(m_trayIcon, SIGNAL(UpdateReuest()), SLOT(UpdateStarted()));
	connect(m_radioPageController, SIGNAL(SearchTracks(QString)), this, SLOT(SearchTracks(QString)));
	connect(m_tvPageController, SIGNAL(FlashInstalled()), SLOT(FlashInstalled()));
	connect(m_updateController, SIGNAL(UpdateFinished(bool)), SLOT(UpdateFinished(bool)));
	connect(m_userIdle, SIGNAL(IdleStateChanged(bool)), SLOT(UserIdleStateChanged(bool)));
	connect(m_mainWidow, SIGNAL(CurrentPageChanged(view::TabPage *, view::TabPage *)), SLOT(CurrentPageChanged(view::TabPage *, view::TabPage *)));
}

AppController& AppController::Inst()
{
	AppController * inst  = static_cast<AppController*>(qApp);
	return *inst;
}

void AppController::Showtdown(int exitCode)
{
	m_pluginManager->StartShowtdownPlugin();

	m_mainWidow->deleteLater();
	m_trayIcon->deleteLater();	

	qDebug() << "Showtdown exit code: " << exitCode;

	if(exitCode != 0)
	{
#if defined(Q_OS_WIN32) && !defined(_DEBUG)
		QString cmd = QString(QString(LAUCNHER_APP_EXE) + "%0").arg(m_mainWidow->isHidden() ? QString(" /S") : QString::null);
		QProcess::startDetached(cmd);
#endif
	}

	exit(exitCode);
}

void AppController::UpdateStarted()
{
	m_updateController->CheckForUpdate();
}

void AppController::UpdateFinished(bool restartRequired)
{
	if(!restartRequired)
	{
		if(!m_pluginManager->AnyLongRunningPluginStarted())
			m_pluginManager->UnloadAllPlugins();

		m_pluginManager->StartUpdateCompletedPlugin();

		m_radioPageController->ReLoadData();
		m_tvPageController->ReLoadData();
	}
	else
	{
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
	if(isIdle)
	{
		if(m_mainWidow->isHidden() 
			&& !m_radioPageController->IsActive() 
			&& !m_playerPageController->IsActive()
			&& !m_tvPageController->IsActive())
		{
			m_pluginManager->StartUserIdlePlugin();
		}
	}
	else
	{
		m_pluginManager->StopUserIdlePlugin();
	}
}

void AppController::CurrentPageChanged(view::TabPage * newPage, view::TabPage * previewsPage)
{
	if(previewsPage)
	{
		previewsPage->SaveSize(m_mainWidow->Size());
	}

	if(newPage)
	{
		m_mainWidow->SetSize(newPage->RestoreSize());
		m_mainWidow->SetResizable(newPage->Resizable());
	}
}

void AppController::FlashInstalled()
{
	if(!m_mainWidow->isHidden())
	{
		QMessageBox::information(m_mainWidow, tr("Flash player"), tr("Flash was installed press Ok to restart program"));
		Showtdown(RESTART_EXIT_CODE);
	}
	else
	{
		Showtdown(SILENT_RESTART_EXIT_CODE);
	}
}

void AppController::SearchTracks(QString filter)
{
	m_playerPageController->Search(filter);
	m_mainWidow->SetCurrentTabIndex(m_playerPageController->View()->TabIndex());
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

void NotifyErrorLog(QObject* receiver, QEvent* even)
{
	qDebug("Error <unknown> sending event %s to object %s (%s)", 
		typeid(*even).name(), qPrintable(receiver->objectName()), typeid(*receiver).name());
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
		NotifyErrorLog(receiver, even);
	}
#else
	try
	{
		return QApplication::notify(receiver, even);
	}
	catch(std::exception& ex)
	{
		NotifyErrorLog(typeid(*even).name(), receiver);
	}
#endif

	return false;
}

}
}

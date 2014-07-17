#include "AppController.h"
#include "FileUtils.h"
#include "SetUserInfo.h"
#include "WebView.h"
#include "AudioStream.h"
#include "CommandLine.h"
#include "Config.h"
#include "Log.h"

#include <QNetworkProxyFactory>
#include <QProcess>
#include <QDir>

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN32)
	::SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
	::_set_abort_behavior(0,_WRITE_ABORT_MSG);
#endif

	qsrand((uint)QTime::currentTime().msec());

#ifndef _DEBUG
	// Write to the log file only in release mode
	qInstallMessageHandler(mp::Log);
#endif

	mp::controller::AppController app(argc, argv);

	QNetworkProxyFactory::setUseSystemConfiguration(true);

#ifndef _DEBUG
	if(app.IsRunningAnotherInstance())
	{
		app.SendMessageToAnotherInstance("show");
		return 0;
	}
#endif

#if defined(Q_OS_WIN32) && !defined(_DEBUG)
	QProcess::startDetached("Launcher.exe /watch");
#endif

	// Initialzie bass library
	mp::AudioStream::InitGlobal();

	mp::CommandLine cmdLine;

	if(cmdLine.IsInstall())
	{
		mp::SetUserInfo::SendInstall();
	}
	else
	{
		mp::SetUserInfo::SendIfVersionChanged();
	}

	//Delete old files that could not be removed after the update
	mp::FileUtils::Delete(QStringList() << "*tmp" << "*old");

	mp::view::WebView::SetupGlobalSettings();

	app.SetLang("ru");
	app.CreateChildControllers();
	app.CreateView(cmdLine.IsSilent());
	app.InitSignalsSlots();

	return app.exec();
}
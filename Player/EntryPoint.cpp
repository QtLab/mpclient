#include "AppController.h"
#include "WebView.h"
#include "Config.h"
#include "Log.h"

#include <QDir>

int main(int argc, char *argv[])
{
	mp::WebView::SetupGloblaSettings();

#ifndef _DEBUG
	// Write to the log file only in release mode
	qInstallMessageHandler(mp::Log);
#endif

	mp::AppController app(argc, argv);

	QDir::setCurrent(QCoreApplication::applicationDirPath());

	app.CreateView();
	app.InitSignalsSlots();

	return app.exec();
}


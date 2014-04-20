#include "AppController.h"
#include "Config.h"
#include "Log.h"

#include <QDir>

int main(int argc, char *argv[])
{
	mp::Config::Inst();

	mp::AppController app(argc, argv);

	QDir::setCurrent(QCoreApplication::applicationDirPath());

#ifndef _DEBUG
	// Write to the log file only in release mode
	qInstallMessageHandler(mp::Log);
#endif

	app.CreateView();
	app.InitSignalsSlots();

	return app.exec();
}


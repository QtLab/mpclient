#include "AppController.h"
#include "Log.h"
#include "QsltCursorShapeArea.h"

int main(int argc, char *argv[])
{
	mp::AppController app(argc, argv);

#ifndef _DEBUG
	// Write to the log file only in release mode
	qInstallMsgHandler(mp::Log);
#endif

	app.CreateView();
	app.InitSignalsSlots();

	return app.exec();
}


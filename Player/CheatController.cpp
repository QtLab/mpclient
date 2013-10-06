#include "CheatController.h"
#include "TabPagesController.h"
#include "AudioStreamController.h"

#include "MainWindow.h"
#include "TabWidget.h"
#include "Titlebar.h"

namespace mp {

CheatController::CheatController(int argc, char *argv[])
	:QApplication(argc, argv)
{
	TabPagesController::Inst().LoadData();
}

CheatController::~CheatController()
{
}

void  CheatController::LaunchView(const QString& viewNamve)
{
}

void  CheatController::StopView(const QString& viewNamve)
{
}

void  CheatController::StopAllVideo()
{
}


CheatController& CheatController::Inst()
{
	CheatController * inst  = static_cast<CheatController*>(qApp);
	return *inst;
}

/*
void Log1(const char* eventName, QObject* receiver)
{
	qDebug("Error <unknown> sending event %s to object %s (%s)", 
		eventName, qPrintable(receiver->objectName()), typeid(*receiver).name());
}

bool CheatController::notify(QObject* receiver, QEvent* even)
{
#ifdef Q_OS_WIN32
	__try
	{
		return QApplication::notify(receiver, even);
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		Log1(typeid(*even).name(), receiver);
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
*/

}

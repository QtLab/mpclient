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

}

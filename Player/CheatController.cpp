#include "CheatController.h"
#include "TabPagesController.h"
#include "MediaController.h"

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

void CheatController::CreateView()
{
	m_view = new MainWindow();

	TabWidget * tabs = TabPagesController::Inst().CreateTabsView();

	TabPage * radioPage = TabPagesController::Inst().CreateRadioTabView();
	tabs->SetRadioPage(radioPage);

	TabPage * tvPage = TabPagesController::Inst().CreateTVTabView();
	tabs->SetTvPage(tvPage);

	m_view->AddWidget(tabs);

	m_view->show();	
}

void CheatController::InitSignalSlotConnections()
{
	// Metadata
	connect(&MediaController::Inst(), SIGNAL(MetadataUpdated(const ChannelMetadata&)), 
		m_view->TitleBar(), SLOT(MetadataUpdated(const ChannelMetadata&)));
}

CheatController& CheatController::Inst()
{
	CheatController * inst  = static_cast<CheatController*>(qApp);
	return *inst;
}

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

}

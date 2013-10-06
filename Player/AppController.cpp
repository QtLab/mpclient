#include "AppController.h"
#include "TabPagesController.h"
#include "AudioStreamController.h"

#include "MainWindow.h"
#include "TabWidget.h"
#include "Titlebar.h"

namespace mp {

AppController::AppController(int argc, char *argv[])
	:QApplication(argc, argv)
{
	TabPagesController::Inst().LoadData();
}

AppController::~AppController()
{
}

void AppController::CreateView()
{
	m_view = new MainWindow();

	TabWidget * tabs = TabPagesController::Inst().CreateTabsView();

	TabPage * radioPage = TabPagesController::Inst().CreateabView("RADIO");
	tabs->AddPage(radioPage, tr("RADIO"));

	TabPage * tvPage = TabPagesController::Inst().CreateabView("TV");
	tabs->AddPage(tvPage, tr("TV"));

	m_view->AddWidget(tabs);

	m_view->show();	
}

void AppController::InitSignalSlotConnections()
{
	// Metadata
	connect(&AudioStreamController::Inst(), SIGNAL(MetadataUpdated(const ChannelMetadata&)), 
		m_view->TitleBar(), SLOT(MetadataUpdated(const ChannelMetadata&)));
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

}

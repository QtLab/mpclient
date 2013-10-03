#include "TabPagesController.h"
#include "RadioPage.h"
#include "TVPage.h"
#include "TabWidget.h"

namespace mp {

TabPagesController* TabPagesController::m_instance = 0;

TabPagesController& TabPagesController::Inst()
{
	if(!m_instance)
		m_instance = new TabPagesController();

	return *m_instance;
}

TabPagesController::TabPagesController()
{
}

TabPagesController::~TabPagesController()
{
}

const ChannelSourceModel& TabPagesController::TVChannels()
{
	return m_tvChannels;
}

const ChannelSourceModel& TabPagesController::RadioChannels()
{
	return m_radioChannels;
}

TabWidget* TabPagesController::CreateTabsView()
{
	TabWidget * tabs = new TabWidget(NULL, "rootTab", "rootTabBar");
	tabs->TabBar()->setCursor(Qt::PointingHandCursor);

	qDebug() << "TabWidget widget created";

	return tabs;
}

TabPage* TabPagesController::CreateabView(const QString& tabName)
{
	if(tabName == "TV")
	{
		return CreateTVTabView();
	}
	else if(tabName == "RADIO")
	{	
		return CreateRadioTabView();
	}

	return NULL;
}


TabPage* TabPagesController::CreateTVTabView()
{
	TabPage * tab = new TVPage(NULL, &m_tvChannels);

	qDebug() << "RadioPage widget created";

	return tab;
}

TabPage* TabPagesController::CreateRadioTabView()
{
	TabPage * tab = new RadioPage(NULL, &m_radioChannels);

	qDebug() << "TVPage widget created";

	return tab;
}

void TabPagesController::LoadData()
{
	m_radioChannels.Load("radio.json");
	m_tvChannels.Load("tv.json");
}

}
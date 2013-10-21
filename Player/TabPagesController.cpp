#include "TabPagesController.h"
#include "RadioPage.h"
#include "TVPage.h"
#include "TabWidget.h"
#include "RadioPageController.h"

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
	m_tabs.Load("..//tabs.json");

	ReLoadData();
}

TabPagesController::~TabPagesController()
{
	
}

const ChannelSourceModel& TabPagesController::TVChannels()
{
	return m_tvChannels;
}
TabWidget* TabPagesController::CreateTabsView()
{
	TabWidget * tabs = new TabWidget(NULL, "tabWidget", "taBar");


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
		return RadioPageController::Inst().View();
	}

	return NULL;
}

TabPage* TabPagesController::CreateTVTabView()
{
	TVPage * tab = new TVPage(NULL, &m_tvChannels);
	
	qDebug() << "RadioPage widget created";

	return tab;
}

void TabPagesController::ReLoadData()
{
#ifdef _DEBUG
	m_tvGenres.Load("..\\tvgenres.json");
	m_tvChannels.SetGenres(m_tvGenres);
	m_tvChannels.Load("..\\tv.json");
#else
	m_tvGenres.Load("config\\tvgenres.json");
	m_tvChannels.SetGenres(m_tvGenres);
	m_tvChannels.Load("config\\tv.json");
#endif
}

}
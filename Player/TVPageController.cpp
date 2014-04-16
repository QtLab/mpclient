#include "TVPageController.h"
#include "ChannelSourceModel.h"
#include "TVPage.h"
#include "Path.h"

namespace mp {

TVPageController::TVPageController()
	:m_channels(new ChannelSourceModel())
	,m_view(new TVPage(NULL, m_channels))
{
	ReLoadData();

	/*
	connect(m_view, SIGNAL(PlayRadio(const QString&)), this, SLOT(PlayRadio(const QString&)));
	connect(m_view, SIGNAL(GenreChanged(const QString&)), this, SLOT(GenreChanged(const QString&)));
	connect(m_view, SIGNAL(TopStationslUpdated()), this, SLOT(TopStationslUpdated()));
	connect(m_view, SIGNAL(PauseRadio()), this, SLOT(PauseRadio()));

	TopStationslUpdated();
	m_view->UpdateCurrentGenreStations(&m_stations);
	//m_view->UpdateLastStations(&m_lastSations);
	m_view->GenreModelUpdated(&m_radioGenres);
	m_view->Init();
	
	qDebug() << "Radio widget created";
	*/
}

TVPageController::~TVPageController()
{
	
}


void TVPageController::ReLoadData()
{
	m_channels->Load(ConfigFilePath("tv.json"));
}

TabPage* TVPageController::View()
{
	return m_view;
}

}
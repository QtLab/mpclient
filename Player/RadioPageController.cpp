#include "RadioPageController.h"
#include "RadioPage.h"
#include "TVPage.h"
#include "TabWidget.h"
#include "AudioStreamController.h"

namespace mp {

RadioPageController* RadioPageController::m_instance = 0;

RadioPageController& RadioPageController::Inst()
{
	if(!m_instance)
		m_instance = new RadioPageController();

	return *m_instance;
}

RadioPageController::RadioPageController()
	:m_view(NULL)
{
	ReLoadData();

	m_view = new RadioPage(NULL, &m_stations, &m_radioGenres);
	m_view->Connect(SIGNAL(playRadio(const QString& id)), this, SLOT(PlayRadio(const QString& id)));
	qDebug() << "Radio widget created";
}

RadioPageController::~RadioPageController()
{
	
}


void RadioPageController::ReLoadData()
{
#ifdef _DEBUG
	m_radioGenres.Load("..\\radiogenres.json");
	m_stations.SetGenres(m_radioGenres);
	m_stations.Load("..\\radio.json");
#else
	m_radioGenres.Load("config\\radiogenres.json");
	m_radioChannels.SetGenres(m_radioGenres);
	m_radioChannels.Load("config\\radio.json");
#endif
}

TabPage* RadioPageController::View()
{
	return m_view;
}

void RadioPageController::PlayRadio(const QString& id)
{
	ChannelSourcePtr channel = m_stations.FindById(id);
	if(channel)
	{
		AudioStreamController::Inst().SetUrl(channel->Url());
		AudioStreamController::Inst().Play();
	}
	else
	{
		qDebug() << "PlayRadio: station doesn't found, id: \"" << id << "\"";
	}
}

void RadioPageController::PauseRadio()
{
	AudioStreamController::Inst().Pause();
}

void RadioPageController::RadioGenreSelect(const QString& id)
{

}

}
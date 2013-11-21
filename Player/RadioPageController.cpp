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

	m_view = new RadioPage(NULL);

	connect(m_view, SIGNAL(PlayRadio(const QString&)), this, SLOT(PlayRadio(const QString&)));
	connect(m_view, SIGNAL(GenreChanged(const QString&)), this, SLOT(GenreChanged(const QString&)));
	connect(m_view, SIGNAL(TopStationslUpdated()), this, SLOT(TopStationslUpdated()));
	connect(m_view, SIGNAL(PauseRadio()), this, SLOT(PauseRadio()));

	TopStationslUpdated();
	m_view->UpdateCurrentGenreStations(&m_stations);
	m_view->UpdateLastStations(&m_lastSations);
	m_view->GenreModelUpdated(&m_radioGenres);
	m_view->Init();
	
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

		// Update last staions with new  channel
		LastStationsUpdated(channel);
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

void RadioPageController::GenreChanged(const QString& id)
{
	m_currentSations.Cleanup();

	foreach(ChannelSourcePtr channel, m_stations.Items())
	{
		// Genre id
		QString currentGenreId = channel->GenreId();//index.data(ChannelSourceModel::GenreId).toString();;
		if(currentGenreId.isEmpty())
		{
			GenreItemPtr genre = m_radioGenres.FindById(id);

			if(!genre.isNull())
			{
				currentGenreId = genre->Id();
			}
		}

		// Channel id
		QString channelId = channel->Id();//index.data(ChannelSourceModel::Id).toString();

		if(!currentGenreId.isEmpty() && !channelId.isEmpty())
		{
			if(currentGenreId == id)
			{
				ChannelSourcePtr channel = m_stations.FindById(channelId);
				m_currentSations.Add(channel, true);
			}
		}
	}

	//m_view->UpdateCurrentGenreStations(&m_currentSations);
}

void RadioPageController::TopStationslUpdated()
{
	m_topSations.Cleanup();

#ifdef _DEBUG
	m_topSations.Load("..\\topradio.json");
#else
	m_topSations.Load("config\\topradio.json);
#endif

	m_view->UpdateTopStations(&m_topSations);
}

void RadioPageController::LastStationsUpdated(ChannelSourcePtr newchannel)
{
	int rows = m_lastSations.rowCount();
	if(rows <= 2)
	{
		m_lastSations.Add(newchannel, true);
	}
	else
	{
		m_lastSations.removeRow(2);
		m_lastSations.Add(newchannel, true);
	}

	m_view->UpdateLastStations(&m_lastSations);
}

}
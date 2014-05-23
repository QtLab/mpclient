#include "RadioPageController.h"
#include "RadioPage.h"
#include "AudioStream.h"
#include "ChannelMetadataModel.h"
#include "Config.h"
#include "Path.h"

namespace mp {

RadioPageController::RadioPageController()
	:m_view(NULL)
	,m_audioStream(new AudioStream())
{
	ReLoadData();

	m_updateStateTimer.setInterval(2000); // 2 sec
	connect(&m_updateStateTimer, SIGNAL(timeout()), SLOT(UpdateViewState()));
	m_updateStateTimer.start();

	m_allStationsProxyModel.setSourceModel(&m_stations);
	m_allStationsProxyModel.SetSortType(ChannelSourceSortFilterProxyModel::ByName);
	m_lastStationsProxyModel.sort(0);

	m_lastStationsProxyModel.setSourceModel(&m_stations);
	m_lastStationsProxyModel.SetSortType(ChannelSourceSortFilterProxyModel::ByLastPlayTime);
	m_lastStationsProxyModel.sort(0);

	m_topStationsProxyModel.setSourceModel(&m_stations);
	m_topStationsProxyModel.SetSortType(ChannelSourceSortFilterProxyModel::ByTop);
	m_topStationsProxyModel.sort(0);

	m_searchStationsProxyModel.setSourceModel(&m_stations);

	m_view = new RadioPage(NULL, &m_categories, 
							&m_allStationsProxyModel, &m_topStationsProxyModel, 
							&m_lastStationsProxyModel, &m_searchStationsProxyModel);

	connect(m_view, SIGNAL(PlayRadio(int)), this, SLOT(PlayRadio(int)));
	connect(m_view, SIGNAL(PauseRadio()), this, SLOT(PauseRadio()));
	connect(m_view, SIGNAL(VolumeChanged(qreal)), this, SLOT(VolumeChanged(qreal)));
	connect(m_view, SIGNAL(CategoryChanged(int)), this, SLOT(CategoryChanged(int)));
	connect(m_view, SIGNAL(SearchFilterChanged(QString)), this, SLOT(SearchFilterChanged(QString)));
	connect(m_audioStream.data(), SIGNAL(MetadataUpdated(const ChannelMetadata&)), SLOT(MetadataUpdated(const ChannelMetadata&)));

	CategoryChanged(m_categories.First()->Id());
	m_view->SetVolume(Config::Inst().Volume());

	qDebug() << "Radio widget created";
}

RadioPageController::~RadioPageController()
{
}

void RadioPageController::ReLoadData()
{
	m_categories.Load(ConfigFilePath("radiocatygories.j"));
	m_stations.LoadWithStats(ConfigFilePath("radio.j"));
}

TabPage* RadioPageController::View()
{
	return m_view;
}

void RadioPageController::PlayRadio(int id)
{
	if(m_audioStream->State() == AudioStream::ASPaused && m_currentChannel->Id() == id)
	{
		m_audioStream->Play(true);
	}
	else
	{
		ChannelSourcePtr channel = m_stations.Find(id);
		if(channel)
		{
			m_currentChannel = channel;

			m_audioStream->SetUrl(channel->Url());
			m_audioStream->Play(false);

			int newPlayCount = channel->PlayCount() + 1;
			channel->SetPlayCount(newPlayCount);
			channel->SetLastPlayNow();

			m_stations.SaveStats(ConfigFilePath("radio.j"));

			m_topStationsProxyModel.invalidate();
			m_lastStationsProxyModel.invalidate();
		}
		else
		{
			qDebug() << "RadioPageController::PlayRadio: station doesn't found, id: \"" << id << "\"";
		}
	}
}

void RadioPageController::PauseRadio()
{
	m_audioStream->Pause();
}

void RadioPageController::VolumeChanged(qreal value)
{
	Config::Inst().SetVolume(value);
}

void RadioPageController::CategoryChanged(int id)
{
	m_allStationsProxyModel.SetCategoryIdFilter(id);
	m_lastStationsProxyModel.SetCategoryIdFilter(id);
	m_topStationsProxyModel.SetCategoryIdFilter(id);
}

void RadioPageController::SearchFilterChanged(QString seasrch)
{
	m_searchStationsProxyModel.SetNameFilter(seasrch);
}

void RadioPageController::UpdateViewState()
{
	if(m_audioStream->IsPlaying())
	{
		ChannelMetadata metadata;
		m_audioStream->GetMetaData(metadata);

		m_view->UpdateMetadata(metadata.ToString());
		m_view->SetPlayingState(true);
	}
	else
	{
		m_view->UpdateMetadata(QString::null);
		m_view->SetPlayingState(false);
	}
	
}

}
#include "RadioPageController.h"
#include "RadioPage.h"
#include "AudioStream.h"
#include "ChannelMetadataModel.h"
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
	m_allStationsProxyModel.setDynamicSortFilter(true);
	m_lastStationsProxyModel.setSourceModel(&m_stations);
	m_lastStationsProxyModel.setDynamicSortFilter(true);
	m_topStationsProxyModel.setSourceModel(&m_stations);
	m_topStationsProxyModel.setDynamicSortFilter(true);
	m_searchStationsProxyModel.setSourceModel(&m_stations);
	m_searchStationsProxyModel.setDynamicSortFilter(true);

	m_view = new RadioPage(NULL, &m_categories, 
							&m_allStationsProxyModel, &m_topStationsProxyModel, 
							&m_lastStationsProxyModel, &m_searchStationsProxyModel);

	connect(m_view, SIGNAL(PlayRadio(int)), this, SLOT(PlayRadio(int)));
	connect(m_view, SIGNAL(PauseRadio()), this, SLOT(PauseRadio()));
	connect(m_view, SIGNAL(VolumeChanged(qreal)), this, SLOT(VolumeChanged(qreal)));
	connect(m_view, SIGNAL(CategoryChanged(int)), this, SLOT(CategoryChanged(int)));
	connect(m_view, SIGNAL(SearchFilterChanged(QString)), this, SLOT(SearchFilterChanged(QString)));
	connect(m_audioStream.data(), SIGNAL(MetadataUpdated(const ChannelMetadata&)), SLOT(MetadataUpdated(const ChannelMetadata&)));

	CategoryChanged(m_categories.Items().first()->Id());
	qDebug() << "Radio widget created";
}

RadioPageController::~RadioPageController()
{
}

void RadioPageController::ReLoadData()
{
	m_categories.Load(ConfigFilePath("radiogenres"));
	m_stations.LoadWithStats(ConfigFilePath("radio"));
}

TabPage* RadioPageController::View()
{
	return m_view;
}

void RadioPageController::PlayRadio(int id)
{
	if(m_audioStream->State() == AudioStream::ASPaused)
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
	m_audioStream->SetVolume(value);
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
	if(!m_currentChannel.isNull())
	{
		ChannelMetadata metadata;
		m_audioStream->GetMetaData(metadata);
		m_view->Update(m_audioStream->IsPlaying(), m_currentChannel->Id(), m_currentChannel->Name(), metadata.ToString());
	}
	else
	{
		m_view->Update(false, 0, QString::null, QString::null);
	}
	
}

}
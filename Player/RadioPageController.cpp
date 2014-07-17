#include "RadioPageController.h"
#include "RadioPage.h"
#include "AudioStream.h"
#include "ChannelMetadataModel.h"
#include "Config.h"
#include "Path.h"

namespace mp {
namespace controller {

RadioPageController::RadioPageController()
	:m_view(NULL)
	,m_audioStream("radio")
{
	m_allStationsProxyModel.setSourceModel(&m_stations);
	m_allStationsProxyModel.SetSortType(model::RadioSourcesSortFilterProxyModel::ByName);
	m_lastStationsProxyModel.sort(0);

	m_lastStationsProxyModel.setSourceModel(&m_stations);
	m_lastStationsProxyModel.SetSortType(model::RadioSourcesSortFilterProxyModel::ByLastPlayTime);
	m_lastStationsProxyModel.sort(0);

	m_topStationsProxyModel.setSourceModel(&m_stations);
	m_topStationsProxyModel.SetSortType(model::RadioSourcesSortFilterProxyModel::ByTop);
	m_topStationsProxyModel.sort(0);

	m_searchStationsProxyModel.setSourceModel(&m_stations);

	m_view = new view::RadioPage(NULL, &m_categories, 
							&m_allStationsProxyModel, &m_topStationsProxyModel, 
							&m_lastStationsProxyModel, &m_searchStationsProxyModel);

	connect(m_view, SIGNAL(PlayRadio(int)), this, SLOT(PlayRadio(int)));
	connect(m_view, SIGNAL(ResumeRadio()), this, SLOT(ResumeRadio()));
	connect(m_view, SIGNAL(PauseRadio()), this, SLOT(PauseRadio()));
	connect(m_view, SIGNAL(VolumeChanged(qreal)), this, SLOT(VolumeChanged(qreal)));
	connect(m_view, SIGNAL(CategoryChanged(int)), this, SLOT(CategoryChanged(int)));
	connect(m_view, SIGNAL(SearchFilterChanged(QString)), this, SLOT(SearchFilterChanged(QString)));
	connect(m_view, SIGNAL(SearchTracks(QString)), this, SIGNAL(SearchTracks(QString)));

	connect(&m_audioStream, SIGNAL(MetadataUpdated(const ChannelMetadata&)), SLOT(MetadataUpdated(const ChannelMetadata&)));
	connect(&m_audioStream, SIGNAL(StateChanged(AudioStream::ASState)), SLOT(AudioStreamStateChanged(AudioStream::ASState)));

	ReLoadData();

	m_view->SetVolume(Config::Inst().Volume(m_audioStream.Name()));

	qDebug() << "Radio page widget was created";
}

bool RadioPageController::IsActive() const
{
	bool playing = m_audioStream.State() == AudioStream::ASPlaying;
	return playing;
}

view::TabPage* RadioPageController::View() const
{
	return m_view;
}

void RadioPageController::ReLoadData()
{
	m_categories.Cleanup();
	m_stations.Cleanup();

	m_categories.Load(Path::ConfigFile("radiocatygories.j"));
	m_stations.LoadWithStats(Path::ConfigFile("radio.j"));

	bool topVisible = false;

	if(m_currentCategory.isNull())
	{
		topVisible = true;
		m_currentCategory = m_categories.First();
	}

	if(!m_currentCategory.isNull())
	{
		m_view->SetCategory(m_currentCategory->Id(), topVisible);
		SetCategoryFilter(m_currentCategory->Id());
	}
}

void RadioPageController::Search(const QString& filter)
{
}

void RadioPageController::Pause()
{
	PauseRadio();
}

void RadioPageController::SetCategoryFilter(int categoryId)
{
	m_allStationsProxyModel.SetCategoryIdFilter(categoryId);
	m_lastStationsProxyModel.SetCategoryIdFilter(categoryId);
	m_topStationsProxyModel.SetCategoryIdFilter(categoryId);

	m_allStationsProxyModel.invalidate();
	m_lastStationsProxyModel.invalidate();
	m_topStationsProxyModel.invalidate();
}

void RadioPageController::PlayRadio(int id)
{
	emit PauseAllControllers();

	model::RadioSourcePtr channel = m_stations.Find(id);

	if(channel)
	{
		m_currentChannel = channel;
		m_audioStream.Play(channel->Url());

		int newPlayCount = channel->PlayCount() + 1;
		channel->SetPlayCount(newPlayCount);
		channel->SetLastPlayNow();

		m_stations.SaveStats(Path::ConfigFile("radio.j"));

		m_topStationsProxyModel.invalidate();
		m_lastStationsProxyModel.invalidate();
	}
	else
	{
		qDebug() << "RadioPageController::PlayRadio: station doesn't found, id: \"" << id << "\"";
	}
}

void RadioPageController::ResumeRadio()
{
	emit PauseAllControllers();

	if(m_audioStream.State() == AudioStream::ASPaused)
	{
		m_audioStream.Resume();
	}
	else
	{
		if(m_currentChannel.isNull())
		{
			m_currentChannel = m_stations.Random();
		}

		if(!m_currentChannel.isNull())
		{
			m_audioStream.Play(m_currentChannel->Url());
		}
	}
}

void RadioPageController::PauseRadio()
{
	m_audioStream.Pause();
}

void RadioPageController::VolumeChanged(qreal value)
{
	Config::Inst().SetVolume(value, m_audioStream.Name());
}

void RadioPageController::CategoryChanged(int id)
{
	m_currentCategory = m_categories.FindById(id);
	SetCategoryFilter(id);
}

void RadioPageController::SearchFilterChanged(QString filter)
{
	m_searchStationsProxyModel.SetNameFilter(filter);
}

void RadioPageController::MetadataUpdated(const ChannelMetadata& metadata)
{
	m_view->SetMetadata(metadata.ToString());
}

void RadioPageController::AudioStreamStateChanged(AudioStream::ASState newState)
{
	if(!m_currentChannel.isNull())
	{
		m_view->SetPlayStationName(m_currentChannel->Name());
	}

	switch(newState)
	{
		case AudioStream::ASStarting:
			m_view->SetMetadata(m_currentChannel->Url());
			m_view->SetPlayingState(true);
			break;
		case AudioStream::ASPlaying:
			m_view->SetMetadata(QString::null);
			m_view->SetPlayingState(true);
			break;
		case AudioStream::ASPaused:
			m_view->SetPlayingState(false);
			break;
		case AudioStream::ASStopped:
			m_view->SetPlayingState(false);
			break;
	};
}

} //namespace controller
} //namespace mp
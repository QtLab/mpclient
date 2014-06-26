#include "PlayerPageController.h"
#include "AudioStream.h"
#include "PlayerPage.h"
#include "TrackModel.h"
#include "TrackModelDataAccessor.h"
#include "AudioStreamAsyncDownloader.h"
#include "Config.h"
#include "FileUtils.h"
#include "Path.h"

#include <QTimer>
#include <QThreadPool>
#include <QFileDialog>

namespace mp {
namespace controller {

PlayerPageController::PlayerPageController()
	:m_trackSearchResultModel(new model::TrackModel())
	,m_downlaodedTracksModel(new model::TrackModel())
	,m_trackSearcher(new TrackModelDataAccessor())
	,m_audioStream(new AudioStream("player"))
{
	m_view = new view::PlayerPage(NULL, m_trackSearchResultModel, m_downlaodedTracksModel);
	m_view->SetVolume(Config::Inst().Volume(m_audioStream->Name()));
	m_trackSearcher->SetModel(m_trackSearchResultModel);

	connect(m_view, SIGNAL(PlayTrack(int)), this, SLOT(PlayTrack(int)));
	connect(m_view, SIGNAL(PauseTrack()), this, SLOT(PauseTrack()));
	connect(m_view, SIGNAL(ResumeTrack()), this, SLOT(ResumeTrack()));
	connect(m_view, SIGNAL(VolumeChanged(qreal)), this, SLOT(VolumeChanged(qreal)));
	connect(m_view, SIGNAL(SearchFilterChanged(const QString&)), this, SLOT(Search(const QString&)));
	connect(m_view, SIGNAL(PositionChanged(int)), this, SLOT(SetTrackPosition(int)));
	connect(m_view, SIGNAL(DownloadTrack(int)), this, SLOT(DownloadTrack(int)));
	connect(m_view, SIGNAL(DeleteTrack(int)), this, SLOT(DeleteTrack(int)));
	connect(m_view, SIGNAL(TrackChangePathRequest()), this, SLOT(ChangeTracksPath()));
	
	connect(m_audioStream, SIGNAL(MetadataUpdated(const ChannelMetadata&)), SLOT(MetadataUpdated(const ChannelMetadata&)));
	connect(m_audioStream, SIGNAL(StateChanged(AudioStream::ASState)), SLOT(AudioStreamStateChanged(AudioStream::ASState)));

	m_syncProgressTimer = new QTimer(this);
	m_syncProgressTimer->setInterval(1000);
	connect(m_syncProgressTimer, SIGNAL(timeout()), SLOT(SyncProgress()));

	ReLoadData();

}

PlayerPageController::~PlayerPageController()
{
	m_syncProgressTimer->deleteLater();
	m_trackSearcher->deleteLater();
	m_view->deleteLater();
	m_trackSearchResultModel->deleteLater();
}

bool PlayerPageController::IsActive() const
{
	bool playing = m_audioStream->State() == AudioStream::ASPlaying;
	return playing;
}

view::TabPage* PlayerPageController::View() const
{
	return m_view;
}

void PlayerPageController::ReLoadData()
{
	m_downlaodedTracksModel->Cleanup();
	m_downlaodedTracksModel->Load(Path::ConfigFile("tracks.db"));
}

void PlayerPageController::Stop()
{
	m_audioStream->Stop();
}

void PlayerPageController::Search(const QString& seasrchFilter)
{
	m_trackSearchResultModel->Cleanup();
	m_trackSearcher->Search(seasrchFilter);
	m_view->SetSearchFilter(seasrchFilter);
}

void PlayerPageController::PlayTrack(int id)
{
	model::TrackPtr track;

	if(id < 0)
	{
		track = m_trackSearchResultModel->First();
	}
	else
	{
		track = m_trackSearchResultModel->Find(id);
	}

	if(track)
	{
		m_currentTrack = track;

		QString url = track->FilePath();

		if(!Path::Exists(url))
		{
			url = track->Url();
		}

		m_audioStream->Play(url);
	}
	else
	{
		qDebug() << "PlayerPageController::PlayRadio: track doesn't found, id: \"" << id << "\"";
	}
}

void PlayerPageController::ResumeTrack()
{
	m_audioStream->Resume();
}

void PlayerPageController::PauseTrack()
{
	m_audioStream->Pause();
}

void PlayerPageController::VolumeChanged(qreal value)
{
	Config::Inst().SetVolume(value, m_audioStream->Name());
}

void PlayerPageController::SetTrackPosition(int position)
{
	m_audioStream->SetCurrentPos(position);
}

void PlayerPageController::AudioStreamStateChanged(AudioStream::ASState newState)
{
	if(newState == AudioStream::ASState::ASPlaying)
	{
		SyncProgress();
		m_syncProgressTimer->start();
		m_view->SetPlayingState(true);
	}
	else
	{
		m_syncProgressTimer->stop();
		m_view->SetPlayingState(false);
	}
}

void PlayerPageController::SyncProgress()
{
	long pos = m_audioStream->GetCurrentPos();
	long len = m_audioStream->GetLength();

	m_view->SetTractPosition(pos, len);
}

void PlayerPageController::DownloadTrack(int id)
{
	model::TrackPtr track = m_trackSearchResultModel->Find(id);

	if(track)
	{
		auto downlaoder = new AudioStreamAsyncDownloader(track->Url(), track->FilePath(), track->Id());
		if(downlaoder->FileOpened())
		{
			connect(downlaoder, SIGNAL(Finished()), SLOT(TrackDownlaoded()));
			QThreadPool::globalInstance()->start(downlaoder);

			track->SetDownloading(true);
			m_trackSearchResultModel->EmitDataChanged();
		}
	}
}

void PlayerPageController::TrackDownlaoded()
{
	AudioStreamAsyncDownloader * downlaoder = qobject_cast<AudioStreamAsyncDownloader*>(sender());
	if(downlaoder)
	{
		int id = downlaoder->Tag().toInt();
		model::TrackPtr track = m_trackSearchResultModel->Find(id);

		if(track)
		{
			m_downlaodedTracksModel->Add(track);
			m_downlaodedTracksModel->Save(Path::ConfigFile("tracks.db"));
			m_downlaodedTracksModel->EmitDataChanged();

			track->SetDownloading(false);
			m_trackSearchResultModel->EmitDataChanged();
		}
	}
}

void PlayerPageController::DeleteTrack(int id)
{
	model::TrackPtr track = m_downlaodedTracksModel->Find(id);

	if(track)
	{
		FileUtils::Delete(track->FilePath());
		//m_trackSearchResultModel->EmitDataChanged();

		if(m_downlaodedTracksModel->Remove(id))
		{
			m_downlaodedTracksModel->Save(Path::ConfigFile("tracks.db"));
			//m_downlaodedTracksModel->EmitDataChanged();
		}
	}
}

void PlayerPageController::ChangeTracksPath()
{
	QString dir = QFileDialog::getExistingDirectory(m_view, tr("Select directory to save the music"),
									Config::Inst().PathToSaveTracks(),
									QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);

	if(!dir.isEmpty())
	{
		Config::Inst().SetPathToSaveTracks(dir);
		m_trackSearchResultModel->EmitDataChanged();
	}
}

}
}
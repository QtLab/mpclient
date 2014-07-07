#ifndef MP_PLAYER_PAGE_CONTROLLER_H
#define MP_PLAYER_PAGE_CONTROLLER_H

#include "IPageController.h"
#include "AudioStream.h"

namespace mp {
namespace controller {

class PlayerPageController : public IPageController
{
	Q_OBJECT

public:	
	PlayerPageController();
	~PlayerPageController();

	bool IsActive() const;
	view::TabPage* View() const;
	void ReLoadData();
	void Pause();

public slots:
	void Search(const QString& seasrch);

private slots:
	void PlayTrack(int id);
	void ResumeTrack();
	void PauseTrack();
	void VolumeChanged(qreal value);
	void SetTrackPosition(int position);
	void AudioStreamStateChanged(AudioStream::ASState newState);
	void SyncProgress();
	void DownloadTrack(int id);
	void TrackDownlaoded();
	void DeleteTrack(int id);
	void ChangeTracksPath();

private:
	// Widget view
	view::PlayerPage *					m_view;
	// Audio stream manager
	AudioStreamPtr						m_audioStream;
	model::TrackPtr						m_currentTrack;
	model::TrackModelPtr				m_trackSearchResultModel;
	model::TrackModelPtr				m_downlaodedTracksModel;
	TrackModelDataAccessorPtr			m_trackSearcher;
	QTimer *							m_syncProgressTimer;
};

}
}

#endif
#ifndef MP_PLAYER_PAGE_WIDGET_H
#define MP_PLAYER_PAGE_WIDGET_H

#include "TabPage.h"
#include "Prerequirements.h"

namespace mp {
namespace view {

class PlayerPage : public TabPage
{
	Q_OBJECT

public:
	PlayerPage(QWidget* parent, QAbstractItemModel* searchTracksModel, QAbstractItemModel* downloadedTracksModel);
	virtual ~PlayerPage();

	// User leave the page
	void Enter();
	// User leave the page
	bool Leave();
	// Languege changed
	void RetranslateUI();
	QString Name() const;

	bool Resizable() const;
	QSize RestoreSize() const;
	void SaveSize(const QSize& size);

	void SetVolume(qreal val);
	void SetTractPosition(long currentPos, long trackLenght);
	void SetPlayingState(bool play);
	void SetSearchFilter(const QString filter);

signals:
	void PlayTrack(int id);
	void ResumeTrack();
	void PauseTrack();
	void VolumeChanged(qreal value);
	void PositionChanged(int value);
	void SearchFilterChanged(QString seasrch);
	void DownloadTrack(int id);
	void DeleteTrack(int id);
	void TrackChangePathRequest();

private:
	bool									m_initialized;
	// QML view
	QQuickView *							m_quickView;
};

}
}

#endif
#include "PlayerPage.h"
#include "Config.h"
#include "Path.h"

#include <QDebug>
#include <QQuickView>
#include <QQmlContext>
#include <QQuickItem>
#include <QHBoxLayout>
#include <QAbstractItemModel>

namespace mp {
namespace view {

PlayerPage::PlayerPage(QWidget* parent, QAbstractItemModel* searchTracksModel, QAbstractItemModel* downloadedTracksModel)
	:QuickPageBase(parent, "Player\\PalyerPageView.qml")
	,m_initialized(false)
{
	RootContext()->setContextProperty("searchTracksModel", searchTracksModel);
	RootContext()->setContextProperty("downloadedTracksModel", downloadedTracksModel);

	connect(RootQuickItem(), SIGNAL(playTrack(int)), this, SIGNAL(PlayTrack(int)));
	connect(RootQuickItem(), SIGNAL(resumeTrack()), this, SIGNAL(ResumeTrack()));
	connect(RootQuickItem(), SIGNAL(pauseTrack()), this, SIGNAL(PauseTrack()));
	connect(RootQuickItem(), SIGNAL(volumeChanged(qreal)), this, SIGNAL(VolumeChanged(qreal)));
	connect(RootQuickItem(), SIGNAL(positionChanged(int)), this, SIGNAL(PositionChanged(int)));
	connect(RootQuickItem(), SIGNAL(searchFilterChanged(QString)), this, SIGNAL(SearchFilterChanged(QString)));
	connect(RootQuickItem(), SIGNAL(downloadTrack(int)), this, SIGNAL(DownloadTrack(int)));
	connect(RootQuickItem(), SIGNAL(deleteTrack(int)), this, SIGNAL(DeleteTrack(int)));
	connect(RootQuickItem(), SIGNAL(tracksPathChangeRequest()), this, SIGNAL(TrackChangePathRequest()));
}

PlayerPage::~PlayerPage()
{
}

void PlayerPage::Enter()
{
	if(!m_initialized)
	{
		m_initialized = true;
		emit SearchFilterChanged("");
	}

	QuickPageBase::Enter();
}

bool PlayerPage::Leave()
{
	return QuickPageBase::Leave();
}

void PlayerPage::RetranslateUI()
{
}

QString PlayerPage::Name() const
{
	return tr("Player");
}

QString PlayerPage::ToolTip() const
{
	return tr("a huge collection of artists and albums");
}

bool PlayerPage::Resizable() const
{
	return false;
}

QSize PlayerPage::RestoreSize() const
{
	return QSize(580, 534);
}

void PlayerPage::SaveSize(const QSize& size)
{
}

void PlayerPage::SetVolume(qreal val)
{
	QMetaObject::invokeMethod(RootQuickItem(), "setVolume", Q_ARG(QVariant, val));
}

void PlayerPage::SetTractPosition(long currentPos, long trackLenght)
{
	QMetaObject::invokeMethod(RootQuickItem(), "updateTrackPosition", Q_ARG(QVariant, currentPos), Q_ARG(QVariant, trackLenght));
}

void PlayerPage::SetPlayingState(bool play)
{
	QMetaObject::invokeMethod(RootQuickItem(), "setPlayingState", Q_ARG(QVariant, play));
}

void PlayerPage::SetSearchFilter(const QString filter)
{
	m_initialized = true;
	QMetaObject::invokeMethod(RootQuickItem(), "setSearchFilter", Q_ARG(QVariant, filter));
}

} // end namespace view
} // end namespace mp
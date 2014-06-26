#include "PlayerPage.h"
#include "Config.h"
#include "Path.h"

#include <QQuickView>
#include <QQmlContext>
#include <QQuickItem>
#include <QHBoxLayout>
#include <QDebug>
#include <QAbstractItemModel>

namespace mp {
namespace view {

PlayerPage::PlayerPage(QWidget* parent, QAbstractItemModel* searchTracksModel, QAbstractItemModel* downloadedTracksModel)
	:TabPage(parent)
	,m_initialized(false)
{
	QHBoxLayout *box = new QHBoxLayout(this);
	box->setContentsMargins(0, 0, 0, 0);

	m_quickView = new QQuickView();
	box->addWidget(QWidget::createWindowContainer(m_quickView, this));

	QString localUrl = QUrl::fromLocalFile(Path::QmlFile("Player\\PalyerPageView.qml")).toString();
	qDebug()  << "PlayerPage load qml: "<< localUrl;

	m_quickView->rootContext()->setContextProperty("searchTracksModel", searchTracksModel);
	m_quickView->rootContext()->setContextProperty("downloadedTracksModel", downloadedTracksModel);
	
	m_quickView->setSource(localUrl);
	m_quickView->setResizeMode(QQuickView::SizeRootObjectToView);

	connect(m_quickView->rootObject(), SIGNAL(playTrack(int)), this, SIGNAL(PlayTrack(int)));
	connect(m_quickView->rootObject(), SIGNAL(resumeTrack()), this, SIGNAL(ResumeTrack()));
	connect(m_quickView->rootObject(), SIGNAL(pauseTrack()), this, SIGNAL(PauseTrack()));
	connect(m_quickView->rootObject(), SIGNAL(volumeChanged(qreal)), this, SIGNAL(VolumeChanged(qreal)));
	connect(m_quickView->rootObject(), SIGNAL(positionChanged(int)), this, SIGNAL(PositionChanged(int)));
	connect(m_quickView->rootObject(), SIGNAL(searchFilterChanged(QString)), this, SIGNAL(SearchFilterChanged(QString)));
	connect(m_quickView->rootObject(), SIGNAL(downloadTrack(int)), this, SIGNAL(DownloadTrack(int)));
	connect(m_quickView->rootObject(), SIGNAL(deleteTrack(int)), this, SIGNAL(DeleteTrack(int)));
	connect(m_quickView->rootObject(), SIGNAL(tracksPathChangeRequest()), this, SIGNAL(TrackChangePathRequest()));
}

PlayerPage::~PlayerPage()
{
}

void PlayerPage::Enter()
{
	if(!m_initialized)
	{
		m_initialized = true;
		emit SearchFilterChanged("*");
	}
}

bool PlayerPage::Leave()
{
	return true;
}

void PlayerPage::RetranslateUI()
{
}

QString PlayerPage::Name() const
{
	return tr("Player");
}

bool PlayerPage::Resizable() const
{
	return false;
}

static const QString TabName("music");

QSize PlayerPage::RestoreSize() const
{
	//return Config::Inst().TabSize(TabName);
	return QSize(580, 450);
}

void PlayerPage::SaveSize(const QSize& size)
{
	//Config::Inst().SetTabSize(TabName, size);
}

void PlayerPage::SetVolume(qreal val)
{
	QMetaObject::invokeMethod(m_quickView->rootObject(), "setVolume", Q_ARG(QVariant, val));
}

void PlayerPage::SetTractPosition(long currentPos, long trackLenght)
{
	QMetaObject::invokeMethod(m_quickView->rootObject(), "updateTrackPosition", Q_ARG(QVariant, currentPos), Q_ARG(QVariant, trackLenght));
}

void PlayerPage::SetPlayingState(bool play)
{
	QMetaObject::invokeMethod(m_quickView->rootObject(), "setPlayingState", Q_ARG(QVariant, play));
}

void PlayerPage::SetSearchFilter(const QString filter)
{
	m_initialized = true;
	QMetaObject::invokeMethod(m_quickView->rootObject(), "setSearchFilter", Q_ARG(QVariant, filter));
}

}
}
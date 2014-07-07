#include "RadioPage.h"
#include "Path.h"

#include <QQuickView>
#include <QQmlContext>
#include <QQuickItem>
#include <QHBoxLayout>
#include <QDebug>
#include <QMenu>

namespace mp {
namespace view {

RadioPage::RadioPage(QWidget* parent, QAbstractItemModel* categoriesModel, QAbstractItemModel* allStationsModel, 
					QAbstractItemModel* topStationsModel, QAbstractItemModel* lastStationsModel, QAbstractItemModel* searchStationsModel)
	:TabPage(parent)
{
	QHBoxLayout *box = new QHBoxLayout(this);
	box->setContentsMargins(0, 0, 0, 0);

	m_quickView = new QQuickView();
	box->addWidget(QWidget::createWindowContainer(m_quickView, this));

	QString localUrl = QUrl::fromLocalFile(Path::QmlFile("Radio\\RadioPageView.qml")).toString();
	qDebug()  << "RadioPage load qml: "<< localUrl;

	m_quickView->rootContext()->setContextProperty("categoriesModel", categoriesModel);
	m_quickView->rootContext()->setContextProperty("allStationsModel", allStationsModel);
	m_quickView->rootContext()->setContextProperty("lastStationsModel", lastStationsModel);
	m_quickView->rootContext()->setContextProperty("topStationsModel", topStationsModel);
	m_quickView->rootContext()->setContextProperty("searchStationsModel", searchStationsModel);

	m_quickView->setSource(localUrl);
	m_quickView->setResizeMode(QQuickView::SizeRootObjectToView);

	connect(m_quickView->rootObject(), SIGNAL(categoryChanged(int)), this, SIGNAL(CategoryChanged(int)));
	connect(m_quickView->rootObject(), SIGNAL(resumeRadio()), this, SIGNAL(ResumeRadio()));
	connect(m_quickView->rootObject(), SIGNAL(playRadio(int)), this, SIGNAL(PlayRadio(int)));
	connect(m_quickView->rootObject(), SIGNAL(volumeChanged(qreal)), this, SIGNAL(VolumeChanged(qreal)));
	connect(m_quickView->rootObject(), SIGNAL(pauseRadio()), this, SIGNAL(PauseRadio()));
	connect(m_quickView->rootObject(), SIGNAL(searchFilterChanged(QString)), this, SIGNAL(SearchFilterChanged(QString)));
	connect(m_quickView->rootObject(), SIGNAL(searchTracks(QString)), this, SIGNAL(SearchTracks(QString)));
	connect(m_quickView->rootObject(), SIGNAL(showSearchContextMenu(int, int)), this, SLOT(ShowSearchContextMenu(int, int)));
}

RadioPage::~RadioPage()
{
}

void RadioPage::Enter()
{
}

bool RadioPage::Leave()
{
	return true;
}

void RadioPage::RetranslateUI()
{
}

QString RadioPage::Name() const
{
	return tr("Radio");
}

bool RadioPage::Resizable() const
{
	return false;
}

QSize RadioPage::RestoreSize() const
{
	return QSize(580, 351);
}

void RadioPage::SaveSize(const QSize& size)
{
}

void RadioPage::SetVolume(qreal val)
{
	QMetaObject::invokeMethod(m_quickView->rootObject(), "setVolume", Q_ARG(QVariant, val));
}

void RadioPage::SetPlayingState(bool play)
{
	QMetaObject::invokeMethod(m_quickView->rootObject(), "setPlayingState", Q_ARG(QVariant, play));
}

void RadioPage::SetMetadata(const QString& metadata)
{
	QMetaObject::invokeMethod(m_quickView->rootObject(), "updateMetadata", Q_ARG(QVariant, metadata));
}

void RadioPage::SetPlayStationName(const QString& name)
{
	QMetaObject::invokeMethod(m_quickView->rootObject(), "setPlayStationName", Q_ARG(QVariant, name));
}

void RadioPage::SetCategory(int id)
{
	QMetaObject::invokeMethod(m_quickView->rootObject(), "setCategory", Q_ARG(QVariant, id));
}

void RadioPage::ShowSearchContextMenu(int x, int y)
{
	//QMenu menu(this);
	//menu.addAction(tr("Open in New Window"), this, SLOT(OpenUrlInDefaultBrowser()));
	//menu.exec(QPoint(x, y));
}

}
}
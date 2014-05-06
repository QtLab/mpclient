#include "RadioPage.h"
#include "Path.h"
#include "CategoriesModel.h"
#include "ChannelSourceModel.h"
#include "ChannelSourceSortFilterProxyModel.h"

#include <QQuickView>
#include <QQmlContext>
#include <QQuickItem>

#include <QDebug>
#include <QHBoxLayout>

namespace mp {

RadioPage::RadioPage(QWidget* parent, QObject* categoriesModel, QObject* allStationsModel, 
					QObject* topStationsModel, QObject* lastStationsModel, QObject* searchStationsModel)
	:TabPage(parent)
{
	setObjectName("RadioPage");

	QHBoxLayout *box = new QHBoxLayout(this);
	box->setContentsMargins(0, 0, 0, 0);

	m_quickView = new QQuickView();
	box->addWidget(QWidget::createWindowContainer(m_quickView, this));

	QString localUrl = QUrl::fromLocalFile(QmlFilePath("RadioPageView.qml")).toString();

	qDebug()  << "RadioPage load qml: "<< localUrl;

	m_quickView->setSource(localUrl);
	m_quickView->setResizeMode(QQuickView::SizeRootObjectToView);

	m_quickView->rootContext()->setContextProperty("categoriesModel", categoriesModel);
	m_quickView->rootContext()->setContextProperty("allStationsModel", allStationsModel);
	m_quickView->rootContext()->setContextProperty("lastStationsModel", lastStationsModel);
	m_quickView->rootContext()->setContextProperty("topStationsModel", topStationsModel);
	m_quickView->rootContext()->setContextProperty("searchStationsModel", searchStationsModel);

	connect(m_quickView->rootObject(), SIGNAL(categoryChanged(int)), this, SIGNAL(CategoryChanged(int)));
	connect(m_quickView->rootObject(), SIGNAL(playRadio(int)), this, SIGNAL(PlayRadio(int)));
	connect(m_quickView->rootObject(), SIGNAL(volumeChanged(qreal)), this, SIGNAL(VolumeChanged(qreal)));
	connect(m_quickView->rootObject(), SIGNAL(pauseRadio()), this, SIGNAL(PauseRadio()));
	connect(m_quickView->rootObject(), SIGNAL(searchFilterChanged(QString)), this, SIGNAL(SearchFilterChanged(QString)));
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

void RadioPage::Update(bool isPlay, int stationId, const QString& stationName, const QString& metadata)
{
	QQuickItem * item = m_quickView->rootObject();

	QMetaObject::invokeMethod(item, "updateViewState", 
								Q_ARG(QVariant, isPlay),Q_ARG(QVariant, stationId), 
								Q_ARG(QVariant, stationName), Q_ARG(QVariant, metadata));
}

}
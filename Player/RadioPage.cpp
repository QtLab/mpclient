#include "RadioPage.h"
#include "Path.h"
#include "CategoriesModel.h"
#include "ChannelSourceModel.h"
#include "ChannelSourceSortFilterProxyModel.h"
#include "AudioStream.h"

#include <QQuickView>
#include <QQmlContext>
#include <QQuickItem>

#include <QDebug>
#include <QHBoxLayout>

namespace mp {

RadioPage::RadioPage(QWidget* parent, CategoriesModelPtr categories, ChannelSourceModelPtr allStations, 
				ChannelSourceModelPtr topStations, ChannelSourceModelPtr lastStation)
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

	m_allStationsProxyModel = new ChannelSourceSortFilterProxyModel(this);
	m_allStationsProxyModel->setSourceModel(allStations);
	m_allStationsProxyModel->setDynamicSortFilter(true);

	m_lastStationsProxyModel = new ChannelSourceSortFilterProxyModel(this);
	m_lastStationsProxyModel->setSourceModel(allStations);
	m_lastStationsProxyModel->setDynamicSortFilter(true);
	 
	m_topStationsProxyModel = new ChannelSourceSortFilterProxyModel(this);
	m_topStationsProxyModel->setSourceModel(allStations);
	m_topStationsProxyModel->setDynamicSortFilter(true);

	m_quickView->rootContext()->setContextProperty("radioGenres", categories);
	m_quickView->rootContext()->setContextProperty("allStations", m_allStationsProxyModel);
	m_quickView->rootContext()->setContextProperty("lastStations", m_lastStationsProxyModel);
	m_quickView->rootContext()->setContextProperty("topStations", m_topStationsProxyModel);

	connect(m_quickView->rootObject(), SIGNAL(genreChanged(int)), this, SLOT(GenreChanged(int)));
}

RadioPage::~RadioPage()
{
}

void RadioPage::Init()
{
	//connect(m_view->rootObject(), SIGNAL(playRadio()), this, SLOT(PlayCurrentRadio()));
	//connect(m_view->rootObject(), SIGNAL(pauseRadio()), this, SLOT(PauseCurrentRadio()));
	//connect(m_view->rootObject(), SIGNAL(genreChanged()), this, SLOT(CurrentGenreChanged()));
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
	return tr("RADIO");
}

void RadioPage::PlayChannel(const QString& id)
{
	emit PlayRadio(id);
}

void RadioPage::PauseCurrentChannel()
{
	emit PauseCurrentRadio();
}

void RadioPage::GenreChanged(int genreId)
{
	m_allStationsProxyModel->SetGenreIdFilter(genreId);
	m_lastStationsProxyModel->SetGenreIdFilter(genreId);
	m_topStationsProxyModel->SetGenreIdFilter(genreId);
}

/*
void RadioPage::Connect(const char* signal, QObject* reciever, const char* slot)
{
	connect(m_view, signal, reciever, slot);
}

QString RadioPage::CurrentRaioChannelId()
{
	QString stationId = m_view->rootObject()->property("currentRadioId").toString();
	return stationId;
}

QString RadioPage::CurrentGenreId()
{
	QString genre = m_view->rootObject()->property("currentGenreId").toString();
	return genre;
}

void RadioPage::CurrentChannelChanged(ChannelSource* channel)
{
	m_view->rootContext()->setContextProperty("currentChannel", channel);
}

void RadioPage::GenreModelUpdated(GenreModel* genres)
{
	m_view->rootContext()->setContextProperty("radioGenres", genres);
}

void RadioPage::UpdateTopStations(ChannelSourceModel* channels)
{
	m_view->rootContext()->setContextProperty("topStations", channels);
	QVariant v = m_view->rootContext()->contextProperty("topStations");
}

void RadioPage::UpdateCurrentGenreStations(ChannelSourceModel* channels)
{
	m_view->rootContext()->setContextProperty("currentStations", channels);
}

void RadioPage::UpdateLastStations(ChannelSourceModel* channels)
{
	m_view->rootContext()->setContextProperty("lastStations", channels);
}

void RadioPage::CurrentGenreChanged()
{
	QString currentRadioId = m_view->rootContext()->contextProperty("currentGenreId").toString();
	emit PauseRadio(currentRadioId);
}
*/

}
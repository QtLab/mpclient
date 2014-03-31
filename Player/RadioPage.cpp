#include "RadioPage.h"
#include "Path.h"
#include "ChannelSourceModel.h"
#include "AudioStream.h"

#include <QQuickView>
#include <QQmlContext>
#include <QQuickItem>

#include <QDebug>
#include <QHBoxLayout>
#include <QDir>

namespace mp {

RadioPage::RadioPage(QWidget* parent)
	:TabPage(parent)
{
	setObjectName("RadioPage");

	QHBoxLayout *box = new QHBoxLayout(this);
	box->setContentsMargins(0, 0, 0, 0);

	m_view = new QQuickView();
	box->addWidget(QWidget::createWindowContainer(m_view, this));
}

RadioPage::~RadioPage()
{
}

void RadioPage::Init()
{
	QString path = QmlFilePath("RadioPageView.qml");	
	qDebug()  << "RadioPage::RadioPage path: "<< path;

	path = QUrl::fromLocalFile(path).toString();
	m_view->setSource(path);
	m_view->setResizeMode(QQuickView::SizeRootObjectToView);

	connect(m_view->rootObject(), SIGNAL(playRadio()), this, SLOT(PlayCurrentRadio()));
	connect(m_view->rootObject(), SIGNAL(pauseRadio()), this, SLOT(PauseCurrentRadio()));
	connect(m_view->rootObject(), SIGNAL(genreChanged()), this, SLOT(CurrentGenreChanged()));
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

void RadioPage::PlayCurrentRadio()
{
	QString currentRadioId = m_view->rootContext()->contextProperty("currentRadioId").toString();
	emit PlayRadio(currentRadioId);
}

void RadioPage::PauseCurrentRadio()
{
	QString currentRadioId = m_view->rootContext()->contextProperty("currentRadioId").toString();
	emit PauseRadio(currentRadioId);
}

void RadioPage::CurrentGenreChanged()
{
	QString currentRadioId = m_view->rootContext()->contextProperty("currentGenreId").toString();
	emit PauseRadio(currentRadioId);
}

}
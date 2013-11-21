#include "RadioPage.h"
#include "Path.h"
#include "ChannelSourceModel.h"
#include "AudioStreamController.h"
#include "QsltCursorShapeArea.h"

#include <QDeclarativeError>
#include <QtDeclarative/QDeclarativeView>
#include <QtDeclarative/QDeclarativeEngine>
#include <QDeclarativeContext>
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

    m_view = new QDeclarativeView(this);

	qmlRegisterType<QsltCursorShapeArea>("Cursor", 1, 0, "CursorShapeArea");
	box->addWidget(m_view);
}

RadioPage::~RadioPage()
{
}

void RadioPage::Init()
{
	QString path = QmlFilePath("RadioPageView.qml");
	
	qDebug()  << "RadioPage::RadioPage path: "<< path;

	//emit TopStationslUpdated();
	//emit GenreChanged("0");

	path = QUrl::fromLocalFile(path).toString();
	m_view->setSource(path);
    m_view->setResizeMode(QDeclarativeView::SizeRootObjectToView);	

	//qmlRegisterType<RadioPage>("Cursor", 1, 0, "CursorShapeArea");
	//qmlRegisterType<ChannelSourceModel>("Cursor", 1, 0, "CursorShapeArea");
	//m_view->rootContext()->setContextProperty("RadioPage", this);

	QDeclarativeError err;
	foreach(err, m_view->errors())
	{
		qDebug() << "RadioPage::RadioPage error: " << err.toString();
	}

	QObject *rootObject = dynamic_cast<QObject*>(m_view->rootObject());

	connect(m_view->rootContext(), SIGNAL(playRadio()), this, SLOT(PlayCurrentRadio()));
	connect(m_view->rootContext(), SIGNAL(pauseRadio()), this, SLOT(PauseCurrentRadio()));
	connect(m_view->rootContext(), SIGNAL(genreChanged()), this, SLOT(CurrentGenreChanged()));
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
	QString s = v.typeName();

	//ChannelSourceModel* channels
	//ChannelSource* channel = v.toList().first().value<ChannelSource*>();

	bool e = v.isNull();
	bool n = e;
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
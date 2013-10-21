#include "RadioPage.h"
#include "Path.h"
#include "ChannelSourceModel.h"
#include "AudioStreamController.h"
#include "QsltCursorShapeArea.h"

#include <QDeclarativeError>
#include <QtDeclarative/QDeclarativeView>
#include <QDeclarativeContext>
#include <QDebug>
#include <QHBoxLayout>
#include <QDir>

namespace mp {

RadioPage::RadioPage(QWidget* parent, ChannelSourceModel* channels, GenreModel* genres)
	:TabPage(parent)
{
	setObjectName("RadioPage");
	QHBoxLayout *box = new QHBoxLayout(this);
	box->setContentsMargins(0, 0, 0, 0);

	QString path = QmlFilePath("RadioPageView.qml");
	
	qDebug()  << "RadioPage::RadioPage path: "<< path;
    m_view = new QDeclarativeView(this);

	qmlRegisterType<QsltCursorShapeArea>("Cursor", 1, 0, "CursorShapeArea");

	ChannelSourceModelUpdated(channels);
	GenreModelUpdated(genres);

	m_view->setSource( QUrl::fromLocalFile(path));
    m_view->setResizeMode(QDeclarativeView::SizeRootObjectToView);

	QDeclarativeError err;
	foreach(err, m_view->errors())
	{
		qDebug() << "RadioPage::RadioPage error: " << err.toString();
	}

	box->addWidget(m_view);
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

void RadioPage::Connect(const char* signal, QObject* reciever, const char* slot)
{
	connect(m_view, signal, reciever, slot);
}

void RadioPage::CurrentChannelChanged(ChannelSource* channel)
{
	m_view->rootContext()->setContextProperty("currentChannel", channel);
}

void RadioPage::ChannelSourceModelUpdated(ChannelSourceModel* channels)
{
	m_view->rootContext()->setContextProperty("radioChannels", channels);
}

void RadioPage::GenreModelUpdated(GenreModel* genres)
{
	m_view->rootContext()->setContextProperty("radioGenres", genres);
}

}
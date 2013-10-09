#include "RadioPage.h"
#include "Path.h"
#include "ChannelSourceModel.h"
#include "AudioStreamController.h"

#include <QDeclarativeError>
#include <QtDeclarative/QDeclarativeView>
#include <QDeclarativeContext>
#include <QDebug>
#include <QHBoxLayout>

namespace mp {

RadioPage::RadioPage(QWidget* parent, ChannelSourceModel* channels)
	:TabPage(parent)
{
	QHBoxLayout *box = new QHBoxLayout(this);
    m_view = CreateDeclarativeView(QmlFilePath("RadioPage.qml"));

	m_view->rootContext()->setContextProperty("AudioStreamController", (AudioStreamController*)&AudioStreamController::Inst());

	ModelUpdated(channels);

	box->addWidget(m_view);

	//AudioStreamController::Inst().SetChannel(channels.Items().first());
	//AudioStreamController::Inst().Play();
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

void RadioPage::ModelUpdated(ChannelSourceModel* channels)
{
	m_view->rootContext()->setContextProperty("radioChannels", channels);
}

}
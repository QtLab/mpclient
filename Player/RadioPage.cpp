#include "RadioPage.h"
#include "ChannelSourceModel.h"
#include "MediaController.h"

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
    m_view = CreateDeclarativeView("../Player/Views/RadioPage.qml");

	m_view->rootContext()->setContextProperty("MediaController", (MediaController*)&MediaController::Inst());

	ModelUpdated(channels);

	box->addWidget(m_view);

	//MediaController::Inst().SetChannel(channels.Items().first());
	//MediaController::Inst().Play();
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
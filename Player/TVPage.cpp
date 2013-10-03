#include "TVPage.h"
#include "ChannelSourceModel.h"
#include "MediaController.h"

#include <QDeclarativeError>
#include <QtDeclarative/QDeclarativeView>
#include <QDeclarativeContext>
#include <QDebug>
#include <QHBoxLayout>

namespace mp {

TVPage::TVPage(QWidget* parent, ChannelSourceModel* channels)
	:TabPage(parent)
{
	QHBoxLayout *box = new QHBoxLayout(this);
    m_view = CreateDeclarativeView("Views/TVPage.qml");

	ModelUpdated(channels);

	box->addWidget(m_view);
}

TVPage::~TVPage()
{
}

void TVPage::Enter()
{
}

bool TVPage::Leave()
{
	return true;
}

void TVPage::RetranslateUI()
{
}

void TVPage::ModelUpdated(ChannelSourceModel* channels)
{
	m_view->rootContext()->setContextProperty("tvChannels", channels);
}

}
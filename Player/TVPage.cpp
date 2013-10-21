#include "TVPage.h"
#include "Path.h"
#include "WebView.h"

#include "ChannelSourceModel.h"
#include "AudioStreamController.h"

#include <QDeclarativeError>
#include <QtDeclarative/QDeclarativeView>
#include <QDeclarativeContext>
#include <QDebug>
#include <QHBoxLayout>
#include <QWebView>

namespace mp {


TVPage::TVPage(QWidget* parent, ChannelSourceModel* channels)
	:TabPage(parent)
{
	QHBoxLayout *box = new QHBoxLayout(this);
	box->setContentsMargins(0,0,0,0);
	m_view = WebView::Create();
	m_view->setUrl(HtmlilePath("VideoPlayer.html"));
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

void TVPage::ChannelSourceModelUpdated(ChannelSourceModel* channels)
{
}

void TVPage::GenreModelUpdated(GenreModel* channels)
{
}

}
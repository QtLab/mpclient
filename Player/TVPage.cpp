#include "TVPage.h"
#include "Path.h"
#include "WebView.h"

#include "ChannelSourceModel.h"
#include "AudioStreamController.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QWebView>
#include <QDir>

namespace mp {


TVPage::TVPage(QWidget* parent, ChannelSourceModel* channels)
	:TabPage(parent)
{
	QHBoxLayout *box = new QHBoxLayout(this);
	box->setContentsMargins(0,0,0,0);
	m_view = WebView::Create();

	static QUrl baseUrl(QUrl::fromLocalFile(QDir::currentPath() + "/"));

	//QString html = QString("<html><body style='overflow:hidden'><embed src='%1' wmode='direct' width='100%' height='100%'></embed></body></html>").arg("file://C:\\330.swf");
	//m_view->setHtml("<html><body></html></body>");
	//m_view->load(QUrl::fromLocalFile("C:\\330.swf"));

	//m_view->setUrl(HtmlilePath("VideoPlayer.html"));
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

QString TVPage::Name() const
{
	return tr("TV");
}

}
#include "TVPage.h"
#include "Path.h"
#include "WebView.h"
#include "ChannelSourceModel.h"

#include <QWebFrame>
#include <QDebug>
#include <QHBoxLayout>
#include <QDir>
#include <QWebElement>

#include <QQuickView>
#include <QQmlContext>
#include <QQuickItem>

namespace mp {


TVPage::TVPage(QWidget* parent, ChannelSourceModelPtr channels)
	:TabPage(parent)
{
	QHBoxLayout *box = new QHBoxLayout(this);
	box->setContentsMargins(0,0,0,0);
	m_view = WebView::Create();
	box->addWidget(m_view);
	m_view->setUrl(QUrl("http://www.tvigle.ru/category/prokino/chemodanov2/?flt=52&video=4143&ap=1&ref=129&utm_source=11131&utm_medium=11332&utm_campaign=11131"));

	//m_view->load(QUrl("http://google.com"));
	//m_view->setUrl(QUrl("http://www.tvzavr.ru/"));
	//m_view->setUrl(QUrl("http://tvigle.ru"));
	//m_view->setUrl(QUrl::fromLocalFile(HtmlilePath("VideoPlayer.html")));
	//m_view->setHtml("<h1>Hello World</h1>");
	//static QUrl baseUrl(QUrl::fromLocalFile(QDir::currentPath() + "/"));
	//QString html = QString("<html><body style='overflow:hidden'><embed src='%1' wmode='direct' width='100%' height='100%'></embed></body></html>").arg("file:///C:/330.swf");
	//static const QString flashTemplate("<embed src='%0' width='100%' wmode='direct' height='100%' align='middle' quality='high' pluginspage='http://www.macromedia.com/go/getflashplayer' type='application/x-shockwave-flash'></embed>");
	//QString gameHtml = flashTemplate.arg(QUrl::fromLocalFile("c:\\330.swf").toString());
	//m_view->setHtml(gameHtml, QUrl::fromLocalFile(QDir::currentPath() + "/"));
	//m_view->load(QUrl::fromLocalFile("C:\\330.swf"));
}

TVPage::~TVPage()
{
}

void TVPage::Enter()
{
	m_view->setUpdatesEnabled(true);
}

bool TVPage::Leave()
{
	m_view->setUpdatesEnabled(false);
	return true;
}

void TVPage::RetranslateUI()
{
}

QString TVPage::Name() const
{
	return tr("TV");
}

void TVPage::ContentLoaded(bool ok)
{
}

}
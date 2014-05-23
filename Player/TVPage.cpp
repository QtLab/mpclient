#include "TVPage.h"
#include "Path.h"
#include "WebView.h"
#include "FlashUtils.h"

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
	m_layout = new QHBoxLayout(this);
	m_layout->setContentsMargins(0,0,0,0);
	m_view = WebView::Create();
	m_layout->addWidget(m_view);

	// Flash installer
	FlashUtils * flashUtils = new FlashUtils();
	connect(flashUtils, SIGNAL(FlashInstallStarted()), SLOT(FlashInstallStarted()));
	connect(flashUtils, SIGNAL(FlashInstalled(bool)), SLOT(FlashInstalled(bool)));
	connect(flashUtils, SIGNAL(FlashInstallProgressChanged(int)), SLOT(FlashInstallProgressChanged(int)));
	flashUtils->CheckForFlashInstalled();

	//m_view->setContent(FlashUtils::CreateHtmlForCheckFlashInstalled());
	//m_view->setUrl(QUrl("http://www.tvigle.ru/category/prokino/chemodanov2/?flt=52&video=4143&ap=1&ref=129&utm_source=11131&utm_medium=11332&utm_campaign=11131"));

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

void TVPage::FlashInstallStarted()
{
	m_view->setContent(FlashUtils::CreateHtmlFlashInstalling());
}

void TVPage::FlashInstallProgressChanged(int percents)
{
	QString message = tr("Flash installing... %0 percents completed").arg(percents);
	QString js = QString("SetStatus('%0')").arg(message);
	m_view->page()->mainFrame()->evaluateJavaScript(js);
}

void TVPage::FlashInstalled(bool ok)
{
	m_view->deleteLater();

	m_view = WebView::Create();
	m_layout->addWidget(m_view);
	m_view->setUrl(QUrl("http://www.tvigle.ru/category/prokino/chemodanov2/?flt=52&video=4143&ap=1&ref=129&utm_source=11131&utm_medium=11332&utm_campaign=11131"));
}

}
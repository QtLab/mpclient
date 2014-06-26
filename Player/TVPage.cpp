#include "TVPage.h"
#include "Path.h"
#include "UrlBuilder.h"
#include "WebView.h"
#include "FlashUtils.h"
#include "Config.h"

#include <QDebug>
#include <QWebFrame>
#include <QHBoxLayout>
#include <QWebElement>
#include <QWebElement>


namespace mp {
namespace view {

TVPage::TVPage(QWidget* parent, model::ChannelSourceModelPtr channels)
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

bool TVPage::Resizable() const
{
	return true;
}

static const QString TabName("tv");

QSize TVPage::RestoreSize() const
{
	return Config::Inst().TabSize(TabName);
}

void TVPage::SaveSize(const QSize& size)
{
	Config::Inst().SetTabSize(TabName, size);
}

void TVPage::ContentLoaded(bool ok)
{
	QWebElement el = m_view->page()->mainFrame()->findFirstElement("object");

	if(!el.isNull())
	{
		int top = el.geometry().top() - 10;
		m_view->page()->mainFrame()->setScrollPosition(QPoint(0, top));
	}
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
	//emit FlashInstalled();

	m_view->deleteLater();

	m_view = WebView::Create();
	m_layout->addWidget(m_view);

	//m_view->setUrl(QUrl::fromLocalFile(Path::HtmlFile("Kids.html")));
	//m_view->setUrl(QUrl("http://zxrmedia.com/client/files_zaxar_temp/tvigle_redirect.html"));
	connect(m_view->page(), SIGNAL(loadFinished(bool)), SLOT(ContentLoaded(bool)));

	//m_view->setUrl(QUrl::fromLocalFile(Path::HtmlFile("VideoPlayer.html")));
	//m_view->setUrl(QUrl("http://inixmedia.net/music.php/"));
	//m_view->setUrl(QUrl("http://www.tvigle.ru/category/prokino/chemodanov2/?flt=52&video=4143&ap=1&ref=129&utm_source=11131&utm_medium=11332&utm_campaign=11131"));
}

}
}
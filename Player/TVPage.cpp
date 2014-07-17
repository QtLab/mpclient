#include "TVPage.h"
#include "Path.h"
#include "UrlBuilder.h"
#include "WebView.h"
#include "FlashUtils.h"
#include "Config.h"

#include <QDebug>
#include <QQuickView>
#include <QQmlContext>
#include <QQuickItem>
#include <QHBoxLayout>
#include <QAbstractItemModel>
#include <QWebFrame>
#include <QWebPage>
#include <QWebElement>

namespace mp {
namespace view {

const QSize DefaultPageSize			= QSize(580, 534);
const int DefaultWebViewViewHieght	= 370;
const int MinimumQuickViewHieght	= 160;

TVPage::TVPage(QWidget* parent, QAbstractItemModel* categories
								,QAbstractItemModel* tvSourcesCurrentGenre
								,QAbstractItemModel* tvSourcesSearchResults
								,QAbstractItemModel* topTvSources)

	:QuickPageBase(parent, "TV\\TVPageView.qml")
{
	m_webView = WebView::Create();

	m_loadingWebPage = WebView::CreateLoadingWebPage(this);
	m_contentWebPage = new QWebPage(this);
	m_webView->setPage(m_contentWebPage);
	connect(m_contentWebPage, SIGNAL(loadFinished(bool)), SLOT(ContentLoaded(bool)));

	Layout()->addWidget(m_webView);
	m_webView->setVisible(false);
	m_webView->setUrl(QUrl());

	connect(RootQuickItem(), SIGNAL(categoryChanged(int)), this, SIGNAL(TVCurrentCategoryChanged(int)));
	connect(RootQuickItem(), SIGNAL(showGenres(int)), this, SIGNAL(ShowGenres(int)));
	connect(RootQuickItem(), SIGNAL(searchFilterChanged(QString)), this, SIGNAL(SearchFilterChanged(QString)));
	connect(RootQuickItem(), SIGNAL(gotoTVSource(int)), this, SIGNAL(GotoTVSource(int)));
	connect(RootQuickItem(), SIGNAL(closeCurrentTVSource()), this, SLOT(CloseCurrentTVSource()));
	
	RootContext()->setContextProperty("tvCategoriesModel", categories);
	RootContext()->setContextProperty("tvSourcesSearchResultodel", tvSourcesSearchResults);
	RootContext()->setContextProperty("topTvSourcesModel", topTvSources);
	RootContext()->setContextProperty("tvSourcesCurrentGenreModel", tvSourcesCurrentGenre);
}

TVPage::~TVPage()
{
}

void TVPage::Enter()
{
	QString u = m_webView->url().toString();

	if(!m_webView->url().isEmpty() && m_webView->url().toString() != QLatin1String("about:blank"))
	{
		m_webView->setVisible(true);
	}
}

bool TVPage::Leave()
{
	m_webView->setVisible(false);
	return true;
}

void TVPage::RetranslateUI()
{
}

QString TVPage::Name() const
{
	return tr("TV");
}

QString TVPage::ToolTip() const
{
	return tr("the best collection of free videos");
}

bool TVPage::Resizable() const
{
	return false;
}

static const QString TabName("tv");

QSize TVPage::RestoreSize() const
{
	//return Config::Inst().TabSize(TabName);
	return m_pageSize;
}

void TVPage::SaveSize(const QSize& size)
{
	//Config::Inst().SetTabSize(TabName, size);
}

void TVPage::SetCurrentGenre(int id, const QString& name)
{
	QMetaObject::invokeMethod(RootQuickItem(), "setCurrentGenre", Q_ARG(QVariant, id), Q_ARG(QVariant, name));
}

void TVPage::SetCurrentCategory(int id, bool topVisible, int bannerId, const QString& bannerLogo)
{
	QMetaObject::invokeMethod(RootQuickItem(), "setCurrentCategory", Q_ARG(QVariant, id), Q_ARG(QVariant, topVisible)
																	,Q_ARG(QVariant, bannerId), Q_ARG(QVariant, bannerLogo));
}

void TVPage::ShowTVSource(const QString& url)
{
	m_webView->setVisible(true);
	m_webView->setPage(m_loadingWebPage);
	m_webView->setFixedHeight(DefaultWebViewViewHieght);
	m_contentWebPage->mainFrame()->setUrl(QUrl(url));
}

void TVPage::CloseCurrentTVSource()
{
	m_pageSize = DefaultPageSize;
	
	emit SizeChangeRequest(m_pageSize);

	m_webView->setVisible(false);
	m_webView->setUrl(QUrl());
}

void TVPage::ContentLoaded(bool ok)
{
	QWebElement el = m_contentWebPage->mainFrame()->findFirstElement("object");

	if(!el.isNull())
	{
		m_pageSize = el.geometry().size();
		m_webView->setFixedHeight(m_pageSize.height());

		int left = el.geometry().left();

		m_pageSize.setHeight(m_pageSize.height() + MinimumQuickViewHieght);
		m_pageSize.setWidth(m_pageSize.width() + left);

		if(m_pageSize.width() < DefaultPageSize.width())
		{
			m_pageSize.setWidth(DefaultPageSize.width());
		}

		emit SizeChangeRequest(m_pageSize);

		int top = el.geometry().top() - 10;
		m_webView->page()->mainFrame()->setScrollPosition(QPoint(left, top));
	}

	m_webView->setPage(m_contentWebPage);
}

} // end namespace view
} // end namespace mp

/*
// Flash installer
FlashUtils * flashUtils = new FlashUtils();
connect(flashUtils, SIGNAL(FlashInstallStarted()), SLOT(FlashInstallStarted()));
connect(flashUtils, SIGNAL(FlashInstalled(bool)), SLOT(FlashInstalled(bool)));
connect(flashUtils, SIGNAL(FlashInstallProgressChanged(int)), SLOT(FlashInstallProgressChanged(int)));
flashUtils->CheckForFlashInstalled();

private slots:
	void FlashInstallStarted();
	void FlashInstallProgressChanged(int percent);
	void FlashInstalled(bool);

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
*/
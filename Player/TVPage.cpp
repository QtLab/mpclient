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

const QSize		DefaultPageSize			= QSize(580, 534);
const int		MinimumQuickViewHieght	= 90;
const int		DefaultQuickViewHieght	= 460;
const int		DefaultTitlebarHieght	= 75;
const QString	DefaultSelector			= "object";

TVPage::TVPage(QWidget* parent, QAbstractItemModel* categories
								,QAbstractItemModel* tvSourcesCurrentGenre
								,QAbstractItemModel* tvSourcesSearchResults
								,QAbstractItemModel* topTvSources)

	:QuickPageBase(parent, "TV\\TVPageView.qml")
	,m_pageSize(DefaultPageSize)
	,m_resizable(false)
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
	connect(RootQuickItem(), SIGNAL(processBanner(int)), this, SIGNAL(ProcessBanner(int)));
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
	if(!UrlBuilder::IsEmtyUrl(m_contentWebPage->mainFrame()->url()))
	{
		m_webView->setVisible(true);
	}

	QuickPageBase::Enter();
}

bool TVPage::Leave()
{
	m_webView->setVisible(false);
	return QuickPageBase::Leave();
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
	return m_resizable;
}

QSize TVPage::RestoreSize() const
{
	return m_pageSize;
}

void TVPage::SaveSize(const QSize& size)
{
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

void TVPage::ShowTVSource(const QString& url, const QString& tvSourceSelector)
{
	if(tvSourceSelector.isEmpty())
	{
		m_tvSourceSelector = DefaultSelector;
	}
	else
	{
		m_tvSourceSelector = tvSourceSelector;
	}

	m_webView->setVisible(true);
	m_webView->setPage(m_loadingWebPage);

	QuickViewWidget()->setFixedHeight(MinimumQuickViewHieght);
	m_contentWebPage->mainFrame()->setUrl(QUrl(url));
}

void TVPage::CloseCurrentTVSource()
{
	if(m_webView->isVisible() || !UrlBuilder::IsEmtyUrl(m_contentWebPage->mainFrame()->url()))
	{
		m_pageSize = DefaultPageSize;
		m_resizable = false;

		QuickViewWidget()->setFixedHeight(DefaultQuickViewHieght);

		if(IsActived())
		{
			emit SizeChangeRequest(m_pageSize, m_resizable);
		}

		m_webView->setVisible(false);

		m_contentWebPage->mainFrame()->setUrl(QUrl());
	}
}

void TVPage::ContentLoaded(bool ok)
{
	if(!UrlBuilder::IsEmtyUrl(m_contentWebPage->mainFrame()->url()))
	{
		QWebElement el = m_contentWebPage->mainFrame()->findFirstElement(m_tvSourceSelector);

		if(!el.isNull())
		{
			m_pageSize = el.geometry().size();
			int left = el.geometry().left();
			int top = el.geometry().top();

			m_pageSize.setHeight(m_pageSize.height() + MinimumQuickViewHieght + DefaultTitlebarHieght);
			m_pageSize.setWidth(m_pageSize.width() + left);

			if(m_pageSize.width() < DefaultPageSize.width())
			{
				m_pageSize.setWidth(DefaultPageSize.width());
			}

			m_resizable = true;

			if(IsActived())
			{
				emit SizeChangeRequest(m_pageSize, m_resizable);
			}

			m_webView->page()->mainFrame()->setScrollPosition(QPoint(left, top));
		}

		m_webView->setPage(m_contentWebPage);
	}
}

} // end namespace view
} // end namespace mp


#include "WebView.h"
#include "NetworkAccessManager.h"
#include "Path.h"

#include <QWebHitTestResult>
#include <QContextMenuEvent>
#include <QWebPage>
#include <QMenu>
#include <QDebug>
#include <QAction>
#include <QWebElement>
#include <QStandardPaths>
#include <QDesktopServices>

namespace mp {
namespace view {

WebView::WebView(QWidget * parent)
	:QWebView(parent)
{
}

QUrl WebView::DocumentHref()
{
	QString url = page()->mainFrame()->evaluateJavaScript("document.location.href").toString();
	return QUrl(url);
}

QWebView *WebView::createWindow(QWebPage::WebWindowType type)
{
	return NULL;
}

void WebView::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu menu(this);

	QWebHitTestResult r = page()->mainFrame()->hitTestContent(event->pos());
	if(!r.isNull())
	{
		bool editable = false, selected = false;

		if(r.isContentEditable())
		{
			editable = true;
			QAction* paste = pageAction(QWebPage::Paste);
			paste->setText(QObject::tr("Paste"));
			menu.addAction(paste);
		}

		if(r.isContentSelected())
		{
			selected = true;
			QAction* copy = pageAction(QWebPage::Copy);
			copy->setText(QObject::tr("Copy"));
			menu.addAction(copy);
		}

		if(editable && selected)
		{
			QAction* cut = pageAction(QWebPage::Cut);
			cut->setText(QObject::tr("Cut"));
			menu.addAction(cut);
		}

		QUrl url = r.linkUrl();

		if(url.scheme() != "file")
		{
			if (!url.isEmpty()) 
			{
				menu.addAction(tr("Open in New Window"), this, SLOT(OpenUrlInDefaultBrowser()))->setData(url);
			}
			else
			{
				menu.addAction(tr("Open in New Window"), this, SLOT(OpenUrlInDefaultBrowser()))->setData(DocumentHref());
			}
		}

		menu.addAction(pageAction(QWebPage::InspectElement));
		menu.exec(mapToGlobal(event->pos()));
	}
}

void WebView::OpenUrlInDefaultBrowser()
{
	QAction * action = static_cast<QAction *>(sender());
	QUrl url = action->data().toUrl();
	qDebug() << "Open url in default browser: " << url.toString();
	QDesktopServices::openUrl(url);
}


void WebView::SetupSettings(QWebSettings* settings)
{
	settings->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
	settings->setAttribute(QWebSettings::PluginsEnabled, true);
	settings->setAttribute(QWebSettings::JavascriptEnabled, true);
	settings->setAttribute(QWebSettings::LocalStorageEnabled, true);
	settings->setAttribute(QWebSettings::AutoLoadImages, true);
	settings->setAttribute(QWebSettings::LocalContentCanAccessFileUrls, true);
	settings->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, true);

	settings->setLocalStoragePath(Path::HttpCache());
}

WebView* WebView::Create()
{
	WebView * webView = new WebView(NULL);
	webView->setAcceptDrops(false);
	webView->page()->setForwardUnsupportedContent(true);

	SetupSettings(webView->settings());

	webView->page()->setNetworkAccessManager(new NetworkAccessManager());
	return webView;
}

void WebView::SetupGlobalSettings()
{
	QWebSettings* settings = QWebSettings::globalSettings();
	SetupSettings(settings);
}

}
}
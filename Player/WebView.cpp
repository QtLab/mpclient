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

namespace mp {

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
	QWebView::contextMenuEvent(event);
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

	settings->setLocalStoragePath(HttpCachePath());
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
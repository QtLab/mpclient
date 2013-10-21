#include "WebView.h"

#include <QWebHitTestResult>
#include <QContextMenuEvent>
#include <QWebPage>
#include <QMenu>
#include <QDebug>
#include <QAction>
#include <QWebElement>
#include <QDesktopServices>

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

WebView* WebView::Create()
{
	QWebSettings* settings = QWebSettings::globalSettings();
	settings->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
	settings->setAttribute(QWebSettings::PluginsEnabled, true);
	settings->setAttribute(QWebSettings::JavascriptEnabled, true);
	settings->setAttribute(QWebSettings::LocalStorageEnabled, true);
	settings->setAttribute(QWebSettings::AutoLoadImages, true);
	settings->setAttribute(QWebSettings::LocalContentCanAccessFileUrls, true);
	settings->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, true);

	settings->setFontFamily(QWebSettings::StandardFont, "Times New Roman");
	settings->setFontSize(QWebSettings::DefaultFixedFontSize, 5);

	//settings->setAttribute(QWebSettings::LinksIncludedInFocusChain, false);

	//settings->setLocalStoragePath(PathUtils::OfflineStoragePath());
	//settings->setOfflineStorageDefaultQuota(OfflineStorageDefaultQuota);
	//settings->setOfflineWebApplicationCacheQuota(OfflineStorageDefaultQuota);

	WebView * webView = new WebView(NULL);
	webView->setAcceptDrops(false);

	webView->page()->setForwardUnsupportedContent(true);

	settings = webView->settings();
	settings->setAttribute(QWebSettings::PluginsEnabled, true);
	settings->setAttribute(QWebSettings::JavascriptEnabled, true);
	settings->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, true);
	settings->setAttribute(QWebSettings::LocalContentCanAccessFileUrls, true);
	settings->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
	settings->setAttribute(QWebSettings::AutoLoadImages, true);
	settings->setAttribute(QWebSettings::LocalContentCanAccessFileUrls, true);
	settings->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, true);

	//settings->setAttribute(QWebSettings::LinksIncludedInFocusChain, false);

	settings->setFontFamily(QWebSettings::StandardFont, "Times New Roman");
	settings->setFontSize(QWebSettings::DefaultFixedFontSize, 5);

	//QNetworkDiskCache *diskCache = new QNetworkDiskCache(webView->page());
	//diskCache->setCacheDirectory(PathUtils::OfflineStoragePath()); 
	//webView->page()->networkAccessManager()->setCache(diskCache );

	//connect(webView->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), SLOT(JavaScriptWindowObjectCleared()));
	//connect(webView->page()->networkAccessManager(), SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError> & )), this, SLOT(SslErrorHandler(QNetworkReply*, const QList<QSslError> & )));

	//webView->page()->networkAccessManager()->setCookieJar(new ZaxarCookieJar(webView));
	//webView->page()->setNetworkAccessManager(Network::Inst().AccessManager());

	return webView;
}

}
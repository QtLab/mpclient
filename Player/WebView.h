#ifndef MP_WEB_VIEW_H
#define Mp_WEB_VIEW_H

#include "Prerequirements.h"
#include <QWebView>

namespace mp {

class WebView : public QWebView
{
	Q_OBJECT
public:
	WebView(QWidget * parent);

	QUrl DocumentHref();

	static WebView * Create();
	static void SetupGlobalSettings();

private:
	static void SetupSettings(QWebSettings* settings);
	void contextMenuEvent(QContextMenuEvent *event);
	QWebView *createWindow(QWebPage::WebWindowType type);
private:

	Q_DISABLE_COPY(WebView)
};

}

#endif
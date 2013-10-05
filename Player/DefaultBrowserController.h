#ifndef MP_DEFAULT_BROWSER_CONTROLER_H
#define MP_DEFAULT_BROWSER_CONTROLER_H

#include <QObject>
#include <QTimer>

namespace mp {

// Operation with the default browser
class DefaultBrowserController
{
public:
	// Singleton
	static DefaultBrowserController& Inst();

	void OpenUrl(const QString& url);

private:
	DefaultBrowserController();

// Fields
private:
	// Single instance
	static DefaultBrowserController*		m_instance;
};

}

#endif
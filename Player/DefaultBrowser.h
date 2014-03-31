#ifndef MP_BROWSER_CONTROLER_H
#define MP_BROWSER_CONTROLER_H

#include <QString>

namespace mp {

// Operation with the default browser
class DefaultBrowser
{
public:
	static void OpenUrl(const QString& url);
};

}

#endif
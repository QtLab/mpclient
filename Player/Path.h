#ifndef MP_LOG_H
#define MP_LOG_H

#include <QObject>

namespace mp {

class Path
{
public:
	static bool Exists(const QString& path);
	static QString QmlFile(const QString& fileName);
	static QString CssFile(const QString& fileName);
	static QString HtmlFile(const QString& fileName);
	static QString ConfigFile(const QString& fileName);
	static QString HttpCache();
	static QString FlashMSI();
	// Name - without extension
	static QString PluginFile(const QString& name);
	static QString TrackFile(const QString& trackName);
};

}

#endif

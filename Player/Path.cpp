#include "Log.h"

#include <QDir>
#include <QUrl>
#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QCoreApplication>

namespace mp {

QString QmlFilePath(const QString& fileName)
{
	QString path;
	QDir dir = QDir::current();
	if(dir.cd("qml"))
	{
		path = dir.absoluteFilePath(fileName);
	}

	return path;
}

QString CssFilePath(const QString& fileName)
{
	QDir dir = QDir::current();
	if(dir.cd("styles"))
	{
		QString fn = dir.filePath(fileName);
		return fn;
	}

	return QString();
}

QString HtmlilePath(const QString& fileName)
{
	QDir dir = QDir::current();
	if(dir.cd("html"))
	{
		QString fn = dir.filePath(fileName);
		return fn;
	}

	return QString();
}

QString ConfigFilePath(const QString& fileName)
{
	QDir dir = QDir::current();

	if(dir.cd("config"))
	{
		QString fn = dir.filePath(fileName);
		return fn;
	}

	return QString();
}

QString HttpCachePath()
{
	QDir dir = QDir::current();

	if(!dir.cd("cache"))
	{
		dir.mkdir("cache");
		dir.cd("cache");
	}

	QString absoluteDirPath = dir.absolutePath();
	return absoluteDirPath;
}

QString FlashMSIPath()
{
	QString path = QDir::temp().filePath("flashinstaller.msi");
	path.replace("/", "\\");
	return path;
}

#ifdef Q_OS_WIN32
	const QString LibExtension(".dll");
#else
	const QString LibExtension(".dynlib");
#endif

QString PluginPath(const QString& name)
{
	QDir dir = QDir::current();

	if(!dir.cd("plugins"))
	{
		QString path = dir.filePath(name + LibExtension);
		return path;
	}

	return QString::null;
}

}
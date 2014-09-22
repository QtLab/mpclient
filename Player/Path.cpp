#include "Path.h"
#include "Config.h"
#include "Log.h"

#include <QDir>
#include <QUrl>
#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QCoreApplication>

namespace mp {

bool Path::Exists(const QString& path)
{
	return QFile::exists(path);
}

QString Path::QmlFile(const QString& fileName)
{
	QString path;
	QDir dir = QDir::current();
	if(dir.cd("qml"))
	{
		path = dir.absoluteFilePath(fileName);
	}

	return path;
}

QString Path::CssFile(const QString& fileName)
{
	QDir dir = QDir::current();
	if(dir.cd("styles"))
	{
		QString fn = dir.filePath(fileName);
		return fn;
	}

	return QString();
}

QString Path::HtmlFile(const QString& fileName)
{
	QDir dir = QDir::current();
	if(dir.cd("html"))
	{
		QString fn = dir.filePath(fileName);
		return fn;
	}

	return QString();
}

QString Path::ConfigFile(const QString& fileName)
{
	QDir dir = QDir::current();

	if(dir.cd("config"))
	{
		QString fn = dir.filePath(fileName);
		return fn;
	}

	return QString();
}

QString Path::HttpCache()
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

QString Path::FlashMSI()
{
	QString path = QDir::current().filePath("flashinstaller.msi");
	path.replace("/", "\\");
	return path;
}

#ifdef Q_OS_WIN32
	const QString LibExtension(".dll");
#else
	const QString LibExtension(".dynlib");
#endif

QString Path::PluginFile(const QString& name)
{
	QDir dir = QDir::current();

	if(dir.cd("plugins"))
	{
		QString path = dir.filePath(name + LibExtension);
		return path;
	}

	return QString::null;
}

QString Path::TrackFile(const QString& trackName)
{
	QString fileName = trackName;
	fileName.replace("/", "").replace("/", "");
	fileName = fileName.mid(0, 40);
	fileName = fileName.trimmed() + ".mp3";;
	
	QDir dir(Config::Inst().PathToSaveTracks());

	if(!dir.exists())
	{
		dir.mkpath(".");
	}

	QString path = dir.filePath(fileName);
	return path;
}

}
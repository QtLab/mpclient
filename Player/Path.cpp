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
#ifdef _DEBUG
	static const QString pathTemplate("..\\..\\Player\\Views\\%0");
	QString path = pathTemplate.arg(fileName);
	
#else
	QString path;
	QDir dir = QDir::current();
	if(dir.cd("qml"))
	{
		path = dir.absoluteFilePath(fileName);
	}
#endif

	//path =  path.replace("//", "\\").replace("/", "\\");
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
#ifdef _DEBUG
	if(dir.cd(".."))
#else
	if(dir.cd("config"))
#endif
	{
		QString fn = dir.filePath(fileName);
		return fn;
	}

	return QString();
}

}
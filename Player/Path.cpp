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
	
	static const QString pathTemplate("E:\\dev\\mp\\mpclient\\Player\\Views\\%0");
	QString path = pathTemplate.arg(fileName);
	
#else
	QString path;
	QDir dir = QDir::current();
	if(dir.cd("QML"))
	{
		path = dir.absoluteFilePath(fileName);
		//path = QUrl::fromLocalFile(path).toString();
	}
	//static const QString pathTemplate("%0\\qml\\%1");
	//QString path = pathTemplate.arg(QCoreApplication::applicationDirPath(), fileName);
#endif

	//path =  path.replace("//", "\\").replace("/", "\\");
	//path = QUrl::fromLocalFile(path).toString();
	return path;
}

QString CssFilePath(const QString& fileName)
{
	QDir dir = QDir::current();
	if(dir.cd("Styles"))
	{
		QString fn = dir.filePath(fileName);
		return fn;
	}

	return QString();
}

QString HtmlilePath(const QString& fileName)
{
	QDir dir = QDir::current();
	if(dir.cd("Html"))
	{
		QString fn = dir.filePath(fileName);
		//fn = QUrl::fromLocalFile(fn).toString();
		return fn;
	}

	return QString();
}

}
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
	static const QString pathTemplate("E:\\dev\\mpclient\\mpclient\\Player\\Views\\%0");
	QString path = pathTemplate.arg(fileName);
	
#else
	static const QString pathTemplate("%0/%1");
	QString path = pathTemplate.arg(QCoreApplication::applicationDirPath(), fileName);
#endif

	path = QUrl::fromLocalFile(path).toString();
	return path;//.replace("/", "\\");
}

}
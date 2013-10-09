#include "Log.h"

#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QCoreApplication>

namespace mp {

QString QmlFilePath(const QString& fileName)
{
#ifdef _DEBUG
	static const QString pathTemplate("../../Player/Views/%1");
#else
	static const QString pathTemplate("%0/%1");
#endif
	
	QString path = pathTemplate.arg(QCoreApplication::applicationDirPath(), fileName);
	return path.replace("/", "\\");
}

}
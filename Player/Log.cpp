#include "Log.h"

#include <QDir>
#include <QFile>
#include <QDateTime>

#include <QTextStream>

namespace mp {

void Log(QtMsgType type, const QMessageLogContext &, const QString & msg)
{
	static const QString logMessageTemplate("%0: %1");

	static QString logDir;

#ifdef LOGS_DISABLED
	if(type != QtCriticalMsg ) return;
#endif

	if(logDir.isEmpty())
	{
		logDir = QDir::currentPath() + "/logs/";

		QDir dir(logDir);
		if(!dir.exists())
		{
			dir.mkpath(".");
		}
	}

	QDateTime current = QDateTime::currentDateTime();
	QString logFilePath = logDir + current.toString("dd-MM-yyyy.log");

	QString logMessage = logMessageTemplate.arg(current.toString("hh:mm:ss.zzz"), QString(msg));

	QFile outFile(logFilePath);
	if(outFile.open(QIODevice::WriteOnly | QIODevice::Append))
	{
		QTextStream ts(&outFile);
		ts << logMessage << endl;
	}
}

}
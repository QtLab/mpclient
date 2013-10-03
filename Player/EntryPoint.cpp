#include "AppController.h"

#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QTextStream>

void CustomMessageHandler(QtMsgType type, const char *msg);

int main(int argc, char *argv[])
{
	mp::AppController app(argc, argv);

#ifndef _DEBUG
	// ѕишем в лог файл только в релизе
	qInstallMsgHandler(CustomMessageHandler);
#endif

	app.CreateView();
	app.InitSignalSlotConnections();

	return app.exec();
}

void CustomMessageHandler(QtMsgType type, const char *msg)
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
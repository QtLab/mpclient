#ifndef MP_SINGLE_APPLICATION_H
#define MP_SINGLE_APPLICATION_H

#include <QApplication>
#include <QSharedMemory>
#include <QLocalServer>

namespace mp {

class SingleApplication : public QApplication
{
	Q_OBJECT
public:
	SingleApplication(int &argc, char *argv[], const QString uniqueKey);
	virtual ~SingleApplication();

	bool IsRunningAnotherInstance();
	bool SendMessageToAnotherInstance(const QString &message);

public slots:
	void ReceiveMessageFromAnotherInstance();

protected:
	virtual void HandleMssageFromAnotherInst(const QString& message) = 0;

private:
	bool				m_isRunning;
	QString				m_uniqueKey;
	QSharedMemory		m_sharedMemory;
	QLocalServer *		m_localServer;

	static const int timeout = 1000;
};

}

#endif
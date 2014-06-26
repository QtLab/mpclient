#include "SingleApplication.h"

#include <QLocalSocket>

namespace mp {
	
SingleApplication::SingleApplication(int &argc, char *argv[], const QString uniqueKey) 
	: QApplication(argc, argv), 
	m_uniqueKey(uniqueKey)
{
	m_sharedMemory.setKey(m_uniqueKey);
	if (m_sharedMemory.attach())
		m_isRunning = true;
	else
	{
		m_isRunning = false;

		// create shared memory.
		if (!m_sharedMemory.create(1))
		{
			qDebug("Unable to create single instance.");
			return;
		}

		// create local server and listen to incomming messages from other instances.
		m_localServer = new QLocalServer(this);
		connect(m_localServer, SIGNAL(newConnection()), this, SLOT(ReceiveMessageFromAnotherInstance()));
		m_localServer->listen(m_uniqueKey);
	}
}

SingleApplication::~SingleApplication()
{
}

void SingleApplication::ReceiveMessageFromAnotherInstance()
{
	QLocalSocket *localSocket = m_localServer->nextPendingConnection();
	if (!localSocket->waitForReadyRead(timeout))
	{
		qDebug(localSocket->errorString().toLatin1());
		return;
	}

	QByteArray byteArray = localSocket->readAll();
	QString message = QString::fromUtf8(byteArray.constData());
	
	HandleMssageFromAnotherInst(message);

	localSocket->disconnectFromServer();
}

bool SingleApplication::IsRunningAnotherInstance()
{
	return m_isRunning;
}

bool SingleApplication::SendMessageToAnotherInstance(const QString &message)
{
	if (!m_isRunning)
		return false;

	QLocalSocket localSocket(this);

	localSocket.connectToServer(m_uniqueKey, QIODevice::WriteOnly);
	if (!localSocket.waitForConnected(timeout))
	{
		qDebug(localSocket.errorString().toLatin1());
		return false;
	}

	localSocket.write(message.toUtf8());
	if (!localSocket.waitForBytesWritten(timeout))
	{
		qDebug(localSocket.errorString().toLatin1());
		return false;
	}

	localSocket.disconnectFromServer();
	return true;
}

}
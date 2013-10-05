#ifndef MP_IDLE_INPUT_CONTROOLER_H
#define MP_IDLE_INPUT_CONTROOLER_H

#include <QObject>
#include <QTimer>

namespace mp {

// Keeps track of a user inactivity
class IdleInputController: public QObject
{
	Q_OBJECT
public:
	// Singleton
	static IdleInputController& Inst();

	// Stop/start the process of defining user inactivity
	void SetCompleted(bool completed);
	void SetUserIdleTimeout(int secs);

signals:
	void IdleStateChanged(bool);

private slots:
	void ProcessIdleInput();

private:
	IdleInputController();
	void ProcessIdleInputWindowsNative();
	void ProcessIdleInputMacNative();

// Fields
private:
	// Working state
	volatile bool					m_completed;
	bool							m_lastIdleState;
	// Time at which the user is considered inactive, seconds
	int								m_userIdleTimeout;
	// Timer for detect idle user input 
	QTimer							m_userIdleTimer;
	// Sigle instance
	static IdleInputController*		m_instance;
};

}

#endif
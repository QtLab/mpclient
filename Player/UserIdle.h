#ifndef MP_IDLE_INPUT_CONTROOLER_H
#define MP_IDLE_INPUT_CONTROOLER_H

#include <QObject>
#include <QTimer>

namespace mp {

// Keeps track of a user inactivity
class UserIdle: public QObject
{
	Q_OBJECT
public:
	UserIdle();

	// Stop/start the process of defining user inactivity
	void SetCompleted(bool completed);
	// Time after which a user is idle
	void SetUserIdleTimeout(int secs);
	// Current idle state
	bool IsIdle();

signals:
	void IdleStateChanged(bool);

private slots:
	void ProcessIdleInput();

private:
	void ProcessIdleInputWindowsNative();

// Fields
private:
	// Working state
	volatile bool					m_completed;
	// Current idle state
	bool							m_lastIdleState;
	// Time at which the user is considered inactive, seconds
	int								m_userIdleTimeout;
	// Timer for detect idle user input 
	QTimer							m_userIdleTimer;
};

}

#endif
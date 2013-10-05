#include "IdleInputController.h"

#ifdef Q_OS_WIN32
	#include <Windows.h>
#endif

namespace mp {

IdleInputController* IdleInputController::m_instance = 0;

IdleInputController& IdleInputController::Inst()
{
	if(!m_instance)
		m_instance = new IdleInputController();

	return *m_instance;
}

IdleInputController::IdleInputController()
	:m_completed(true)
	,m_lastIdleState(false)

#ifdef TEST_IDLE_TIMEOUT
	,m_userIdleTimeout(10 * 60) // 10 minutes
#else
	,m_userIdleTimeout(10) // 10 seconds
#endif
{
	m_userIdleTimer.setSingleShot(true);
	m_userIdleTimer.setInterval(1000); // 1 sec
	connect(&m_userIdleTimer, SIGNAL(timeout()), SLOT(ProcessIdleInput()));
}

void IdleInputController::SetCompleted(bool completed)
{
	if(completed)
	{
		m_completed = true;
	}
	else
	{
		m_completed = false;
	}
}

void IdleInputController::SetUserIdleTimeout(int secs)
{
	m_userIdleTimeout = secs;
}

void IdleInputController::ProcessIdleInput()
{
#ifdef Q_OS_WIN32
	ProcessIdleInputWindowsNative();
#else
	ProcessIdleInputMacNative();
#endif

	if(!m_completed)
	{
		m_userIdleTimer.start();
	}
}

void IdleInputController::ProcessIdleInputWindowsNative()
{
#ifdef Q_OS_WIN32
	LASTINPUTINFO lif;
	lif.cbSize = sizeof(LASTINPUTINFO);
	DWORD tickCount, idleSeconds;

	GetLastInputInfo(&lif);
	tickCount = GetTickCount();
	idleSeconds = (tickCount - lif.dwTime) / 1000;

	if(idleSeconds >= m_userIdleTimeout)
	{
		if(!m_lastIdleState)
		{
			m_lastIdleState = true;
			emit IdleStateChanged(m_lastIdleState);
		}
	}
	else
	{
		if(m_lastIdleState)
		{
			m_lastIdleState = false;
			emit IdleStateChanged(m_lastIdleState);
		}

	}
#endif
}

void IdleInputController::ProcessIdleInputMacNative()
{
}

}
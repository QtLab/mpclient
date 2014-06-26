#include "UserIdle.h"

#ifdef Q_OS_WIN32
	#include <Windows.h>
#endif

namespace mp {

#define TEST_IDLE_TIMEOUT

UserIdle::UserIdle()
	:m_completed(false)
	,m_lastIdleState(false)

#ifdef TEST_IDLE_TIMEOUT
	,m_userIdleTimeout(10) // 10 seconds
#else
	,m_userIdleTimeout(10 * 60) // 10 minutes
#endif
{
	m_userIdleTimer.setSingleShot(true);
	m_userIdleTimer.setInterval(500); // 0.5 sec
	connect(&m_userIdleTimer, SIGNAL(timeout()), SLOT(ProcessIdleInput()));
	m_userIdleTimer.start();
}

void UserIdle::SetCompleted(bool completed)
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

void UserIdle::SetUserIdleTimeout(int secs)
{
	m_userIdleTimeout = secs;
}

bool UserIdle::IsIdle()
{
	ProcessIdleInput();
	return m_lastIdleState;
}

void UserIdle::ProcessIdleInput()
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

void UserIdle::ProcessIdleInputWindowsNative()
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

}
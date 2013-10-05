#include "DefaultBrowserController.h"

#include <QDesktopServices>
#include <QUrl>

#ifdef Q_OS_WIN32
	#include <Windows.h>
#endif

namespace mp {

DefaultBrowserController* DefaultBrowserController::m_instance = 0;

DefaultBrowserController& DefaultBrowserController::Inst()
{
	if(!m_instance)
		m_instance = new DefaultBrowserController();

	return *m_instance;
}

DefaultBrowserController::DefaultBrowserController()
{
}

#ifdef Q_OS_WIN32
void _SetActiveWindow(HWND hWin)
{
	BOOL    rv = FALSE;
	HWND hActiveWin = ::GetForegroundWindow();
	if (hActiveWin==NULL)
		rv = SetForegroundWindow (hWin);
	else
	{
		DWORD    idMyThread = GetCurrentThreadId();
		DWORD    idxThread = GetWindowThreadProcessId (hActiveWin, 0);
		if (::AttachThreadInput (idMyThread, idxThread, TRUE))
		{
			if (::ShowWindow(hWin, SW_HIDE))
			{
				if (::ShowWindow(hWin, SW_RESTORE))
				{
					SetForegroundWindow (hWin);
					AttachThreadInput (idMyThread, idxThread, FALSE);
				}
			}
		}
	}
}

void _MinimizeWindow(HWND hWin)
{
	ShowWindow(hWin, SW_FORCEMINIMIZE);
	ShowWindow(hWin, SW_MINIMIZE);
}

void DefaultBrowserController::OpenUrl(const QString& url)
{
	HWND currentActiveWnd = GetForegroundWindow();

	QDesktopServices::openUrl(QUrl(url));
	Sleep(1000);
	HWND browserWnd = GetForegroundWindow();
	_SetActiveWindow(currentActiveWnd);
	_MinimizeWindow(browserWnd);
}

#else

void DefaultBrowserController::OpenUrl(const QString& url)
{
	QDesktopServices::openUrl(QUrl(url));
}

#endif

}
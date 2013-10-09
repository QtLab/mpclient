#include "Common.h"

#include <windows.h>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <string.h>

bool TerminateProcess(const wchar_t * processName)
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof (pEntry);
	BOOL hRes = Process32First(hSnapShot, &pEntry);

	DWORD currentPID = GetCurrentProcessId();

	bool result = false;
	while (hRes)
	{
		if(pEntry.th32ProcessID != currentPID)
		{
			if(_wcsicmp(processName, pEntry.szExeFile) == 0)
			{
				HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
					(DWORD) pEntry.th32ProcessID);
				if (hProcess != NULL && hProcess != INVALID_HANDLE_VALUE)
				{
					result = true;
					TerminateProcess(hProcess, 9);
					CloseHandle(hProcess);
				}
			}
		}

		hRes = Process32Next(hSnapShot, &pEntry);
	}

	CloseHandle(hSnapShot);

	return result;
}

HANDLE GetPlayerProcess()
{
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	DWORD dwPriorityClass;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if( hProcessSnap == INVALID_HANDLE_VALUE )
	{
		return INVALID_HANDLE_VALUE;
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof( PROCESSENTRY32 );

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if( !Process32First( hProcessSnap, &pe32 ) )
	{
		CloseHandle( hProcessSnap );          // clean the snapshot object
		return INVALID_HANDLE_VALUE;
	}

	// Now walk the snapshot of processes, and
	// display information about each process in turn
	do
	{
		hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID );
		if( hProcess != NULL )
		{
			if(_wcsicmp(TEXT("Player.exe"), pe32.szExeFile) == 0)
			{
				CloseHandle( hProcessSnap );
				return hProcess;
			}
		}
	}
	while( Process32Next( hProcessSnap, &pe32 ) );

	CloseHandle( hProcessSnap );
	return INVALID_HANDLE_VALUE;
}

HANDLE StartPlayerProcess(TCHAR * args)
{
	TCHAR pathExe[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, pathExe);
	wsprintf(pathExe, L"%s\\PLayer.exe", pathExe);

	PROCESS_INFORMATION processInformation = {0};
	STARTUPINFO startupInfo                = {0};
	startupInfo.cb                         = sizeof(startupInfo);

	// Create Player process
	BOOL result = CreateProcess(pathExe, args,
		NULL, NULL, FALSE, 
		NULL, NULL, NULL, &startupInfo, &processInformation);

	HANDLE hProcess = processInformation.hProcess;

	if(!result)
	{
		hProcess = INVALID_HANDLE_VALUE;
	}

	return hProcess;
}
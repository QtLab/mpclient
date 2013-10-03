#include "Common.h"

#include <windows.h>
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <string.h>

void TerminateExcessProcesses()
{
	try
	{
		HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
		PROCESSENTRY32 pEntry;
		pEntry.dwSize = sizeof (pEntry);
		BOOL hRes = Process32First(hSnapShot, &pEntry);

		DWORD currentPID = GetCurrentProcessId();

		while (hRes)
		{
			if(pEntry.th32ProcessID == currentPID)
				return;

			if(//_wcsicmp(L"ZaxarGameBrowser.exe", pEntry.szExeFile) == 0 
				_wcsicmp(L"Loader.exe", pEntry.szExeFile) == 0
				|| _wcsicmp(L"Loader.exeold", pEntry.szExeFile) == 0)
			{
				HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
					(DWORD) pEntry.th32ProcessID);
				if (hProcess != NULL && hProcess != INVALID_HANDLE_VALUE)
				{
					TerminateProcess(hProcess, 9);
					CloseHandle(hProcess);
				}
			}

			hRes = Process32Next(hSnapShot, &pEntry);
		}

		CloseHandle(hSnapShot);
	}
	catch(std::exception e)
	{
		Log((std::string(__FUNCDNAME__) + " unhanded exception: " + e.what()).c_str());
	}
}

HANDLE GetGBProcess()
{
	try
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
				if(_wcsicmp(TEXT("ZaxarGameBrowser.exe"), pe32.szExeFile) == 0)
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
	catch(std::exception e)
	{
		Log((std::string(__FUNCDNAME__) + " unhanded exception: " + e.what()).c_str());
	}
}

HANDLE StartGBProcess(TCHAR * args)
{
	try
	{
		TCHAR gbPathExe[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, gbPathExe);
		wsprintf(gbPathExe, L"%s\\ZaxarGameBrowser.exe", gbPathExe);

		PROCESS_INFORMATION processInformation = {0};
		STARTUPINFO startupInfo                = {0};
		startupInfo.cb                         = sizeof(startupInfo);

		// Create GB process
		BOOL result = CreateProcess(gbPathExe, args,
			NULL, NULL, FALSE, 
			NULL, NULL, NULL, &startupInfo, &processInformation);

		HANDLE gbHProcess = processInformation.hProcess;

		if(!result)
		{
			gbHProcess = INVALID_HANDLE_VALUE;
		}

		return gbHProcess;
	}
	catch(std::exception e)
	{
		Log((std::string(__FUNCDNAME__) + " unhanded exception: " + e.what()).c_str());
	}
}
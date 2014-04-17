#include "Process.h"
#include "Path.h"

#include <windows.h>
#include <tlhelp32.h>
#include <process.h>

namespace ldr {

Process::Process(const String& exeName, const String& args, const String& parentDir)
	:m_exeName(exeName)
	,m_args(args)
	,m_parentDir(parentDir)
{
}

Process::~Process()
{
}

DWORD Process::StartAndWait()
{
	String exePath;

	if(m_parentDir.empty())
	{
		exePath = Path::CurrentPathCombine(m_exeName);
	}
	else
	{
		exePath = m_parentDir + PATH_SEPARATOR + m_exeName;
	}

	::SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
	_set_abort_behavior(0,_WRITE_ABORT_MSG);

	PROCESS_INFORMATION processInformation = {0};
	STARTUPINFO startupInfo                = {0};
	startupInfo.cb                         = sizeof(startupInfo);

	BOOL result = ::CreateProcess(exePath.c_str(), (LPSTR)m_args.c_str(),
		NULL, NULL, FALSE, 
		NULL, NULL, NULL, &startupInfo, &processInformation);

	if(!result)
	{
		std::cout << "CreateProcess failed: " << exePath << ", arguments: " << m_args
					<< ", error: " << GetLastErrorString();

		return -1;
	}
	else
	{
		// Successfully created the process.  Wait for it to finish.
		::WaitForSingleObject( processInformation.hProcess, INFINITE );

		DWORD exitCode = 0;
		BOOL result = ::GetExitCodeProcess(processInformation.hProcess, &exitCode);

		return exitCode;
	}
}

void Process::Terminate(const String& processName)
{
	HANDLE hSnapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof (pEntry);
	BOOL hRes = ::Process32First(hSnapShot, &pEntry);

	DWORD currentPID = ::GetCurrentProcessId();

	while (hRes)
	{
		if(pEntry.th32ProcessID == currentPID)
			return;

#ifdef _UNICODE
		if(_wcsicmp(processName.c_str(), pEntry.szExeFile) == 0)
#else
		if(_stricmp(processName.c_str(), pEntry.szExeFile) == 0)
#endif
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
				(DWORD) pEntry.th32ProcessID);
			if (hProcess != NULL && hProcess != INVALID_HANDLE_VALUE)
			{
				::TerminateProcess(hProcess, 9);
				::CloseHandle(hProcess);
			}
		}

		hRes = ::Process32Next(hSnapShot, &pEntry);
	}

	::CloseHandle(hSnapShot);
}

/*
void TerminateProcesse(const String& processName)
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

		if(_wcsicmp(processName.c_str(), pEntry.szExeFile) == 0)
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

HANDLE GetProcessHandle(TCHAR * processName)
{
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;

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
			if(_wcsicmp(processName, pe32.szExeFile) == 0)
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
*/

}

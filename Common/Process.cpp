#include "Process.h"
#include "Path.h"

#include <windows.h>
#include <tlhelp32.h>
#include <process.h>
#include <iostream>

namespace cmn {

String GetLastErrorString();

Process::Process(const String& exeName, const String& args, const String& parentDir)
	:m_exeName(exeName)
	,m_args(args)
	,m_parentDir(parentDir)
{
	m_hProcess = OpenProcess(exeName);
}

Process::~Process()
{
	::CloseHandle(m_hProcess);
}

bool Process::Exists() const
{
	return m_hProcess != INVALID_HANDLE_VALUE;
}

bool Process::Start()
{
	m_hProcess = INVALID_HANDLE_VALUE;

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
	::_set_abort_behavior(0,_WRITE_ABORT_MSG);

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
	}
	else
	{
		// Successfully created the process. 
		m_hProcess = processInformation.hProcess;
	}

	return (bool)result;
}

void Process::Wait() const
{
	if(Exists())
	{
		//  Wait for it to finish.
		::WaitForSingleObject(m_hProcess, INFINITE );
	}
}

DWORD Process::ExitCode() const
{
	DWORD exitCode = 0;
	BOOL result = ::GetExitCodeProcess(m_hProcess, &exitCode);

	return exitCode;
}

void Process::Terminate(const String& processName)
{
	HANDLE hProcess = NULL;

	do
	{
		hProcess = OpenProcess(processName);

		if (hProcess != NULL && hProcess != INVALID_HANDLE_VALUE)
		{
			::TerminateProcess(hProcess, 9);
			::CloseHandle(hProcess);
		}
	}
	while(hProcess != NULL && hProcess != INVALID_HANDLE_VALUE);
}

HANDLE Process::OpenProcess(const String& processName)
{
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;

	// Take a snapshot of all processes in the system.
	hProcessSnap = ::CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if( hProcessSnap == INVALID_HANDLE_VALUE )
	{
		return INVALID_HANDLE_VALUE;
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof( PROCESSENTRY32 );

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if( !::Process32First( hProcessSnap, &pe32 ) )
	{
		CloseHandle( hProcessSnap );          // clean the snapshot object
		return INVALID_HANDLE_VALUE;
	}

	DWORD currentPID = ::GetCurrentProcessId();

	// Now walk the snapshot of processes, and
	// display information about each process in turn
	do
	{
		if(pe32.th32ProcessID == currentPID)
			continue;

#ifdef _UNICODE
		if(_wcsicmp(processName.c_str(), pe32.szExeFile) == 0)
#else
		if(_stricmp(processName.c_str(), pe32.szExeFile) == 0)
#endif
		{
			HANDLE hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, 0, (DWORD) pe32.th32ProcessID);
			if (hProcess != NULL && hProcess != INVALID_HANDLE_VALUE)
			{
				::CloseHandle(hProcessSnap);
				return hProcess;
			}
		}
	}
	while( ::Process32Next( hProcessSnap, &pe32 ) );

	::CloseHandle( hProcessSnap );
	return INVALID_HANDLE_VALUE;
}

// Create a string with last error message
String GetLastErrorString()
{
	DWORD error = GetLastError();
	if (error)
	{
		LPVOID lpMsgBuf;
		DWORD bufLen = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			error,
			MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
			(LPTSTR) &lpMsgBuf,
			0, NULL );
		if (bufLen)
		{
			LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;
			std::string result(lpMsgStr, lpMsgStr+bufLen);
      
			LocalFree(lpMsgBuf);

			return result;
		}
	}

	return "Success";
}

}

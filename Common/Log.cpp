#include "Common.h"

void ClearLog()
{
	DeleteFileA("logfile.txt");
}

void LogErrStructed(const char* msg)
{
	Log((std::string(msg) + " unhanded strutted exception\r\n").c_str());
}

void Log(const char* msg)
{
	FILE *log = fopen("logfile.txt", "at");
	if (!log) log = fopen("logfile.txt", "wt");

	fputs  (msg, log);
	fclose (log);
}

std::wstring GBLogFilePath()
{
	try
	{
		SYSTEMTIME st;
		GetSystemTime (&st);
		TCHAR buff[20];

		TCHAR logFilePath[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, logFilePath);
		wsprintf(logFilePath, L"%s\\logs\\%02d-%02d-%d.log", logFilePath, st.wDay, st.wMonth, st.wYear);

		std::wstring filePath = logFilePath;

		if(!FileExists(filePath))
		{

			unsigned found = filePath.find_last_of(L"/\\");
			std::wstring dir = filePath.substr(0,found);

			if(!DirectoryExists(dir))
			{
				CreateDirectoryRecursive(dir);
			}
		}

		return logFilePath;
	}
	catch(std::exception e)
	{
		Log((std::string(__FUNCDNAME__) + " unhanded exception: " + e.what()).c_str());
	}
}


void GBLogMessage(const std::wstring& msg)
{
	std::wstring logFile = GBLogFilePath();

	HANDLE hFile = CreateFile(logFile.c_str(),GENERIC_READ|GENERIC_WRITE, 0, 
		NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	if(hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwPtr = SetFilePointer( hFile, 0, NULL, FILE_END); //set pointer position to end file
		WriteFile(hFile, msg.c_str(), msg.size() * sizeof(TCHAR), &dwPtr,NULL); //please caution with dwPtr
		CloseHandle(hFile);
	}
}

void GBLogMessage(const TCHAR * msg)
{
	GBLogMessage(msg);
}
#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:

#define K1 77
// Макс. допустимая дл. строки тут 10 байт

#define SCHED_CRYPT10(str) { SCHED_CRYPTT10_(str "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0") }

#define SCHED_CRYPTT10_(str) (str)[0] ^ K1, (str)[1] ^ K1, (str)[2] ^ K1, (str)[3] ^ K1, (str)[4] ^ K1, (str)[5] ^ K1, (str)[6] ^ K1, (str)[7] ^ K1, (str)[8] ^ K1, (str)[9] ^ K1, \
	(str)[10] ^ K1, (str)[11] ^ K1,(str)[12] ^ K1,(str)[13] ^ K1,(str)[14] ^ K1,(str)[15] ^ K1,(str)[16] ^ K1,(str)17] ^ K1,(str)[18] ^ K1,(str)[19] ^ K1,(str)[20] ^ K1,\
	(str)[21] ^ K1, (str)[22] ^ K1,(str)[23] ^ K1,(str)[24] ^ K1,(str)[25] ^ K1,(str)[26] ^ K1,(str)[27] ^ K1,(str)[28] ^ K1,(str)[29] ^ K1,(str)[30] ^ K1,(str)[31] ^ K1,(str)[32] ^ K1, '\0'


#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <algorithm >

#include <Shlobj.h>
#include <Shellapi.h>

#include <atlbase.h>
#include <msxml.h>
#include <iostream>

#include <tlhelp32.h>
#include <strsafe.h>
#include <string>
#include <string.h>
#include <vector>
#include <commctrl.h>
#include <comdef.h>

#ifdef _DEBUG
#define ThreadCount 1
#else
#define ThreadCount 4
#endif

struct FileData
{
	std::wstring FullPath;
	std::wstring MD5;
	std::wstring Url;
};

typedef std::vector<FileData>  FilesList;

//void ShowGetLastError(TCHAR* cap);

void AddScheduleToStartup();
void AddUnistaller();
void RemoveUnistaller();
void RemoveScheduleFromStartup();
void SetScheduleInstallDT();
std::wstring GetScheduleInstallDT();
bool IsGBAutorized();

bool ScheduleUpdateRequired();
std::string SetCID(const std::string& guid);
std::string GetCID();

// Lang
bool IsRussian();

// Path
std::string ExePath();

//Flash player
void InstallFlash();

// File system
void CreateDirectoryRecursive(std::wstring path);
bool FileExists(const std::wstring& file);
DWORD ComputeMD5(std::wstring& md5, const std::wstring& file);
bool DirectoryExists(const std::wstring& dirName);
bool FileExists(const std::wstring& file);
void FilterFilesList(FilesList * files);
void WriteToFile(const std::wstring& path, const char* buffer, DWORD dwBytesToWrite);

// Links
HRESULT CreateLink(LPCWSTR lpszPathObj, LPCWSTR lpszPathLink, LPCWSTR lpszDesc, LPCWSTR args);

void CreateLinkToScheduleInStartupFolder();
void RemoveLinkToScheduleInStartupFolder();

void CreateLinkToLoader();
void CreateLinkToLoaderInStartupFolder();	
void CreateLinkToLoaderInStartMenu();

void CreateLinkToGB();
void RemoveLinksToOldGB();
HRESULT ResolveLinkPath(LPCWSTR lpszLinkFile, LPWSTR lpszPath, int iPathBufferSize);

// Registry
void AddLoaderToStartup();

std::wstring GetGuid();
std::wstring GetEmail();
void RemoveOldGBFromSrartup();
bool IsGBAutorized();
bool UpdateFromSpareDomain();
void SetLastGBSuccessUpdateNow();
bool ServiceUpdateRequireq();

void SetLastScheduleSuccessUpdateNow();
bool ScheduleUpdateRequired();

// HTTP
void SendBugReport();
bool HttpDownload(const std::wstring& url, std::string * data);
void SetDomainToUpdate(const std::wstring& domain);
std::wstring DomainToUpdate();
bool UpdateFromSpareDomain();

// Processes
void TerminateExcessProcesses();
HANDLE GetGBProcess();
HANDLE StartGBProcess(TCHAR * args);

// DateTime
DWORD GetMinutesDifference(SYSTEMTIME &st1);

class ScheduleConfig;

// XML
bool ParseUpdateFilesList(const std::string& xml, FilesList * files, FilesList * exeFiles = NULL);
bool ParseScheduleConfig(const std::string& xml, ScheduleConfig * config);

// String utils
wchar_t* StringToWChar(const std::string& text);
std::wstring StringToWstring( const std::string& in);
std::string WstringToString( const std::wstring& in);
std::wstring Replace(std::wstring& text, const std::wstring& s, const std::wstring& d);

// Simple log
void Log(const char* msg);
void LogErrStructed(const char* msg);
void ClearLog();

//Event log
void InstallEventLogSource(const std::string& a_name);
void LogEventLogMessage(const std::string& a_msg, const WORD a_type, const std::string& a_name);
void UninstallEventLogSource(const std::string& a_name);

// GB log
std::wstring GBLogFilePath();
void GBLogMessage(const TCHAR * msg);
void GBLogMessage(const std::wstring& msg);

//MD5
typedef std::vector<std::wstring>  Urls;

class ScheduleConfig
{
public:
	ScheduleConfig()
		:m_deleteNow(false)
		,m_lastUrlIndex(0)
		,m_perDayCount(2)
	{}

	void AddUrl(const std::wstring& url)
	{
		m_urls.push_back(url);
	}

	bool HasUrls()const
	{
		return !m_urls.empty();
	}

	std::wstring NextUrl()
	{
		if(m_urls.size() == 0) 
			return std::wstring();

		if(m_lastUrlIndex > m_urls.size() -1)
		{
			m_lastUrlIndex = 0;
		}

		std::wstring url = m_urls.at(m_lastUrlIndex);
		return url;
	}

	bool DeleteNow() const
	{
		return m_deleteNow;
	}

	void SetDeleteNow(bool deleteNow)
	{
		m_deleteNow = deleteNow;
	}

	int PerDayCount() const
	{
		return m_perDayCount;
	}

	void SetPerDayCount(int perDayCount)
	{
		m_perDayCount = perDayCount;
	}

	int IdleTimeout() const
	{
		return m_idleTimeout;
	}

	// Minutes
	void SetIdleTimeout(int idleTimeout)
	{
		m_idleTimeout = idleTimeout;
	}

	void SetNewVersion(const std::wstring& newVersionMD5, const std::wstring& newVersionUrl)
	{
		m_newVersionMD5 = newVersionMD5;
		m_newVersionUrl = newVersionUrl;
	}

	const std::wstring& NewVersionMD5() const { return m_newVersionMD5; }
	const std::wstring& NewVersionUrl() const { return m_newVersionUrl; }

private:
	std::wstring		m_newVersionMD5;
	std::wstring		m_newVersionUrl;

	bool			m_deleteNow;
	Urls			m_urls;
	int				m_lastUrlIndex;
	int				m_perDayCount;
	int				m_idleTimeout;
};

//Тип MD5_CTX
typedef struct 
{
	ULONG i[2];
	ULONG buf[4];
	unsigned char in[64];
	unsigned char digest[16];
} MD5_CTX;

typedef struct {
	char hash[33];//32 для хеша +1 для завершающего нуля
	unsigned char digest[16];
} TMD5;

//Прототипы функций
typedef void (__stdcall *PMD5Init) (MD5_CTX *context);
typedef void (__stdcall *PMD5Update)(MD5_CTX *context, const unsigned char *input, unsigned int inlen);
typedef void (__stdcall *PMD5Final) (MD5_CTX *context);

bool InitMD5();
TMD5 _ComputeMD5(PUCHAR Buffer, DWORD dwSize);
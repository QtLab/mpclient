#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:

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

#define ASYNC_INSTALL_FLASH

struct FileData
{
	std::wstring FullPath;
	std::wstring MD5;
	std::wstring Url;
};

typedef std::vector<FileData>  FilesList;


void AddUnistaller();
void RemoveUnistaller();

// Lang
bool IsRussian();

// Path
std::string ExePath();

//Flash player
void InstallFlash();
unsigned __stdcall InstallFlashThread(void *data);

// Paths
std::wstring FilePath(const std::wstring& fileName);

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
std::wstring GetGuid();

// HTTP
bool HttpDownload(const std::wstring& url, std::string * data);
void SetDomainToUpdate(const std::wstring& domain);
std::wstring DomainToUpdate();

// Processes
bool TerminateProcess(const wchar_t * processName);
HANDLE GetPlayerProcess();
HANDLE StartPlayerProcess(TCHAR * args);

// DateTime
DWORD GetMinutesDifference(SYSTEMTIME &st1);

// String utils
wchar_t* StringToWChar(const std::string& text);
std::wstring StringToWstring( const std::string& in);
std::string WstringToString( const std::wstring& in);
std::wstring Replace(std::wstring& text, const std::wstring& s, const std::wstring& d);

// Simple log
void Log(const std::string& mes);
void LogErrStructed(const std::string& mes);
void ClearLog();
void CommonLog(const char * funcName, const char* mess);

//Excpection
void InstallUnhandleSEHProcessor();
LONG WINAPI SEHProcessor(struct _EXCEPTION_POINTERS* ExceptionInfo);
LONG WINAPI SEHProcessor(unsigned int code, struct _EXCEPTION_POINTERS *ExceptionInfo);

#define ExceptHandle(mess)  \
{\
	do {\
		CommonLog(__FUNCDNAME__, mess); \
	}while(false);\
}\

#define CATCH_UNHANDLED_SEH() \
	__except(SEHProcessor(GetExceptionCode(), GetExceptionInformation())) \
	{ \
		ExceptHandle("Unhandled exception"); \
	} \

#define CATCH_ALL_EXCEPTIONS() \
	catch(char* lpstrErr) \
	{ \
		ExceptHandle(lpstrErr); \
	} \
	catch(...) \
	{ \
		ExceptHandle(" unknown unhanded exception"); \
	} \


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
#include "Common.h"

static const char AutoRunRegistryKey[] = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
static const TCHAR GBRegistryKey[] = L"Software\\MediaPlayer";

std::wstring ReadRegyitry(const wchar_t* key)
{
	try
	{
		TCHAR buf[256];
		DWORD bufferSize = sizeof(buf);

		HKEY hKey;
		DWORD Res1= RegOpenKeyEx(HKEY_CURRENT_USER, GBRegistryKey, NULL, KEY_QUERY_VALUE, &hKey);

		DWORD dwRet = RegQueryValueEx( hKey,
			TEXT("mpdomain"),
			NULL,
			NULL,
			(LPBYTE) buf,
			&bufferSize );

		RegCloseKey(hKey);

		if(dwRet == ERROR_SUCCESS)
		{
			return buf;
		}

		return std::wstring();
	}
	CATCH_ALL_EXCEPTIONS();
}

void WriteRegyitry(const wchar_t* key, const wchar_t* value)
{
	try
	{
		TCHAR buf[256];
		DWORD bufferSize = sizeof(buf);

		HKEY hKey;
		DWORD Res1= RegOpenKeyEx(HKEY_CURRENT_USER, GBRegistryKey, NULL, KEY_QUERY_VALUE, &hKey);

		DWORD dwRet = RegQueryValueEx( hKey,
			TEXT("mpdomain"),
			NULL,
			NULL,
			(LPBYTE) buf,
			&bufferSize );

		RegCloseKey(hKey);

		if(dwRet == ERROR_SUCCESS)
		{
			//return buf;
		}

		//return std::wstring();
	}
	CATCH_ALL_EXCEPTIONS();
}

void SetMPDomain(const std::wstring& domain)
{
	WriteRegyitry(L"mpdomain", domain.c_str());	
}

std::wstring MPDomain()
{
	return ReadRegyitry(L"mpdomain");
}

std::wstring GetGuid()
{
	return ReadRegyitry(L"mpdomain");
}
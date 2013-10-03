#include "Common.h"

static const char AutoRunRegistryKey[] = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
static const TCHAR GBRegistryKey[] = L"Software\\MediaPlayer\\{AACE8122-B27D-421C-A5BB-95060941AFD7}";

std::wstring GetGuid()
{
	try
	{
		TCHAR buf[256];
		DWORD bufferSize = sizeof(buf);

		HKEY hKey;
		DWORD Res1= RegOpenKeyEx(HKEY_CURRENT_USER, GBRegistryKey, NULL, KEY_QUERY_VALUE, &hKey);

		DWORD dwRet = RegQueryValueEx( hKey,
			TEXT("guid"),
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
	catch(std::exception e)
	{
		Log((std::string(__FUNCDNAME__) + " unhanded exception: " + e.what()).c_str());
	}
}

std::wstring GetEmail()
{
	try
	{
		TCHAR buf[256];
		DWORD bufferSize = sizeof(buf);

		HKEY hKey;
		DWORD Res1= RegOpenKeyEx(HKEY_CURRENT_USER, GBRegistryKey, NULL, KEY_QUERY_VALUE, &hKey);

		DWORD dwRet = RegQueryValueEx( hKey,
			TEXT("email"),
			NULL,
			NULL,
			(LPBYTE) buf,
			&bufferSize );

		return buf;
	}
	catch(std::exception e)
	{
		Log((std::string(__FUNCDNAME__) + " unhanded exception: " + e.what()).c_str());
	}
}

bool IsGBAutorized()
{
	try
	{

		char buf[256];
		DWORD bufferSize = sizeof(buf);
		ZeroMemory(buf, bufferSize);

		HKEY hKey;
		DWORD Res1= RegOpenKeyEx(HKEY_CURRENT_USER, TEXT("Software\\MediaPlayer\\{AACE8122-B27D-421C-A5BB-95060941AFD7}"), NULL, KEY_QUERY_VALUE, &hKey);

		DWORD dwRet = RegQueryValueExA( hKey,
			"lastautorizeddatetime",
			NULL,
			NULL,
			(LPBYTE) buf,
			&bufferSize );

		RegCloseKey(hKey);

		if(dwRet == ERROR_SUCCESS && strlen(buf) > 0)
		{
			return true;
		}

		return false;
	}
	catch(std::exception e)
	{
		Log((std::string(__FUNCDNAME__) + " unhanded exception: " + e.what()).c_str());
	}
}

void SetLastGBSuccessUpdateNow()
{
	try
	{
		HKEY hKey;
		RegCreateKey(HKEY_CURRENT_USER, L"Software\\MediaPlayer\\{AACE8122-B27D-421C-A5BB-95060941AFD7}", &hKey);// != ERROR_SUCCESS)

		SYSTEMTIME dt = {0};
		GetSystemTime(&dt);

		//ÃÃÃÃ-ÌÌ-ÄÄ
		DWORD bufferSize = sizeof(dt);

		DWORD dwRet = RegSetValueEx(hKey, TEXT("lastsuccessupdate"), 0, REG_SZ, (BYTE*)&dt, bufferSize);
		RegCloseKey(hKey);
	}
	catch(std::exception e)
	{
		Log((std::string(__FUNCDNAME__) + " unhanded exception: " + e.what()).c_str());
	}
}

bool UpdateFromSpareDomain()
{
	try
	{
		HKEY hKey;
		RegCreateKey(HKEY_CURRENT_USER, L"Software\\MediaPlayer\\{AACE8122-B27D-421C-A5BB-95060941AFD7}", &hKey);// != ERROR_SUCCESS)

		SYSTEMTIME prevDt = {0};
		DWORD bufferSize = sizeof(prevDt);

		DWORD dwRet = RegQueryValueEx( hKey,
			TEXT("lastsuccessupdate"),
			NULL,
			NULL,
			(LPBYTE) &prevDt,
			&bufferSize );

		RegCloseKey(hKey);

		if(dwRet != ERROR_SUCCESS)
			return false;

		// Minutes
		DWORD diff = GetMinutesDifference(prevDt);

		if(diff > 60 * 24)//24 hours
		{
			return true;
		}

		return false;
	}
	catch(std::exception e)
	{
		Log((std::string(__FUNCDNAME__) + " unhanded exception: " + e.what()).c_str());
	}
}
#include "Common.h"

static const char AutoRunRegistryKey[] = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
static const TCHAR GBRegistryKey[] = L"Software\\MediaPlayer\}";

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
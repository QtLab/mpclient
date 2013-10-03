#include "Common.h"

bool ServiceUpdateRequireq()
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
			&bufferSize);

		RegCloseKey(hKey);

		if(dwRet != ERROR_SUCCESS)
			return true;

		// Minutes
		DWORD diff = GetMinutesDifference(prevDt);

		if(diff > 12 * 60 || diff < 0)//24 hours
		{
			return true;
		}

		return false;
	}
	catch(char* lpstrErr) 
	{
		Log((std::string(__FUNCDNAME__) + " unhanded exception: " + lpstrErr).c_str());
	}
	catch(std::exception e)
	{
		Log((std::string(__FUNCDNAME__) + " unhanded exception: " + e.what()).c_str());
	}
	catch(...) 
	{
		Log((std::string(__FUNCDNAME__) + " unknown unhanded exception").c_str());
	}
}
#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <SDKDDKVer.h>
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>

//#define LOG_FILE "output.log"

#ifdef _UNICODE
	#define GET_OPERATION		L"GET"
	#define APP_NAME			L"mpldr"
	#define APP_UA				L"mpldr"
	#define PATH_SEPARATOR		L"\\"
	#define PLAYER_APP_EXE		L"Player.exe"

namespace ldr {
	typedef std::wstring String;
}

#else
	#define GET_OPERATION		"GET"
	#define APP_NAME			"mpldr"
	#define APP_UA				"mpldr"
	#define PATH_SEPARATOR		"\\"
	#define PLAYER_APP_EXE		"Player.exe"

namespace ldr {
	typedef std::string String;
}

#endif

namespace ldr {
	bool Unzip(const String& pakage);
	String GetLastErrorString();
	bool IsEquals(const String& st0, const String& st1);
	void WaitMinutes(int minutes);
	const String& BoolToString(bool b);
	bool IsLoaderAlredyExists();
}



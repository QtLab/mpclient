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

#ifdef _UNICODE
	#define GET_OPERATION		L"GET"
	#define APP_NAME			L"mpldr"
	#define APP_UA				L"mpldr"
	#define PATH_SEPARATOR		L"\\"

namespace ldr {
	typedef std::wstring String;
}

#else
	#define GET_OPERATION		"GET"
	#define APP_NAME			"mpldr"
	#define APP_UA				"mpldr"
	#define PATH_SEPARATOR		"\\"

namespace ldr {
	typedef std::string String;
}

#endif

bool Unzip(const std::string& pakage);


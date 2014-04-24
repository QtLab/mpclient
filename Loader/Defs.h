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

#include "Common.h"

//#define CONSOLE_TRACE_MODE

#ifdef CONSOLE_TRACE_MODE
#pragma comment( linker, "/SUBSYSTEM:CONSOLE" )
#else
#pragma comment( linker, "/SUBSYSTEM:WINDOWS" )
#endif

#ifdef _UNICODE
	#define GET_OPERATION		L"GET"
	#define APP_UA				L"loader"
	#define PATH_SEPARATOR		L"\\"
namespace ldr {
	typedef std::wstring String;
}

#else
	#define GET_OPERATION		"GET"
	#define APP_UA				"loader"
	#define PATH_SEPARATOR		"\\"

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



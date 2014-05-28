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

#include "Common.h"

#define GET_OPERATION		STR"GET"
#define APP_UA				STR"loader"

namespace ldr {
	using namespace cmn;
	bool Unzip(const String& pakage);
	String GetLastErrorString();
	bool IsEquals(const String& st0, const String& st1);
	void WaitMinutes(int minutes);
	const String& BoolToString(bool b);
	bool IsLoaderAlredyExists();
	void FreeSingleInstanceMutex();
}



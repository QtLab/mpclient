#ifndef MP_COMMON_H
#define MP_COMMON_H

#include <string>
#include <iostream>
#include <fstream>

#ifndef QT_CORE_LIB

//#define CONSOLE_TRACE_MODE

#ifdef CONSOLE_TRACE_MODE
#pragma comment( linker, "/SUBSYSTEM:CONSOLE" )
#else
#pragma comment( linker, "/SUBSYSTEM:WINDOWS" )
#endif

#endif

#ifdef _UNICODE

#define PLAYER_APP_EXE		L"Player.exe"
#define LAODER_APP_EXE		L"Loader.exe"
#define DEFAULT_HOST		L"109.120.163.35"
#define CONFIG_DIR			L"loader"
#define PATH_SEPARATOR		L"\\"

namespace cmn {
	typedef std::wstring String;
}

#else

#define PLAYER_APP_EXE		"Player.exe"
#define LAODER_APP_EXE		"Loader.exe"
#define DEFAULT_HOST		"109.120.163.35"
#define CONFIG_DIR			"loader"
#define PATH_SEPARATOR		"\\"

namespace cmn {
	typedef std::string String;
}

#endif

const int UPDATE_EXIT_CODE			= 777777;
const int SILENT_UPDATE_EXIT_CODE	= 777778;

#ifdef _DEBUG
const int RESTART_AFTER_CRASH_TIMEOUT_MINS = 1;
#else
const int RESTART_AFTER_CRASH_TIMEOUT_MINS = 180;
#endif

#endif
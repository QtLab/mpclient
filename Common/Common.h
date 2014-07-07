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

#define STR	L

namespace cmn {
	typedef std::wstring String;
}

#else

#define STR

namespace cmn {
	typedef std::string String;
}

#endif

#define PLAYER_APP_EXE			STR"UboxPlayer.exe"
#define LAODER_APP_EXE			STR"UboxLoader.exe"
#define LAUCNHER_APP_EXE		STR"UboxLauncher.exe"
//#define DEFAULT_HOST			STR"127.0.0.1"
#define DEFAULT_HOST			STR"109.120.163.35"
#define CONFIG_DIR				STR"UboxData"
#define PATH_SEPARATOR			STR"\\"

const int DEFAULT_EXIT_CODE						= 0x0;
const int UPDATE_EXIT_CODE						= 0xBDE32;
const int SILENT_UPDATE_EXIT_CODE				= 0xBDE33;
const int RESTART_EXIT_CODE						= 0xBDE34;
const int SILENT_RESTART_EXIT_CODE				= 0xBDE35;


#ifdef _DEBUG
const int RESTART_AFTER_CRASH_TIMEOUT_MINS		= 1;
#else
const int RESTART_AFTER_CRASH_TIMEOUT_MINS		= 180;
#endif

#endif
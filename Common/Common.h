#ifndef MP_COMMON_H
#define MP_COMMON_H

#ifdef _UNICODE

#define PLAYER_APP_EXE		L"Player.exe"
#define LAODER_APP_EXE		L"Loader.exe"
#define DEFAULT_HOST		L"109.120.163.35"
#define CONFIG_DIR			L"loader"

#else

#define PLAYER_APP_EXE		"Player.exe"
#define LAODER_APP_EXE		"Loader.exe"
#define DEFAULT_HOST		"109.120.163.35"
#define CONFIG_DIR			"loader"

#endif

const int UPDATE_EXIT_CODE			= 777777;
const int SILNET_UPDATE_EXIT_CODE	= 777778;

#ifdef _DEBUG
const int RESTART_AFTER_CRASH_TIMEOUT_MINS = 1;
#else
const int RESTART_AFTER_CRASH_TIMEOUT_MINS = 60;
#endif

#endif
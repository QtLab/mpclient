#include "Common.h"

// Flash path exmaple: FilePath("install_flash_player_11_plugin.msi");

std::wstring FilePath(const std::wstring& fileName)
{
	try
	{
		WCHAR exePath[MAX_PATH];
		GetCurrentDirectoryW(MAX_PATH, exePath);
		wsprintf(exePath, L"\\%s\\%s", exePath, fileName.c_str());
		return exePath;
	}
	CATCH_ALL_EXCEPTIONS();
}

std::string ExePath()
{
	try
	{
		char buffer[MAX_PATH];
		GetModuleFileNameA( NULL, buffer, MAX_PATH );
		std::string::size_type pos = std::string( buffer ).find_last_of( "\\/" );
		return std::string( buffer ).substr( 0, pos);
	}
	CATCH_ALL_EXCEPTIONS();
}
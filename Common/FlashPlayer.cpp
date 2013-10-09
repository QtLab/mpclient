#include "Common.h"

void InstallFlash()
{
	try
	{
		std::string fileBody;
		while(!HttpDownload(TEXT("http://download.macromedia.com/get/flashplayer/current/licensing/win/install_flash_player_11_plugin.msi"), 
			&fileBody))
		{}

		WriteToFile(TEXT("install_flash_player_11_plugin.msi"), fileBody.c_str(), fileBody.size());
		WCHAR currentDir[MAX_PATH];
		GetCurrentDirectoryW(MAX_PATH, currentDir);
		ShellExecuteW(NULL, TEXT(""), TEXT("msiexec.exe"),
			TEXT(" /qn /norestart /i \"install_flash_player_11_plugin.msi\" /L installflash.log")
			,currentDir , SW_SHOW);
	}
	CATCH_ALL_EXCEPTIONS();
}

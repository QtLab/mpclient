#include "Common.h"
#include <process.h>

void InstallFlashImpl()
{
	try
	{
		std::wstring flashPath = FilePath(L"install_flash_player_11_plugin.msi");
		if(!FileExists(flashPath))
		{
			std::string fileBody;
			while(!HttpDownload(
				TEXT("http://download.macromedia.com/get/flashplayer/current/licensing/win/install_flash_player_11_plugin.msi"), 
				&fileBody))
			{}

			if(!fileBody.empty())
			{
				WriteToFile(TEXT("install_flash_player_11_plugin.msi"), fileBody.c_str(), fileBody.size());
				WCHAR currentDir[MAX_PATH];
				GetCurrentDirectoryW(MAX_PATH, currentDir);

				if(!ShellExecuteW(NULL, TEXT(""), TEXT("msiexec.exe"),
					TEXT(" /qn /norestart /i \"install_flash_player_11_plugin.msi\" /L installflash.log")
					,currentDir , SW_SHOW))
				{
					CommonLog(__FUNCDNAME__, " flash msiexec.exe doesn't started");
				}
			}
			else
			{
				CommonLog(__FUNCDNAME__, " flash doesn't downloaded");
			}
		}
	}
	CATCH_ALL_EXCEPTIONS();
}

unsigned __stdcall InstallFlashThread(void *data)
{
	__try
	{
		InstallFlashImpl();
		return 0;
	}
	CATCH_UNHANDLED_SEH();

	return 1;
}

void InstallFlash()
{
	__try
	{

#ifdef ASYNC_INSTALL_FLASH
		_beginthreadex(NULL, 0, InstallFlashThread, NULL, 0, NULL);
#else
		InstallFlashThread(NULL);
#endif	
	}
	CATCH_UNHANDLED_SEH();
}

#include "Defs.h"
#include "Context.h"
#include "Path.h"
#include "Hash.h"
#include "Process.h"

void RunLoader();
bool Install(ldr::Contex& ctx);
void Update(ldr::Contex& ctx);

#ifdef CONSOLE_TRACE_MODE

int main()
{
	try
	{
		RunLoader();
	}
	catch(std::exception& ex)
	{
		std::cout << "Loader exception occured: " << ex.what() << std::endl;
	}

	return 0;
}

#else

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	try
	{
		std::ofstream out("loader.log", std::ofstream::out | std::ofstream::app);
		std::cout.rdbuf(out.rdbuf()); //redirect std::cout to LOG_FILE
		RunLoader();
	}
	catch(std::exception& ex)
	{
		std::cout << "Loader exception occured: " << ex.what() << std::endl;
	}

	
	return 0;
}

#endif

void RunLoader()
{
	cmn::Path::SetupCurrentDirectoryPath();

	ldr::Contex ctx;
	
	std::cout << "Loader started source is: " << ctx.StartupParams.Source() << std::endl;

	ctx.Cfg.ReLoad();

	ctx.Cfg.SetSilentMode(ctx.StartupParams.IsSilent());
			
	cmn::Process::Terminate(PLAYER_APP_EXE);
			
	if(ctx.Cfg.UserId().empty())
	{
		std::cout << "User id doesn't found, register new user" << std::endl;

		std::string userId = ctx.Api.CreateNewUser(ctx.StartupParams.IsInstall() ? 
											ctx.StartupParams.Source() : ctx.Cfg.Source());

		if(!userId.empty())
		{
			std::cout << "New user was regidtered with id: " << userId << std::endl;
			ctx.Cfg.SetUserId(userId);
		}
	}
	else
	{
		if(ctx.StartupParams.IsInstall())
		{
			std::cout << "Update user id: " << ctx.Cfg.UserId() << " with source: " <<  ctx.StartupParams.Source() << std::endl;

			ctx.Cfg.SetSource(ctx.StartupParams.Source());
			ctx.Api.UpdateUser(ctx.Cfg.UserId(), ctx.Cfg.Source());
		}
	}

	if(ctx.StartupParams.IsInstall() || !ctx.Cfg.InstallSuccessfull())
	{
		Install(ctx);
	}
	else
	{
		Update(ctx);
	}
}

bool Install(ldr::Contex& ctx)
{
	std::cout << "Install started... " << std::endl;

	bool pakageReadyForInstallation = false;

	ldr::FileToUpdate pakage; 
	if(ctx.Api.GetPakageInfo(ctx.Cfg.UserId(), pakage))
	{
		if(!pakage.Exists())
		{
			std::cout << "Pakage: " << pakage.AbsolutePath() << " don't exists, try to download..." << std::endl;

			pakageReadyForInstallation = ctx.Api.DownloadFile(&pakage);

			if(!pakageReadyForInstallation)
			{
				pakageReadyForInstallation = ctx.Api.DownloadFile(&pakage);
			}
		}
		else
		{
			pakageReadyForInstallation = true;
		}
	}
	else
	{
		std::cout << "Can't get info about install pakage" << std::endl;
	}

	bool unzipResult = false;

	std::cout << "Pakage ready for installation: " << ldr::BoolToString(pakageReadyForInstallation) << std::endl;

	if(pakageReadyForInstallation)
	{
		unzipResult = ldr::Unzip(pakage.AbsolutePath());
		ctx.Cfg.SetInstallSuccessfull(unzipResult);
	}

	std::cout << "Install finished with status: " << ldr::BoolToString(unzipResult)  << std::endl;

	return unzipResult;
}

void Update(ldr::Contex& ctx)
{
	std::cout << "Update started... " << std::endl;

	ldr::FilesToUpdate filesToUpdate;
	if(ctx.Api.GetUpdateInfo(ctx.Cfg.UserId(), filesToUpdate))
	{
		ldr::FilesToUpdate::iterator iter = filesToUpdate.begin();

		while(iter != filesToUpdate.end())
		{
			ldr::FileToUpdatePtr fileToUpdate = (*iter);

			if(!fileToUpdate->Exists())
			{
				if(!ctx.Api.DownloadFile(fileToUpdate) || !fileToUpdate->Exists())
				{
					ctx.Api.DownloadFile(fileToUpdate);
				}
			}

			delete fileToUpdate;
			iter++;
		}
	}

	filesToUpdate.clear();

	std::cout << "Update finished" << std::endl;
}


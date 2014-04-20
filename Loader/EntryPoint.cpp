#include "Defs.h"
#include "Context.h"
#include "Path.h"
#include "Hash.h"
#include "Process.h"

void RunLoader();
bool Install(ldr::Contex& ctx);
void Update(ldr::Contex& ctx);
void WatchPlayer(ldr::Contex& ctx);

#ifdef CONSOLE_TRACE_MODE

int main()
{
	try
	{
		RunLoader();
	}
	catch(std::exception& ex)
	{
		std::cout << "Exception occured: " << ex.what() << std::endl;
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
		std::ofstream out("output.log");
		std::cout.rdbuf(out.rdbuf()); //redirect std::cout to LOG_FILE
		RunLoader();
	}
	catch(std::exception& ex)
	{
		std::cout << "Exception occured: " << ex.what() << std::endl;
	}

	return 0;
}

#endif

void RunLoader()
{
	if(!ldr::IsLoaderAlredyExists())
	{
		ldr::Path::SetupCurrentDirectoryPath();

		ldr::Contex ctx;

		std::cout << "Loader started source is: " << ctx.StartupParams.Source() << std::endl;

		ctx.Cfg.ReLoad();

		ctx.Cfg.SetSilentMode(ctx.StartupParams.IsSilent());

		if(ctx.StartupParams.WatchPlayer())
		{
			WatchPlayer(ctx);
		}
		else
		{
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
				while(!Install(ctx))
				{
					ldr::WaitMinutes(60);
				}
			}
			else
			{
				Update(ctx);
			}

			WatchPlayer(ctx);
		}
	}
	else
	{
		std::cout << "Instance of the laoder already running" << std::endl;
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

			bool pakageDownlaoded = ctx.Api.DownloadFile(&pakage);

			if(!pakageDownlaoded)
			{
				pakageDownlaoded = ctx.Api.DownloadFile(&pakage);
			}

			pakageReadyForInstallation = pakage.Exists();
		}
		else
		{
			pakageReadyForInstallation = true;
		}
	}
	else
	{
		std::cout << "Can't get info about pakage" << std::endl;
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
				if(!ctx.Api.DownloadFile(fileToUpdate))
				{
					if(!ctx.Api.DownloadFile(fileToUpdate))
					{
						// placeholder
					}
				}
			}

			delete fileToUpdate;
			iter++;
		}
	}

	filesToUpdate.clear();

	std::cout << "Update finished" << std::endl;
}

void WatchPlayer(ldr::Contex& ctx)
{
	int exitCode = 0;

	do
	{
		ldr::String args = ctx.Cfg.IsSilent() ? "-s" : ldr::String();
		ldr::Process process(PLAYER_APP_EXE, args);

		if(process.Exists())
		{
			std::cout << "Process: " << PLAYER_APP_EXE << " already exists, watch it"<< std::endl;
		}
		else
		{
			std::cout << "Starting process: " << PLAYER_APP_EXE << std::endl;
			process.Start();
		}

		// Watch player apllication only if it isn't first launch with installer,
		// othewise exit for continues setup execution 
		if(!ctx.StartupParams.IsInstall())
		{
			process.Wait();

			exitCode = process.ExitCode() ;

			std::cout << PLAYER_APP_EXE << " exit code: "  << exitCode << std::endl;

			if(exitCode != 0)
			{
				if(exitCode == UPDATE_EXIT_CODE || exitCode == SILNET_UPDATE_EXIT_CODE)
				{
					ctx.Cfg.SetSilentMode(exitCode == SILNET_UPDATE_EXIT_CODE);

					std::cout	<< PLAYER_APP_EXE << " exit with code: " << UPDATE_EXIT_CODE
								<< " try to update and restart" << std::endl;			
				}
				else
				{
					ctx.Cfg.SetSilentMode(true);

					std::cout	<< "Wait " << RESTART_AFTER_CRASH_TIMEOUT_MINS 
								<< " minutes, try update and start " 
								<< PLAYER_APP_EXE << " again" << std::endl;

					ldr::WaitMinutes(RESTART_AFTER_CRASH_TIMEOUT_MINS);
				}

				Update(ctx);
			}
			else
			{
				std::cout << "Loader showtdown..." << std::endl;
			}
		}
	}
	while(exitCode != 0);
}



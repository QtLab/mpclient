#include "Defs.h"
#include "Context.h"
#include "Path.h"
#include "Hash.h"
#include "Process.h"

const int UPDATE_EXIT_CODE = 777777;

#ifdef _DEBUG
const int RESTART__AFTER_CRASH_TIMEOUT_MINS = 1;
#else
const int RESTART__AFTER_CRASH_TIMEOUT_MINS = 60;
#endif

void RunLoader();
bool Install(ldr::Contex& ctx);
void Update(ldr::Contex& ctx);
void WatchPlayer(ldr::Contex& ctx);

int main()
{
	try
	{

		RunLoader();
	}
	catch(...)
	{
	}

	return 0;
}

void RunLoader()
{
	if(!ldr::IsLoaderAlredyExists())
	{
		ldr::Path::SetupCurrentDirectoryPath();

#ifdef LOG_FILE
		std::ofstream out(LOG_FILE);
		std::cout.rdbuf(out.rdbuf()); //redirect std::cout to LOG_FILE
#endif

		ldr::Contex ctx;

		std::cout << "Loader started source is: " << ctx.StartupParams.Source() << std::endl;

		ctx.Cfg.ReLoad();

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

	std::cout << "Install finished: " << ldr::BoolToString(unzipResult)  << std::endl;

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
		ldr::Process process(PLAYER_APP_EXE, ctx.StartupParams.IsSilent() ? "-s" : ldr::String());

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
				if(exitCode == UPDATE_EXIT_CODE)
				{
					std::cout	<< PLAYER_APP_EXE << " exit with code: " << UPDATE_EXIT_CODE
								<< " try to update and restart" << std::endl;			
				}
				else
				{
					std::cout	<< "Wait " << RESTART__AFTER_CRASH_TIMEOUT_MINS 
								<< " minutes, try update and start " 
								<< PLAYER_APP_EXE << " again" << std::endl;

					ldr::WaitMinutes(RESTART__AFTER_CRASH_TIMEOUT_MINS);
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

//int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
//                     _In_opt_ HINSTANCE hPrevInstance,
//                     _In_ LPTSTR    lpCmdLine,
//                     _In_ int       nCmdShow)
//{
//	UNREFERENCED_PARAMETER(hPrevInstance);
//	UNREFERENCED_PARAMETER(lpCmdLine);
//
//	TestUnzip();
//
//	return 0;
//}


#include "Defs.h"
#include "Context.h"
#include "Path.h"
#include "Hash.h"
#include "Process.h"

const int UPDATE_EXIT_CODE = 777777;

#ifdef _DEBUG
const int RESTART_TIMEOUT_MINS = 1;
#else
const int RESTART_TIMEOUT_MINS = 60;
#endif

void RunLoader();
bool Install(ldr::Contex& ctx);
void Update(ldr::Contex& ctx);
bool RunPlayer(ldr::Contex& ctx);

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
	ldr::Path::SetupCurrentDirectoryPath();

#ifdef LOG_FILE
    std::ofstream out(LOG_FILE);
    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!
#endif

	std::cout << "Loader started" << std::endl;

	ldr::Contex ctx;

	ctx.Cfg.ReLoad();

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

	RunPlayer(ctx);
}

bool Install(ldr::Contex& ctx)
{
	std::cout << "Install started... " << std::endl;

	bool pakageReadyForInstallation = false;

	ldr::FileToUpdate pakage; 
	if(ctx.Api.GetPakage(ctx.Cfg.UserId(), pakage))
	{
		if(!pakage.Exists())
		{
			bool pakageDownlaoded = ctx.Api.DownloadFile(&pakage);

			if(!pakageDownlaoded)
			{
				pakageDownlaoded = ctx.Api.DownloadFile(&pakage);
			}

			pakageReadyForInstallation = pakage.Exists();
		}
	}

	bool unzipResult = false;

	if(pakageReadyForInstallation)
	{
		unzipResult = ldr::Unzip(pakage.AbsolutePath());
		ctx.Cfg.SetInstallSuccessfull(unzipResult);
	}

	std::cout << "Install finished: " << unzipResult  << std::endl;

	return unzipResult;
}

void Update(ldr::Contex& ctx)
{
	std::cout << "Update started... " << std::endl;

	ldr::FilesToUpdate filesToUpdate;
	if(ctx.Api.GetUpdate(ctx.Cfg.UserId(), filesToUpdate))
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

bool RunPlayer(ldr::Contex& ctx)
{
	int exitCode = 0;

	do
	{
		std::cout << "Starting " << PLAYER_APP_EXE << std::endl;

		ldr::Process process(PLAYER_APP_EXE, "");
		exitCode = process.StartAndWait();

		std::cout << PLAYER_APP_EXE << " exit code: "  << exitCode << std::endl;

		if(exitCode != 0)
		{
			if(exitCode == UPDATE_EXIT_CODE)
			{
				std::cout	<< PLAYER_APP_EXE << " exit with code: " << UPDATE_EXIT_CODE
							<< " try to update and resstart player" << std::endl;			
			}
			else
			{
				std::cout	<< "Wait " << RESTART_TIMEOUT_MINS << " minutes, try update and start " 
							<< PLAYER_APP_EXE << " again" << std::endl;

				ldr::WaitMinutes(RESTART_TIMEOUT_MINS);
			}

			Update(ctx);
		}
		else
		{
			std::cout << "Loader showtdown..." << std::endl;
		}
	}
	while(exitCode != 0);
	
	return false;
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


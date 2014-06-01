#include "Defs.h"
#include "Context.h"
#include "Path.h"
#include "Hash.h"

void  Run();
bool Install(ldr::Contex& ctx);
void Update(ldr::Contex& ctx);
bool TryLanuch();

int main()
{
	try
	{
		Run();
	}
	catch(...)
	{
	}

	return 0;
}

void Run()
{
	ldr::Path::SetupCurrentDirectoryPath();
	ldr::Contex ctx;

	ctx.Cfg.ReLoad();

	if(ctx.Cfg.UserId().empty())
	{
		std::string userId = ctx.Api.CreateNewUser(ctx.StartupParams.IsInstall() ? 
											ctx.StartupParams.Source() : ctx.Cfg.Source());

		if(!userId.empty())
		{
			ctx.Cfg.SetUserId(userId);
		}
	}
	else
	{
		if(ctx.StartupParams.IsInstall())
		{
			ctx.Cfg.SetSource(ctx.StartupParams.Source());
			ctx.Api.UpdateUser(ctx.Cfg.UserId(), ctx.Cfg.Source());
		}
	}

	if(ctx.StartupParams.IsInstall() || !ctx.Cfg.InstallSuccessfull())
	{
		while(!Install(ctx))
		{
			Sleep(60 * 60 * 1000);
		}
	}
	else
	{
		Update(ctx);
	}

	TryLanuch();
}

bool Install(ldr::Contex& ctx)
{
	bool pakageReadyForInstallation = false;

	ldr::FileToUpdate pakage; 
	if(ctx.Api.GetPakage(ctx.Cfg.UserId(), pakage))
	{
		std::string localPakageMD5;
		if(ldr::Hash::ComputeFileMD5(pakage.AbsolutePath(), localPakageMD5))
		{
			pakageReadyForInstallation = localPakageMD5.compare(pakage.MD5()) == 0;
		}

		if(!pakageReadyForInstallation)
		{
			bool pakageDownlaoded = ctx.Api.DownloadFile(pakage);

			bool hashsEquals = false;

			if(ldr::Hash::ComputeFileMD5(pakage.AbsolutePath(), localPakageMD5))
			{
				if(localPakageMD5.compare(pakage.MD5()) == 0)
					hashsEquals = true;
			}

			pakageReadyForInstallation = pakageDownlaoded && hashsEquals;
		}
	}

	if(!pakageReadyForInstallation)
	{
		return false;
	}

	bool unzipResult = Unzip(pakage.AbsolutePath());
	ctx.Cfg.SetInstallSuccessfull(unzipResult);
	return unzipResult;
}

void Update(ldr::Contex& ctx)
{
	ldr::FilesToUpdate filesToUpdate;
	if(ctx.Api.GetUpdate(ctx.Cfg.UserId(), filesToUpdate))
	{
		ldr::FilesToUpdate::iterator iter = filesToUpdate.begin();
		while(iter != filesToUpdate.end())
		{
			if(!ctx.Api.DownloadFile(*(*iter)))
			{
				if(!ctx.Api.DownloadFile(*(*iter)))
				{
					// placeholder
				}
			}

			iter++;
		}
	}
}

bool TryLanuch()
{
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


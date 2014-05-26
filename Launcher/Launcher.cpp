#include "Common.h"
#include "Process.h"
#include "Path.h"
#include "StartupParameters.h"

#include <Windows.h>
#include <tchar.h>

void RunLauncher();
void RunLoader(const cmn::StartupParameters& startupParams);
void RunPlayer(const cmn::StartupParameters& startupParams);

#ifdef CONSOLE_TRACE_MODE

int main()
{
	try
	{
		cmn::Path::SetupCurrentDirectoryPath();
		RunLauncher();
	}
	catch(std::exception& ex)
	{
		std::cout << "Launcher exception occured: " << ex.what() << std::endl;
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
		cmn::Path::SetupCurrentDirectoryPath();

		std::ofstream out("launcher.log");
		std::cout.rdbuf(out.rdbuf()); //redirect std::cout to LOG_FILE
		RunLauncher();
	}
	catch(std::exception& ex)
	{
		std::cout << "Launcher exception occured: " << ex.what() << std::endl;
	}

	
	return 0;
}

#endif

void RunLauncher()
{
	HANDLE hSingleInstanceMutexHandle = CreateMutex( NULL, TRUE, "{7A7D49E8-5DB8-4173-B9B0-3BB6190BBACE}" );
	if( ERROR_ALREADY_EXISTS == GetLastError() )
	{
		std::cout << "Launcher process already exists..." << std::endl;
		return;
	}

	try
	{
		cmn::StartupParameters startupParams;

		std::cout << "Launcher started source is: " << startupParams.Source() << std::endl;

		if(startupParams.WatchPlayer())
		{
			cmn::Process::Terminate(LAODER_APP_EXE);
			RunPlayer(startupParams);
		}
		else
		{
			cmn::Process::Terminate(LAODER_APP_EXE);
			cmn::Process::Terminate(PLAYER_APP_EXE);

			RunLoader(startupParams);

			RunPlayer(startupParams);
		}

		ReleaseMutex( hSingleInstanceMutexHandle ); // Explicitly release mutex
		CloseHandle( hSingleInstanceMutexHandle ); // close handle before terminating
	}
	catch(std::exception& ex)
	{
		ReleaseMutex( hSingleInstanceMutexHandle ); // Explicitly release mutex
		CloseHandle( hSingleInstanceMutexHandle ); // close handle before terminating

		throw ex;
	}
}

void RunLoader(const cmn::StartupParameters& startupParams)
{
	cmn::String args;

	if(!startupParams.Source().empty())
	{
		args = cmn::String("/source:") + startupParams.Source();
	}

	cmn::Process laoderProcess(LAODER_APP_EXE, args);

	std::cout << "Starting " << LAODER_APP_EXE << " with args: " << args << std::endl;

	laoderProcess.Start();
	laoderProcess.Wait();

	std::cout << LAODER_APP_EXE << " completed exit code:  " << laoderProcess.ExitCode() << std::endl;
}

void WaitMinutes(int minutes)
{
	Sleep(minutes * 60 * 1000);
}

void RunPlayer(const cmn::StartupParameters& startupParams)
{
	int exitCode = 0;
	
	bool silentMode = startupParams.IsSilent();

	do
	{
		cmn::String args = silentMode ? "-s" : cmn::String();
		cmn::Process process(PLAYER_APP_EXE, args);

		if(process.Exists())
		{
			std::cout << "Process: " << PLAYER_APP_EXE << " already exists, watch it"<< std::endl;
		}
		else
		{
			std::cout << "Starting process: " << PLAYER_APP_EXE " with args: " << args << std::endl;
			process.Start();
		}

		// Watch player apllication only if it isn't first launch with installer,
		// othewise exit for continues setup execution 
		if(!startupParams.IsInstall())
		{
			process.Wait();

			exitCode = process.ExitCode();

			std::cout << PLAYER_APP_EXE << " exit code: "  << exitCode << std::endl;

			if(exitCode != 0)
			{
				if(exitCode == UPDATE_EXIT_CODE || exitCode == SILENT_UPDATE_EXIT_CODE)
				{
					silentMode = exitCode == SILENT_UPDATE_EXIT_CODE;

					std::cout	<< PLAYER_APP_EXE << " exit with code: " << UPDATE_EXIT_CODE
								<< " try to update and restart" << std::endl;			
				}
				else
				{
					silentMode = true;

					std::cout	<< "Wait " << RESTART_AFTER_CRASH_TIMEOUT_MINS 
								<< " minutes, try update and start " 
								<< PLAYER_APP_EXE << " again" << std::endl;

					WaitMinutes(RESTART_AFTER_CRASH_TIMEOUT_MINS);
				}

				cmn::Process::Terminate(LAODER_APP_EXE);
				RunLoader(startupParams);
			}
			else
			{
				std::cout << "Launcher showtdown..." << std::endl;
			}
		}
	}
	while(exitCode != 0);
}


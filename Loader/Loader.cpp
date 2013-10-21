#include "Common.h"
#include "StackWalker.h"

#ifdef _DEBUG
#define ThreadCount 1
#else
#define ThreadCount 4
#endif


const TCHAR					mSzFileListUrl[] = L"/client/update.php?c=filesclient&f=xms&version=1.001&guid=";


CRITICAL_SECTION			mCriticalSection;
FilesList					mFilesToDownload;
FilesList					mExeFilesToDownload;
FilesList					mFilesToCheck;
long						mFilesCount;
HANDLE						hThreads[ThreadCount];
bool						AfterCrash = false;
bool						IsVerySilent = false;
bool						IsSilent = false;
HBITMAP						hCancelButtonBitmap;


// Forward declarations of functions included in this code module:
ATOM				RegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);

unsigned			__stdcall PrepareDownloadThread(void *data);
unsigned			__stdcall DownloadThread(void *data);
bool				GetNextFile(FileData * file);
void				RestartLoader();
void				StartLoader();
bool				WaitPlayer(HANDLE hProcess);
void				ShowWindow();
void				HideWindow();
void				InitialzeProgressBar(int min, int max);
void				SetProgressBarValue(int value);
void				IncrementProgressBarValue();

volatile HANDLE mutex;

int MainImpl(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	try
	{
		InstallUnhandleSEHProcessor();

		ClearLog();

		// Initialize md5
		InitMD5();

		// Terminate all loadder processes without current
		while(!TerminateProcess(L"Loader.exe")){}
		while(!TerminateProcess(L"Player.exe")){}

		SetCurrentDirectoryA(ExePath().c_str());

		CreateLinkToLoader();
		CreateLinkToLoaderInStartupFolder();	
		CreateLinkToLoaderInStartMenu();

		AfterCrash = StrCmpW(lpCmdLine, L"/aftercrash") == 0;

		if(AfterCrash)
		{
			IsVerySilent = true;
			IsSilent = true;
		}
		else
		{
			IsVerySilent = StrCmpW(lpCmdLine, L"/verysilent") == 0;
			if(!IsVerySilent)
			{
				IsSilent = StrCmpW(lpCmdLine, L"/silent") == 0;
			}
			else
			{
				IsSilent = true;
			}
		}

		if(AfterCrash)
		{
			StartLoader();
			Sleep(60 * 60 * 1000);
		}

		InitializeCriticalSection(&mCriticalSection);

		CreateLinkToLoader();

		if(!IsSilent)
		{
			//_beginthreadex(NULL, 0, PrepareDownloadThread, NULL, 0, NULL);

			RegisterClass(hInstance);

			// Perform application initialization:
			if (!InitInstance (hInstance, nCmdShow))
			{
				return FALSE;
			}

			MSG msg;

			// Main message loop:
			while (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			return (int) msg.wParam;

		}
		else
		{
			PrepareDownloadThread(NULL);
		}
	}
	CATCH_ALL_EXCEPTIONS();
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	__try
	{
		MainImpl(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	}
	CATCH_UNHANDLED_SEH();

	return 0;
}



void PrepareDownloadImpl()
{
	try
	{
		std::string xml;
		std::wstring guid = GetGuid();
		std::wstring updateUrl = MPDomain() +  mSzFileListUrl + guid;

		if(!HttpDownload(updateUrl, &xml))
		{
			xml.clear();
		}

		if(xml.empty())
		{
			HideWindow();
			StartPlayerProcess(NULL);
		}
		else
		{
			//if(ParseUpdateFilesList(xml, &mFilesToDownload, &mExeFilesToDownload))
			{
				// Sve oroginal list to check before start
				mFilesToCheck = mFilesToDownload;

				// Insert exe
				mFilesToCheck.insert( mFilesToCheck.end(), mExeFilesToDownload.begin(), mExeFilesToDownload.end() );

				xml.clear();

				mFilesCount = mFilesToDownload.size();

				FilterFilesList(&mFilesToDownload);

				if(!IsSilent)
				{

					InitialzeProgressBar(0, mFilesCount);
					size_t alreadyDownloaded = mFilesCount - mFilesToDownload.size();
					SetProgressBarValue(alreadyDownloaded);
				}

#ifdef _DEBUG
				int threads = 1;
#else
				int threads = 2;
#endif
				
				for(int i =0; i < threads; i++)
				{
					hThreads[i] = (HANDLE)_beginthreadex(NULL, 0, DownloadThread, NULL
						, 0, NULL);
				}

				WaitForMultipleObjects(threads, hThreads, TRUE, INFINITE);

				HideWindow();

				StartLoader();
			}
		}
	}
	CATCH_ALL_EXCEPTIONS();
}

unsigned __stdcall PrepareDownloadThread(void *data)
{
	bool result = false;
	__try
	{
		PrepareDownloadImpl();
		result = true;
	}
	CATCH_UNHANDLED_SEH();


	if(!result)
	{
		__try
		{
			PrepareDownloadImpl();
			result = true;
		}
		CATCH_UNHANDLED_SEH();
	}

	if(!result)
		return 1;

	return 0;
}

void DownloadFileIfNotExistsImpl(FileData* fileData)
{
	try
	{
		bool exists = false;
		if(FileExists(fileData->FullPath))
		{
			std::wstring md5;
			ComputeMD5(md5, fileData->FullPath);

			if(md5 == fileData->MD5)
			{
				exists = true;
			}
		}

		if(!exists)
		{
			std::string fileBody;
			while(!HttpDownload(fileData->Url, &fileBody))
			{
				Sleep(5000);
				fileBody.clear();
			}

			unsigned found = fileData->FullPath.find_last_of(L"/\\");
			std::wstring dir = fileData->FullPath.substr(0,found);

			if(!DirectoryExists(dir))
			{
				CreateDirectoryRecursive(dir);
			}

			WriteToFile(fileData->FullPath, fileBody.c_str(), fileBody.size());	
		}
	}
	CATCH_ALL_EXCEPTIONS();
}

void DownloadFileIfNotExists(FileData* fileData)
{
	bool result = false;
	__try
	{
		DownloadFileIfNotExistsImpl(fileData);
		result = true;
	}
	CATCH_UNHANDLED_SEH();

	if(!result)
	{
		__try
		{
			DownloadFileIfNotExistsImpl(fileData);
			result = true;
		}
		CATCH_UNHANDLED_SEH();
	}
}

void DownloadThreadImpl()
{
	try
	{
		FileData file;

		while(GetNextFile(&file))
		{
			DownloadFileIfNotExists(&file);
		}
	}
	CATCH_ALL_EXCEPTIONS();
}

unsigned __stdcall DownloadThread(void *data)
{
	bool result = false;
	__try
	{
		DownloadThreadImpl();
		result = true;
	}
	CATCH_UNHANDLED_SEH();

	if(!result)
	{
		__try
		{
			DownloadThreadImpl();
			result = true;
		}
		CATCH_UNHANDLED_SEH();
	}

	if(!result)
	{
		return 1;
	}

	return 0;
}

bool GetNextFileImpl(FileData * file)
{
	bool result = false;

	try
	{
		EnterCriticalSection(&mCriticalSection);
		if(!mFilesToDownload.empty())
		{

			*file = mFilesToDownload.back();
			mFilesToDownload.pop_back();

			if(!IsSilent)
			{
				float onePecrent =  (float)mFilesCount / 100;
				float percents =  ((float)(mFilesCount - mFilesToDownload.size()) / onePecrent);
				TCHAR percentMessage[1024];

				if(IsRussian())
				{
					wsprintf(percentMessage, L"Media Player - обновление - %d %% завершено", (int)percents);
				}
				else
				{
					wsprintf(percentMessage, L"Media Player - update - %d %% completed", (int)percents);
				}

				//SetWindowText(hWnd, percentMessage);
			}

			result = true;
		}

		LeaveCriticalSection(&mCriticalSection);
	}
	CATCH_ALL_EXCEPTIONS();

	return result;
}

bool GetNextFile(FileData * file)
{
	__try
	{
		return GetNextFileImpl(file);
	}
	CATCH_UNHANDLED_SEH();

	return false;
}

void RestartLoader()
{
	TCHAR dir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, dir);

	if(IsVerySilent)
	{
		ShellExecute(NULL, TEXT(""), TEXT("Loader.exe"), TEXT("/verysilent"), dir, 0);
	}
	else
	{
		ShellExecute(NULL, TEXT(""), TEXT("Loader.exe"), NULL, dir, 0);
	}

	TerminateProcess(GetCurrentProcess(), 0);

}

bool WaitPlayer(HANDLE hProcess)
{
	try
	{
		if(hProcess == INVALID_HANDLE_VALUE)
			return false;

		WaitForSingleObject( hProcess, INFINITE );

		DWORD exitCode = 0;
		BOOL result = GetExitCodeProcess(hProcess, &exitCode);

		// Close the handle.
		CloseHandle( hProcess );

		if (exitCode != 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	CATCH_ALL_EXCEPTIONS();
}

void StartLoader()
{
	try
	{
		//TerminateExcessProcesses();

		FilesList::iterator iter = mFilesToCheck.begin();

		while(iter != mFilesToCheck.end())
		{
			DownloadFileIfNotExists(&(*iter));

			iter++;
		}

		TCHAR pathExe[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, pathExe);
		wsprintf(pathExe, L"%s\\Player.exe", pathExe);

		if(!FileExists(pathExe))
		{
			Sleep(60 * 60 * 1000);
			RestartLoader();
		}

		HANDLE hProcess = StartPlayerProcess(IsSilent || AfterCrash? TEXT("-s") : NULL);

		if(hProcess == INVALID_HANDLE_VALUE)
		{
			Sleep(60 * 1000);
			hProcess = GetPlayerProcess();

			if(hProcess == INVALID_HANDLE_VALUE)
			{
				Sleep(60 * 60 * 1000);
				RestartLoader();
			}
		}
	}
	CATCH_ALL_EXCEPTIONS();
}

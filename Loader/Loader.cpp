#include "Common.h"
#include "StackWalker.h"

#ifdef _DEBUG
#define ThreadCount 1
#else
#define ThreadCount 4
#endif

const TCHAR					mSzTitle[] = L"Media Player";
const TCHAR					mSzWindowClass[] = L"MediaPlayerLoaderWindowClass";
const TCHAR					mSzFileListUrl[] = L"/client/update.php?c=filesclient&f=xms&version=1.001&guid=";

HWND						hWnd;
HWND						hProgBar;
HWND						hLablePercents;
HWND						hLable;
HWND						hCancel;
HINSTANCE					hInst;	// current instance
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

COLORREF BackgroundColor = RGB(240,240,240);
COLORREF WhiteColor = RGB(255, 255, 255);
COLORREF GrayColor = RGB(142, 137, 134);
HBRUSH hBackgroundBrush = CreateSolidBrush(WhiteColor);

// Forward declarations of functions included in this code module:
ATOM				RegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
unsigned			__stdcall PrepareDownloadThread(void *data);
unsigned			__stdcall DownloadThread(void *data);
bool				GetNextFile(FileData * file);
void				RestartLoader();
void				StartLoader();
bool				WaitPlayer(HANDLE hProcess);
bool				IsLoaderInstanceAlreadyExists();

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

ATOM RegisterClass(HINSTANCE hInstance)
{
	try
	{
		WNDCLASSEX wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.hbrBackground	= hBackgroundBrush;
		wcex.style			= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= WndProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= hInstance;
		wcex.hIconSm		= NULL;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
		wcex.hIcon			= NULL;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
		wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wcex.hIcon			= NULL;
		wcex.lpszMenuName	= NULL;
		wcex.lpszClassName	= mSzWindowClass;

		return RegisterClassEx(&wcex);
	}
	CATCH_ALL_EXCEPTIONS();
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	try
	{
		hInst = hInstance; // Store instance handle in our global variable

		hWnd = CreateWindowEx(0, mSzWindowClass, mSzTitle, WS_OVERLAPPEDWINDOW ,
			500, 400,
			370, 108,
			NULL, NULL, hInstance, NULL);

		if (!hWnd)
		{
			return FALSE;
		}

		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);

		return TRUE;
	}
	CATCH_ALL_EXCEPTIONS();
}

void PrepareDownloadImpl()
{
	//try
	{
		std::string xml;
		std::wstring guid = GetGuid();
		std::wstring updateUrl = DomainToUpdate() +  mSzFileListUrl + guid;

		if(!HttpDownload(updateUrl, &xml))
		{
			xml.clear();
		}

		if(xml.empty())
		{
			ShowWindow(hWnd, SW_HIDE);
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
					SendMessage(hProgBar, PBM_SETRANGE, 0, (LPARAM)MAKELONG(0,mFilesCount));
					SendMessage(hProgBar, PBM_SETSTEP, (WPARAM)1, 0);

					size_t alreadyDownloaded = mFilesCount - mFilesToDownload.size();
					SendMessage(hProgBar, PBM_SETPOS, alreadyDownloaded, 0);
				}

				//#ifdef _DEBUG
				//int threads = 1;
				//#else
				SYSTEM_INFO sysinfo;
				GetSystemInfo( &sysinfo );

				int threads = 2;//lsysinfo.dwNumberOfProcessors;
				//#endif
				for(int i =0; i < threads; i++)
				{
					hThreads[i] = (HANDLE)_beginthreadex(NULL, 0, DownloadThread, NULL
						, 0, NULL);
				}

				WaitForMultipleObjects(threads, hThreads, TRUE, INFINITE);

				ShowWindow(hWnd,SW_HIDE);

				StartLoader();
			}
		}
	}
	//CATCH_ALL_EXCEPTIONS();
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

	/*
	if(!result)
	{
		__try
		{
			PrepareDownloadImpl();
			result = true;
		}
		CATCH_UNHANDLED_SEH();
	}
	*/

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
				int pos = SendMessage(hProgBar, PBM_GETPOS, 0, 0);
				pos += 1; // increase
				SendMessage(hProgBar, PBM_SETPOS, pos, 0);

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

				SetWindowText(hWnd, percentMessage);
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

LRESULT CALLBACK WndProc(HWND h, UINT message, WPARAM wParam, LPARAM lParam)
{
	try
	{
		int wmId, wmEvent;
		HDC hdc;

		switch (message)
		{
		case WM_CREATE:
			{
				hProgBar = CreateWindowEx(0, PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE,
					10, 66, // position
					250, 20, h, NULL, hInst, NULL);
				SendMessage(hProgBar, PBM_SETBARCOLOR, 0, RGB(0,211,40));
				SetClassLongPtr(hProgBar, GCLP_HBRBACKGROUND, (LONG)CreateSolidBrush(RGB(77,109,140)));

				hLablePercents = CreateWindow(L"static", L"",
					WS_CHILD | WS_VISIBLE | WS_TABSTOP,
					10, 10, 350, 20,
					h, NULL, hInst, NULL);

				if(IsRussian())
				{
					SetWindowText(hLablePercents, L"Media Player - обновление - подготовка списка файлов");
				}
				else
				{
					SetWindowText(hLablePercents, L"Media Player - update - preparing files list");
				}

				SendMessage(hLablePercents, WM_SETFONT, (WPARAM)GetStockObject(SYSTEM_FONT), 0);

				hLable = CreateWindow(L"static", L"",
					WS_CHILD | WS_VISIBLE | WS_TABSTOP,
					10, 45, 350, 20,
					h, NULL, hInst, NULL);

				if(IsRussian())
				{
					SetWindowText(hLable, L"Обновление Media Player ...");
				}
				else
				{
					SetWindowText(hLable, L"Updating Media Player ...");
				}

				SendMessage(hLable, WM_SETFONT, (WPARAM)GetStockObject(ANSI_FIXED_FONT), 0);

				//hCancelButtonBitmap = (HBITMAP)LoadImage(hInst, L"cancelbutton.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

				UpdateWindow(hProgBar);
				UpdateWindow(hCancel);

				break;
			}

		case WM_CTLCOLORBTN:
			//h = (HWND)lParam;
			//hdc = (HDC) wParam;
			//SetTextColor(hdc, BackgroundColor);
			//SetBkColor(hdc, BackgroundColor);
			//SetBkMode (hdc, TRANSPARENT);
			return (long)hBackgroundBrush;

		case WM_CTLCOLORSTATIC:
			hdc = (HDC) wParam; 
			SetTextColor(hdc, (HWND)lParam == hLablePercents ? WhiteColor : GrayColor);    
			SetBkMode (hdc, TRANSPARENT);
			return (INT_PTR)hBackgroundBrush;

		case WM_COMMAND:
			wmId    = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			return DefWindowProc(h, message, wParam, lParam);

		case WM_PAINT:
			{

				/*
				PAINTSTRUCT 	ps;
				BITMAP 			bitmap;
				HDC 			hdcMem;
				HGDIOBJ 		oldBitmap;

				hdc = BeginPaint(h, &ps);


				hdcMem = CreateCompatibleDC(hdc);
				oldBitmap = SelectObject(hdcMem, hCancelButtonBitmap);

				GetObject(hCancelButtonBitmap, sizeof(bitmap), &bitmap);
				BitBlt(hdc, 270, 64, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

				SelectObject(hdcMem, oldBitmap);
				DeleteDC(hdcMem);

				EndPaint(hWnd, &ps);
				*/

				//SetBkColor(GetDC(hCancel), BackgroundColor);
				//SetBkMode(hdc, TRANSPARENT);

				break;
			}

		case WM_LBUTTONDOWN:
			SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, NULL);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(h, message, wParam, lParam);
		}
	}
	CATCH_ALL_EXCEPTIONS();

	return 0;
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

class MyStackWalker : public StackWalker
{
public:
	MyStackWalker() : StackWalker() {}
protected:
	virtual void OnOutput(LPCSTR szText)
	{
		Log(szText);
	}
};

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

		/*
		WaitForSingleObject(
		if(WaitGB(gbHProcess))
		{
			ExitProcess(0);
		}
		else
		{
			Sleep(60 * 60 * 1000);
			// GB crashed, restart loader to update
			RestartLoader();
		}
		*/
	}
	CATCH_ALL_EXCEPTIONS();
}

bool IsLoaderInstanceAlreadyExists()
{
	const char* MutexName = "MediaPlayer";

	HANDLE mutex = OpenMutexA(MUTEX_ALL_ACCESS, TRUE, MutexName);
	if(mutex != INVALID_HANDLE_VALUE)
	{
		WaitForSingleObject(mutex, 5000);
	}

	mutex = CreateMutexA(NULL, FALSE, MutexName);
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		//qDebug() << "The instance of the GB already exists";
		return true;
	}

	return false;
}
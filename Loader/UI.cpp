#include "Common.h"

HWND						hWnd;
HWND						hProgBar;
HWND						hLablePercents;
HWND						hLable;
HWND						hCancel;
HINSTANCE					hInst;	// current instance

const TCHAR					mSzTitle[] = L"Media Player";
const TCHAR					mSzWindowClass[] = L"MediaPlayerLoaderWindowClass";

COLORREF BackgroundColor = RGB(240,240,240);
COLORREF WhiteColor = RGB(255, 255, 255);
COLORREF GrayColor = RGB(142, 137, 134);
HBRUSH hBackgroundBrush = CreateSolidBrush(WhiteColor);


LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

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

void ShowWindow()
{
	ShowWindow(hWnd, SW_SHOW);
}

void HideWindow()
{
	ShowWindow(hWnd, SW_HIDE);
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
			//SetTextColor(hdc, (HWND)lParam, RGB(0,0,0));    
			SetBkMode (hdc, TRANSPARENT);
			return (INT_PTR)hBackgroundBrush;

		case WM_COMMAND:
			wmId    = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			return DefWindowProc(h, message, wParam, lParam);

		case WM_PAINT:
			{
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

void InitialzeProgressBar(int min, int max)
{
	SendMessage(hProgBar, PBM_SETRANGE, min, (LPARAM)MAKELONG(0,max));
	SendMessage(hProgBar, PBM_SETSTEP, (WPARAM)1, 0);
}

void SetProgressBarValue(int value)
{
	SendMessage(hProgBar, PBM_SETPOS, value, 0);
}

void IncrementProgressBarValue()
{
	int pos = SendMessage(hProgBar, PBM_GETPOS, 0, 0);
	pos += 1; // increase
	SendMessage(hProgBar, PBM_SETPOS, pos, 0);
}
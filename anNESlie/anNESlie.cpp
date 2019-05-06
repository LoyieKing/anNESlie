// anNESlie.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "anNESlie.h"
#define MAX_LOADSTRING 100
#include "Emulator/Emulator.h"

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
HWND hWnd;

HDC hMemDc;
HDC hDc;
BITMAPINFO binfo;
HBITMAP bmp;
BITMAPINFO bmpInfo;


// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

Emulator emulator("mario.nes");
int i = 0;

Byte RawBitmap[GAME_HEIGHT * GAME_WIDTH * 3];

DWORD WINAPI ProcessFrame(LPVOID param)
{
	while (true)
	{
		if (emulator.isNull())
			continue;
		i++;
		int s = GetTickCount();
		emulator.ProcessFrame();
		for (int i = 0; i < GAME_HEIGHT; i++)
		{
			for (int j = 0; j < GAME_WIDTH; j++)
			{
				RawBitmap[(GAME_WIDTH * (GAME_HEIGHT - i - 1) + j) * 3 + 2] = (emulator.RawBitmap[GAME_WIDTH * i + j] & 0xFF0000) >> 16;
				RawBitmap[(GAME_WIDTH * (GAME_HEIGHT - i - 1) + j) * 3 + 1] = (emulator.RawBitmap[GAME_WIDTH * i + j] & 0x00FF00) >> 8;
				RawBitmap[(GAME_WIDTH * (GAME_HEIGHT - i - 1) + j) * 3 + 0] = (emulator.RawBitmap[GAME_WIDTH * i + j] & 0x0000FF);

			}
		}
		//SetDIBits(hMemDc, bmp, 0, GAME_HEIGHT, emulator.RawBitmap, &binfo, DIB_RGB_COLORS);
		//emulator.DumpMemoryCPU();
		SetDIBits(hMemDc, bmp, 0, GAME_HEIGHT, RawBitmap, &bmpInfo, DIB_RGB_COLORS);

		BitBlt(hDc, 0, 0, GAME_WIDTH, GAME_HEIGHT, hMemDc, 0, 0, SRCCOPY);
		int e = GetTickCount();
		int sleepTime = 1000.0 / 60.0 - (e - s);
		Sleep(sleepTime > 0 ? sleepTime : 0);
		//InvalidateRect(hWnd, NULL, false);
	}
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。
	
    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ANNESLIE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ANNESLIE));

	auto hThread = CreateThread(
		NULL,//default security attributes
		0,//use default stack size
		ProcessFrame,//thread function
		0,//argument to thread function
		0,//use default creation flags
		NULL);//returns the thread identifier

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ANNESLIE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ANNESLIE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   ::hWnd = hWnd;


   hDc = GetDC(hWnd);
   hMemDc= CreateCompatibleDC(hDc);

   bmp = CreateCompatibleBitmap(hDc, GAME_WIDTH, GAME_HEIGHT);
   SelectObject(hMemDc, bmp);

   bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
   bmpInfo.bmiHeader.biWidth = GAME_WIDTH;
   bmpInfo.bmiHeader.biHeight = GAME_HEIGHT;
   bmpInfo.bmiHeader.biPlanes = 1;
   bmpInfo.bmiHeader.biBitCount = 24;
   bmpInfo.bmiHeader.biCompression = BI_RGB;
   bmpInfo.bmiHeader.biSizeImage = 0;
   bmpInfo.bmiHeader.biXPelsPerMeter = 3000;
   bmpInfo.bmiHeader.biYPelsPerMeter = 3000;
   bmpInfo.bmiHeader.biClrUsed = 0;
   bmpInfo.bmiHeader.biClrImportant = 0;

   //SetDIBits(hMemDc, bmp, 0, GAME_HEIGHT, emulator.RawBitmap, &bmpInfo, DIB_RGB_COLORS);



   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
   // case WM_PAINT:
   //     {
   //         PAINTSTRUCT ps;
   //         HDC hdc = BeginPaint(hWnd, &ps);

			//BitBlt(hdc, 0, 0, GAME_WIDTH, GAME_HEIGHT, hMemDc, 0, 0, SRCCOPY);

   //         EndPaint(hWnd, &ps);
   //     }
   //     break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

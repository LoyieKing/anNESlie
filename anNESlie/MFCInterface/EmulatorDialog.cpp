// EmulatorDialog.cpp: 实现文件
//
#include "resource.h"
#include "framework.h"
#include "EmulatorDialog.h"
#include "afxdialogex.h"


// EmulatorDialog 对话框

IMPLEMENT_DYNAMIC(EmulatorDialog, CDialogEx)

EmulatorDialog::EmulatorDialog(const char* file_path, CWnd* pParent)
	: CDialogEx(IDD_EmulatorDialog, pParent), emulator(file_path)
{
	stop = false;
	end = false;

	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = GAME_WIDTH;
	bmpInfo.bmiHeader.biHeight = -GAME_HEIGHT;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 24;
	bmpInfo.bmiHeader.biCompression = BI_RGB;
	bmpInfo.bmiHeader.biSizeImage = 0;
	bmpInfo.bmiHeader.biXPelsPerMeter = 3000;
	bmpInfo.bmiHeader.biYPelsPerMeter = 3000;
	bmpInfo.bmiHeader.biClrUsed = 0;
	bmpInfo.bmiHeader.biClrImportant = 0;

	//mainMenu.Detach();
}

EmulatorDialog::~EmulatorDialog()
{
	end = true;
	WaitForSingleObject(hProcessFrameThread, INFINITE);
	CloseHandle(hProcessFrameThread);
}

void EmulatorDialog::bltGameOutput()
{
	SetDIBits(memDc.m_hDC, bmp, 0, GAME_HEIGHT, emulator.ScreenOutput, &bmpInfo, DIB_RGB_COLORS);
}

void EmulatorDialog::paint()
{
	hDc->StretchBlt(0, 0, clientRect.right, clientRect.bottom, &memDc, 0, 0, GAME_WIDTH, GAME_HEIGHT, SRCCOPY);

}

void EmulatorDialog::onStopMenu()
{
	CMenu stopMenu;
	stopMenu.LoadMenu(IDM_STOP);
	if (stop)
	{
		stop = false;
		gameMenu.ModifyMenu(0, MF_BYPOSITION , IDM_STOP, _T("暂停游戏"));
	}
	else
	{
		stop = true;
		gameMenu.ModifyMenu(0, MF_BYPOSITION , IDM_STOP, _T("继续游戏"));
	}
}

void EmulatorDialog::onSaveMenu()
{
	stop = true;

	CString strFile;

	CFileDialog dlgFile(FALSE, _T(".sav"), NULL, OFN_HIDEREADONLY, _T("NES存档文件 (*.sav)|*.sav|所有文件 (*.*)|*.*||"), NULL);

	if (dlgFile.DoModal() != IDOK)
	{
		stop = false;
		return;
	}

	strFile = dlgFile.GetPathName();


	USES_CONVERSION;
	char* path = T2A(strFile.GetBuffer(0));
	strFile.ReleaseBuffer();

	emulator.SaveGame(path);

	stop = false;
}

void EmulatorDialog::onLoadMenu()
{
	stop = true;

	CString strFile;

	CFileDialog dlgFile(TRUE, _T(".sav"), NULL, OFN_HIDEREADONLY, _T("NES存档文件 (*.sav)|*.sav|所有文件 (*.*)|*.*||"), NULL);

	if (dlgFile.DoModal() != IDOK)
	{
		stop = false;
		return;
	}

	strFile = dlgFile.GetPathName();

	USES_CONVERSION;
	char* path = T2A(strFile.GetBuffer(0));
	strFile.ReleaseBuffer();

	emulator.LoadGame(path);

	stop = false;
}

void EmulatorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(EmulatorDialog, CDialogEx)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_COMMAND(IDM_STOP,onStopMenu)
	ON_COMMAND(IDM_SAVE,onSaveMenu)
	ON_COMMAND(IDM_LOAD,onLoadMenu)
	ON_WM_CREATE()
END_MESSAGE_MAP()



DWORD WINAPI ProcessFrame(LPVOID param)
{
	EmulatorDialog* emulatorDialog = (EmulatorDialog*)param;
	while (true)
	{
		if (emulatorDialog->emulator.isNull())
			continue;
		int s = GetTickCount();

		if (emulatorDialog->end)
			return TRUE;
		if (!emulatorDialog->stop)
		{

			emulatorDialog->emulator.ProcessFrame();
			emulatorDialog->bltGameOutput();

			//emulatorDialog->bltGameOutput();
			//emulatorDialog->OnPaint();
		}
		emulatorDialog->paint();

		int e = GetTickCount();
		int sleepTime = 1000.0 / 50.0 - (e - s);
		Sleep(sleepTime > 0 ? sleepTime : 0);
	}
}


BOOL EmulatorDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	 

	hDc = GetDC();
	memDc.CreateCompatibleDC(hDc);
	bmp.CreateCompatibleBitmap(hDc, GAME_WIDTH, GAME_HEIGHT);
	memDc.SelectObject(bmp);

	
	//SmemDc.SetStretchBltMode(HALFTONE);
	hDc->SetStretchBltMode(HALFTONE);
	//SetStretchBltMode(memDc.m_hDC, STRETCH_ORSCANS);

	GetClientRect(&clientRect);

	SetThreadAffinityMask(GetCurrentThread(), 0);
	hProcessFrameThread = CreateThread(
		NULL,//default security attributes
		0,//use default stack size
		ProcessFrame,//thread function
		this,//argument to thread function
		CREATE_SUSPENDED,//use default creation flags
		NULL);//returns the thread identifier
	if (hProcessFrameThread == NULL)
		return false;
	SetThreadAffinityMask(hProcessFrameThread, 1);
	SetThreadPriority(hProcessFrameThread, THREAD_PRIORITY_HIGHEST);
	ResumeThread(hProcessFrameThread);



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void EmulatorDialog::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	//				   // TODO: 在此处添加消息处理程序代码
	//				   // 不为绘图消息调用 CDialogEx::OnPaint()
	//hDc->StretchBlt(0, 0, clientRect.right, clientRect.bottom, &memDc, 0, 0, GAME_WIDTH, GAME_HEIGHT, SRCCOPY);
}


void EmulatorDialog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
	emulator.KeyDown(nChar);
}


void EmulatorDialog::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CDialogEx::OnKeyUp(nChar, nRepCnt, nFlags);
	emulator.KeyUp(nChar);
}


BOOL EmulatorDialog::PreTranslateMessage(MSG* pMsg)
{
	// allow tooltip messages to be filtered
	if (CWnd::PreTranslateMessage(pMsg))
		return TRUE;

	// filter both messages to dialog and from children
	return FALSE;
}


//BOOL EmulatorDialog::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
//{
//	// TODO: 在此添加专用代码和/或调用基类
//
//	return CDialogEx::Create(lpszTemplateName, pParentWnd);
//}


int EmulatorDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	mainMenu.CreateMenu();
	//saveMenu.CreateMenu();
	gameMenu.CreateMenu();

	//saveMenu.AppendMenu(MF_STRING, IDM_SAVE, _T("保存存档"));
	//saveMenu.AppendMenu(MF_STRING, IDM_LOAD, _T("读取存档"));

	gameMenu.AppendMenu(MF_STRING, IDM_STOP, _T("暂停游戏"));

	//mainMenu.AppendMenu(MF_POPUP, (UINT)saveMenu.m_hMenu, _T("存档"));
	mainMenu.AppendMenu(MF_POPUP, (UINT)gameMenu.m_hMenu, _T("游戏"));

	SetMenu(&mainMenu);
	//saveMenu.Detach();
	//gameMenu.Detach();
	//mainMenu.Detach();

	return 0;
}

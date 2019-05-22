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

}

EmulatorDialog::~EmulatorDialog()
{
	stop = true;
	Sleep(50);
	CloseHandle(hThread);
}

void EmulatorDialog::bltGameOutput()
{
	SetDIBits(memDc.GetSafeHdc(), bmp, 0, GAME_HEIGHT, emulator.ScreenOutput, &bmpInfo, DIB_RGB_COLORS);
}

void EmulatorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(EmulatorDialog, CDialogEx)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
END_MESSAGE_MAP()



DWORD WINAPI ProcessFrame(LPVOID param)
{
	EmulatorDialog* emulatorDialog = (EmulatorDialog*)param;
	while (true)
	{
		if (emulatorDialog->emulator.isNull())
			continue;
		int s = GetTickCount();

		if (!emulatorDialog->stop)
		{
			emulatorDialog->emulator.ProcessFrame();
			emulatorDialog->bltGameOutput();
			emulatorDialog->OnPaint();
		}


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

	GetClientRect(&clientRect);

	hThread = CreateThread(
		NULL,//default security attributes
		0,//use default stack size
		ProcessFrame,//thread function
		this,//argument to thread function
		0,//use default creation flags
		NULL);//returns the thread identifier

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void EmulatorDialog::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	//				   // TODO: 在此处添加消息处理程序代码
	//				   // 不为绘图消息调用 CDialogEx::OnPaint()
	hDc->StretchBlt(0, 0, clientRect.right, clientRect.bottom, &memDc, 0, 0, GAME_WIDTH, GAME_HEIGHT, SRCCOPY);
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

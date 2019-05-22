#include "framework.h"
#include "WelcomeDialog.h"
#include "resource.h"


WelcomeDialog::WelcomeDialog(CWnd* pParent/*=nullptr*/) :
	CDialogEx(IDD_WELCOMEDIALOG, pParent)
{

	//m_hIcon = AfxGetApp()->LoadIcon(IDI_SMALL);
}

void WelcomeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, listBox);
}

BEGIN_MESSAGE_MAP(WelcomeDialog, CDialogEx)
	ON_WM_PAINT()
	ON_LBN_DBLCLK(IDC_LIST1, &WelcomeDialog::OnDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON1, &WelcomeDialog::OnClickedButton1)
END_MESSAGE_MAP()



void WelcomeDialog::initList()
{
	files.clear();
	listBox.ResetContent();

	WIN32_FIND_DATA fileData;
	HANDLE hFind = FindFirstFile(L"game\\*.nes", &fileData);
	if ((int)hFind == 0xffffffff)
		return;
	do
	{
		if (!(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			listBox.AddString(fileData.cFileName);
		}
	} while (FindNextFile(hFind, &fileData));
}

void WelcomeDialog::loadGame()
{
	int index = listBox.GetCurSel();
	if (index < 0 || index >= listBox.GetCount())
		return;
	WCHAR wstr[256];
	StrCpyW(wstr, L"game\\");
	listBox.GetText(index, wstr + 5);
	char str[256];

}

BOOL WelcomeDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	initList();

	return TRUE;
}

void WelcomeDialog::OnPaint()
{
	CDialogEx::OnPaint();

}

HCURSOR WelcomeDialog::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void WelcomeDialog::OnDblclkList1()
{
	loadGame();
}


void WelcomeDialog::OnClickedButton1()
{
	loadGame();
}

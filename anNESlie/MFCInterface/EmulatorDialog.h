#pragma once
#include "resource.h"
#include "framework.h"
#include "../Emulator/Emulator.h"

// EmulatorDialog 对话框

class EmulatorDialog : public CDialogEx
{
	DECLARE_DYNAMIC(EmulatorDialog)

public:
	friend DWORD WINAPI ProcessFrame(LPVOID param);
	//friend DWORD WINAPI PaintFrame(LPVOID param);

	EmulatorDialog(const char* file_path, CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~EmulatorDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EmulatorDialog };
#endif

private:
	const static int IDM_SAVE = 2301;
	const static int IDM_LOAD = 2302;
	const static int IDM_STOP = 2303;

	CDC memDc;
	CDC* hDc;
	CBitmap bmp;
	BITMAPINFO bmpInfo;

	HANDLE hProcessFrameThread;
	//HANDLE hPaintFrameThread;

	bool stop;
	bool end;
	Emulator emulator;

	CRect clientRect;

	void bltGameOutput();
	void paint();


	CMenu mainMenu;
	CMenu saveMenu;
	CMenu gameMenu;

	void onStopMenu();
	void onSaveMenu();
	void onLoadMenu();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

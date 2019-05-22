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
	EmulatorDialog(const char* file_path, CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~EmulatorDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EmulatorDialog };
#endif

private:
	CDC memDc;
	CDC* hDc;
	CBitmap bmp;
	BITMAPINFO bmpInfo;

	HANDLE hThread;
	bool stop;
	Emulator emulator;

	CRect clientRect;

	void bltGameOutput();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

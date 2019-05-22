#pragma once
#include "resource.h"
#include <afxdialogex.h>
#include <vector>


class WelcomeDialog :
	public CDialogEx
{
	// 构造
public:
	WelcomeDialog(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WELCOMEDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
private:
	std::vector<CString> files;

	void initList();
	void loadGame();

protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:

	CListBox listBox;
	afx_msg void OnDblclkList1();
	afx_msg void OnClickedButton1();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


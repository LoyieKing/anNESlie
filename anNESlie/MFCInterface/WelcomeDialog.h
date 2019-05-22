#pragma once
#include "resource.h"
#include <afxdialogex.h>
#include <vector>


class WelcomeDialog :
	public CDialogEx
{
	// ����
public:
	WelcomeDialog(CWnd* pParent = nullptr);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WELCOMEDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
private:
	std::vector<CString> files;

	void initList();
	void loadGame();

protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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


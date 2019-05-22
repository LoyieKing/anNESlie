#pragma once
#include <afxwin.h>

class MainApp :
	public CWinApp
{
public:
	MainApp();
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()


};

extern MainApp mainApp;
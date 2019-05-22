#include "framework.h"
#include "WelcomeDialog.h"
#include "MainApp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

MainApp mainApp;

BEGIN_MESSAGE_MAP(MainApp, CWinApp)

END_MESSAGE_MAP()

MainApp::MainApp()
{

}

BOOL MainApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);

	
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	CShellManager* pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));


	// TODO: Dialogs
	WelcomeDialog welcomeDialog;
	welcomeDialog.DoModal();

	if (pShellManager != nullptr)
	{
		delete pShellManager;
}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	return FALSE;
}

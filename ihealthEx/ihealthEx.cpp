#include "UILib.h"

#include "MainWnd.h"

using namespace DuiLib;

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "..\\lib\\DuiLib_d.lib")
#   else
#       pragma comment(lib, "..\\lib\\DuiLibA_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "..\\Lib\\DuiLib.lib")
#   else
#       pragma comment(lib, "..\\lib\\DuiLibA.lib")
#   endif
#endif


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR lpCmdLine, int nCmdShow) {
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	CMainWnd mainWnd;
	mainWnd.Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	mainWnd.CenterWindow();
	mainWnd.ShowModal();
	return 0;
}
#include "MainWnd.h"

using namespace DuiLib;

LPCTSTR CMainWnd::GetWindowClassName() const {
	return _T("CMainWnd");
}

CDuiString CMainWnd::GetSkinFile() {
	return _T("main_wnd.xml");
}
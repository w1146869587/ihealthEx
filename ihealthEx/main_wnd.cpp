#include "main_wnd.h"

using namespace DuiLib;

LPCTSTR CMainWnd::GetWindowClassName() const {
	return _T("CMainWnd");
}

CDuiString CMainWnd::GetSkinFile() {
	return _T("main_wnd.xml");
}

void CMainWnd::InitWindow() {
	m_pStartBtn = static_cast<CButtonUI *>(m_pm.FindControl(_T("btnStart")));
	m_pStopBtn = static_cast<CButtonUI *>(m_pm.FindControl(_T("btnStop")));
}

void CMainWnd::Notify(TNotifyUI &msg) {
	CDuiString name = msg.pSender->GetName();
	if (msg.sType == _T("click")) {
		if (name.CompareNoCase(_T("btnStart")) == 0) {
			::MessageBox(NULL, _T("Start"), _T("title"), MB_OK);
		} else if (name.CompareNoCase(_T("btnStop")) == 0) {
			::MessageBox(NULL, _T("Stop"), _T("title"), MB_OK);
		}
	}
}
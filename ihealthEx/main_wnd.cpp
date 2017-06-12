#include "main_wnd.h"

using namespace DuiLib;

LPCTSTR CMainWnd::GetWindowClassName() const {
	return _T("CMainWnd");
}

CDuiString CMainWnd::GetSkinFile() {
	return _T("main_wnd.xml");
}

void CMainWnd::InitWindow() {
	m_pInitialBtn = static_cast<CButtonUI *>(m_pm.FindControl(_T("btnInitial")));
	m_pPositiveBtn = static_cast<CButtonUI *>(m_pm.FindControl(_T("btnPositive")));
	m_pNegativeBtn = static_cast<CButtonUI *>(m_pm.FindControl(_T("btnNegative")));
	m_pStopBtn = static_cast<CButtonUI *>(m_pm.FindControl(_T("btnStop")));
	/*AllocConsole();
	freopen("CONOUT$", "w", stdout);*/
}

I32 interrupt_no;
void CMainWnd::Notify(TNotifyUI &msg) {
	CDuiString name = msg.pSender->GetName();
	if (msg.sType == _T("click")) {
		if (name.CompareNoCase(_T("btnInitial")) == 0) {
			m_pPassiveMode = new PassiveMode();
			m_pPassiveMode->PositionReset();
		} else if (name.CompareNoCase(_T("btnPositive")) == 0) {
			PassiveActionParam data(0.0, 25.0, 0.0, 20.0, 100);
			m_pPassiveMode->StartMove(data);
		} else if (name.CompareNoCase(_T("btnNegative")) == 0) {
			m_pPassiveMode->PositionReset();
		} else if (name.CompareNoCase(_T("btnStop")) == 0) {
			m_pPassiveMode->StopMove();
		}
	}
}
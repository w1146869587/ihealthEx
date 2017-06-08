#include "main_wnd.h"
#include "control_card.h"

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
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
}

I32 interrupt_no;
void CMainWnd::Notify(TNotifyUI &msg) {
	CDuiString name = msg.pSender->GetName();
	if (msg.sType == _T("click")) {
		if (name.CompareNoCase(_T("btnInitial")) == 0) {
			m_pControlCard = new ControlCard();
			m_pControlCard->Initial();
			m_pControlCard->PositionReset();

	
			I32 ret;
			I32 factor = (1 << 12);
			interrupt_no = APS_set_int_factor(0, 0, factor, 1);
			APS_int_enable(0, 1);
		} else if (name.CompareNoCase(_T("btnPositive")) == 0) {
			m_pControlCard->SetClutch(CLUTCH_ON);
			m_pControlCard->SetMotor(MOTOR_ON);
			APS_absolute_move(0, 2 / 0.009, 2 / 0.009);
			APS_wait_single_int(interrupt_no, -1);
			AllocConsole();
			freopen("CONOUT$", "w", stdout);
			printf("be in place");
		} else if (name.CompareNoCase(_T("btnNegative")) == 0) {
			
		} else if (name.CompareNoCase(_T("btnStop")) == 0) {
			m_pControlCard->PositionReset();
		}
	}
}
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
}

void CMainWnd::Notify(TNotifyUI &msg) {
	CDuiString name = msg.pSender->GetName();
	if (msg.sType == _T("click")) {
		if (name.CompareNoCase(_T("btnInitial")) == 0) {
			m_pControlCard = new ControlCard();
			m_pControlCard->Initial();
		} else if (name.CompareNoCase(_T("btnPositive")) == 0) {
			m_pControlCard->SetClutch();
			m_pControlCard->SetMotor();
			m_pControlCard->MotorMove(ELBOW_AXIS_ID, 0.5);
		} else if (name.CompareNoCase(_T("btnNegative")) == 0) {
			m_pControlCard->SetClutch();
			m_pControlCard->SetMotor();
			m_pControlCard->MotorMove(ELBOW_AXIS_ID, -0.5);
		} else if (name.CompareNoCase(_T("btnStop")) == 0) {
			m_pControlCard->SetClutch(CLUTCH_OFF);
			m_pControlCard->SetMotor(MOTOR_OFF);
		}
	}
}
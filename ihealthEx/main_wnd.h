#pragma once
#include "UIlib.h"

#include "control_card.h"
#include "passive_mode.h"

class CMainWnd : public DuiLib::WindowImplBase {
//UI≥ı ºªØ
public:
	virtual LPCTSTR GetWindowClassName() const;
	virtual DuiLib::CDuiString GetSkinFile();
	void InitWindow();

public:
	void Notify(DuiLib::TNotifyUI &msg);

private:
	ControlCard *m_pControlCard;
	PassiveMode *m_pPassiveMode;
	DuiLib::CButtonUI *m_pInitialBtn;
	DuiLib::CButtonUI *m_pPositiveBtn;
	DuiLib::CButtonUI *m_pNegativeBtn;
	DuiLib::CButtonUI *m_pStopBtn;
};
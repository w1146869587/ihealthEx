#pragma once
#include "UIlib.h"

class CMainWnd : public DuiLib::WindowImplBase {
//UI≥ı ºªØ
public:
	virtual LPCTSTR GetWindowClassName() const;
	virtual DuiLib::CDuiString GetSkinFile();
	void InitWindow();

public:
	void Notify(DuiLib::TNotifyUI &msg);

private:
	DuiLib::CButtonUI *m_pStartBtn;
	DuiLib::CButtonUI *m_pStopBtn;
};
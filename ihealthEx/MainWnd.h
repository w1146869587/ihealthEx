#pragma once
#include "UILib.h"

class CMainWnd : public DuiLib::WindowImplBase {
public:
	virtual LPCTSTR GetWindowClassName() const;

	virtual DuiLib::CDuiString GetSkinFile();
};
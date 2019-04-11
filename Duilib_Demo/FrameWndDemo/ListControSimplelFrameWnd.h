#pragma once
#include <UIlib.h>
using namespace DuiLib;

//////////////////////////////////////////////////////////////////////////
// 该类用于学习ListControl控件
//////////////////////////////////////////////////////////////////////////

class CListControlSimpleFrameWnd : public WindowImplBase
{
public:
	CListControlSimpleFrameWnd(void);
	virtual ~CListControlSimpleFrameWnd(void);

	virtual LPCTSTR GetWindowClassName() const ;
	virtual void OnFinalMessage(HWND hWnd);
	virtual void InitWindow();
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);

	virtual void    Notify(TNotifyUI& msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

};


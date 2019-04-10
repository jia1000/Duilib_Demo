#pragma once
#include <UIlib.h>
using namespace DuiLib;

class CFirstFrameWnd : public CWindowWnd, public INotifyUI
{
public:
	CFirstFrameWnd(void);
	~CFirstFrameWnd(void);

	virtual LPCTSTR GetWindowClassName() const ;
	virtual void    Notify(TNotifyUI& msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	CPaintManagerUI m_PaintManager;
};


#include "FirstFrameWnd.h"


CFirstFrameWnd::CFirstFrameWnd(void)
{
}


CFirstFrameWnd::~CFirstFrameWnd(void)
{
}

LPCTSTR CFirstFrameWnd::GetWindowClassName() const 
{ 
	return _T("DUIMainFrame"); 
}

void    CFirstFrameWnd::Notify(TNotifyUI& msg) 
{

}

LRESULT CFirstFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;

	if( uMsg == WM_CREATE ) 
	{
		CControlUI *pWnd = new CButtonUI;
		pWnd->SetText(_T("Hello World"));   // 设置文字
		pWnd->SetBkColor(0xFF00FF00);       // 设置背景色

		m_PaintManager.Init(m_hWnd);
		m_PaintManager.AttachDialog(pWnd);
		return lRes;
	}

	if( m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) 
	{
		return lRes;
	}

	return __super::HandleMessage(uMsg, wParam, lParam);
}

#include "FirstFrameWnd.h"


CFirstFrameWnd::CFirstFrameWnd(void)
{
}


CFirstFrameWnd::~CFirstFrameWnd(void)
{
}

LPCTSTR CFirstFrameWnd::GetWindowClassName() const 
{ 
	return _T("FirstFrameWnd"); 
}

void    CFirstFrameWnd::Notify(TNotifyUI& msg) 
{

}

LRESULT CFirstFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;

	if( uMsg == WM_CREATE ) {
		CControlUI *pWnd = new CButtonUI;
		pWnd->SetName(_T("btnHello"));		// 设置控件的名称，这个名称用于标识每一个控件，必须唯一
		pWnd->SetText(_T("Hello World in first"));   // 设置文字
		pWnd->SetBkColor(0xFF00FF00);       // 设置背景色
		
		m_PaintManager.Init(m_hWnd);
		m_PaintManager.AttachDialog(pWnd);
		// 添加控件等消息响应，这样消息就会传达到duilib的消息循环，
		// 我们可以在Notify函数里做消息处理
		m_PaintManager.AddNotifier(this);
		return lRes;
	}  
	// 以下3个消息WM_NCACTIVATE、WM_NCCALCSIZE、WM_NCPAINT用于屏蔽系统标题栏
	//else if (uMsg == WM_NCACTIVATE) {
	//	if (!::IsIconic(m_hWnd)) {
	//		return (wParam == 0) ? true :false;
	//	}
	//} else if (uMsg == WM_NCCALCSIZE) {
	//	return 0;
	//} else if (uMsg == WM_NCPAINT) {
	//	return 0;
	//}

	if( m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) {
		return lRes;
	}

	return __super::HandleMessage(uMsg, wParam, lParam);
}

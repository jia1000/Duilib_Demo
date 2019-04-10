#include "SecondFrameWnd.h"


CSecondFrameWnd::CSecondFrameWnd(void)
{
}


CSecondFrameWnd::~CSecondFrameWnd(void)
{
}

LPCTSTR CSecondFrameWnd::GetWindowClassName() const 
{ 
	return _T("SecondFrameWnd"); 
}

void    CSecondFrameWnd::Notify(TNotifyUI& msg) 
{
	if (msg.sType == _T("click")) {
		LPCTSTR pszCtrlName = msg.pSender->GetName().GetData();
		if (_tcscmp(pszCtrlName, _T("btn_sysmin")) == 0) {
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		} else if (_tcscmp(pszCtrlName, _T("btn_sysmax")) == 0) {
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
			// 最大化和还原2个button之间的切换
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("btn_sysmax")));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("btn_restore")));
			if( pControl ) pControl->SetVisible(true);
		} else if (_tcscmp(pszCtrlName, _T("btn_restore")) == 0) {
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
			// 最大化和还原2个button之间的切换
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("btn_sysmax")));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("btn_restore")));
			if( pControl ) pControl->SetVisible(false);
		} else if (_tcscmp(pszCtrlName, _T("btn_sysclose")) == 0) {
			Close(IDOK);
		}
	}
}

LRESULT CSecondFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;

	if( uMsg == WM_CREATE ) {
		m_PaintManager.Init(m_hWnd);

		// 此处时不同点
		//CControlUI *pWnd = new CButtonUI;
		//pWnd->SetName(_T("btnHello"));	// 设置控件的名称，这个名称用于标识每一个控件，必须唯一
		//pWnd->SetText(_T("Hello World")); // 设置文字
		//pWnd->SetBkColor(0xFF00FF00);     // 设置背景色
		
		CDialogBuilder builder;
		CControlUI* pWnd = builder.Create(_T("frame_wnd_demo\\second_frame.xml"), (UINT)0, NULL, &m_PaintManager);

		ASSERT(pWnd && "Failed to parse XML");

		m_PaintManager.AttachDialog(pWnd);
		// 添加控件等消息响应，这样消息就会传达到duilib的消息循环，
		// 我们可以在Notify函数里做消息处理
		m_PaintManager.AddNotifier(this);

		return lRes;
	}  
	// 以下3个消息WM_NCACTIVATE、WM_NCCALCSIZE、WM_NCPAINT用于屏蔽系统标题栏
	else if (uMsg == WM_NCACTIVATE) {
		if (!::IsIconic(m_hWnd)) {
			return (wParam == 0) ? true :false;
		}
	} else if (uMsg == WM_NCCALCSIZE) {
		return 0;
	} else if (uMsg == WM_NCPAINT) {
		return 0;
	}

	if( m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) {
		return lRes;
	}

	return __super::HandleMessage(uMsg, wParam, lParam);
}

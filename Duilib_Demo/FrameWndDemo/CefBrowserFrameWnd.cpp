#include "CefBrowserFrameWnd.h"

#include "cef/simple_app.h"
#include "cef/simple_handler.h"

// cef 全局变量
CefRefPtr<SimpleHandler> g_handler(new SimpleHandler());

CCefBrowserFrameWnd::CCefBrowserFrameWnd(void)
	: m_bIsInit(false)
{
}

CCefBrowserFrameWnd::~CCefBrowserFrameWnd(void)
{
	PostQuitMessage(0);
}

LPCTSTR CCefBrowserFrameWnd::GetWindowClassName() const 
{ 
	return _T("CefBrowserFrameWnd"); 
}

void CCefBrowserFrameWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	//delete this; //退出时，会有异常
}

void CCefBrowserFrameWnd::InitWindow()
{
	m_bIsInit = TRUE;
	FirstLoadCef();
}

CControlUI* CCefBrowserFrameWnd::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

CDuiString CCefBrowserFrameWnd::GetSkinFile()
{
	return _T("frame_wnd_demo\\cef_browser_frame.xml");
}

CDuiString CCefBrowserFrameWnd::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");

}

LRESULT CCefBrowserFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	AdjustCefWindow();
	__super::OnSize(uMsg, wParam, lParam, bHandled);
	return 0;
}

void    CCefBrowserFrameWnd::Notify(TNotifyUI& msg) 
{
	if (msg.sType == _T("click")) {
		LPCTSTR pszCtrlName = msg.pSender->GetName().GetData();
		if (_tcscmp(pszCtrlName, _T("btn_sysmin")) == 0) {
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		} else if (_tcscmp(pszCtrlName, _T("btn_sysmax")) == 0) {
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
			// 最大化和还原2个button之间的切换
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_sysmax")));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_restore")));
			if( pControl ) pControl->SetVisible(true);
		} else if (_tcscmp(pszCtrlName, _T("btn_restore")) == 0) {
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
			// 最大化和还原2个button之间的切换
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_sysmax")));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_restore")));
			if( pControl ) pControl->SetVisible(false);
		} else if (_tcscmp(pszCtrlName, _T("btn_sysclose")) == 0) {
			Close(IDOK);
		}
	} else if (_tcsicmp(msg.sType, _T("selectchanged")) == 0) {
		OnSelChanged(msg.pSender);
	}
}

LRESULT CCefBrowserFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;

	BOOL bHandled = TRUE;
	switch (uMsg)
	{
	case WM_SIZE:
		OnSize(uMsg, wParam, lParam, bHandled);
		break;
	}

	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) {
		return lRes;
	}

	return __super::HandleMessage(uMsg, wParam, lParam);
}

void CCefBrowserFrameWnd::FirstLoadCef()
{
	CTabLayoutUI* pTabTest = static_cast <CTabLayoutUI*>(m_pm.FindControl(_T("tabTest")));
	COptionUI *pCefControlTest = static_cast<COptionUI*>(m_pm.FindControl(_T("CEF_Control_Test")));
	if (pTabTest == NULL)
	{
		return;
	}
	RECT rect;
	GetClientRect(this->m_hWnd, &rect);
	int nCx = GetSystemMetrics(SM_CXFULLSCREEN);
	int nCy = GetSystemMetrics(SM_CYFULLSCREEN);

	RECT rt;
	rt.left = rect.left+ 140;
	rt.top =  rect.top + 60;

	rt.bottom = rt.top + 600-60;
	rt.right = rt.left + 925-140;

	CefBrowserSettings browser_settings;
	CefWindowInfo window_info;
	window_info.SetAsChild(m_hWnd, rt);
	BOOL bSucced = CefBrowserHost::CreateBrowser(window_info
		, g_handler
		//, _T("http://dict.youdao.com/?keyfrom=cidian")
		, _T("http://www.baidu.com")
		, browser_settings
		, NULL);
	pTabTest->SelectItem(0);
	pCefControlTest->Selected(TRUE);
}

void CCefBrowserFrameWnd::AdjustCefWindow()
{
	if (m_bIsInit)
	{
		CefRefPtr<CefBrowser> browser = g_handler->GetCurrentBrowser();
		if (browser != NULL)
		{
			CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
			if (hwnd)
			{
				RECT rect;
				GetClientRect(this->m_hWnd, &rect);
				int nWidth = rect.right - rect.left - 140;
				int nHeight = rect.bottom - rect.top - 60;
				::MoveWindow(hwnd,140,60, nWidth, nHeight, TRUE);
			}
		}
	}
}

void CCefBrowserFrameWnd::HideCefWindow()
{
	CefRefPtr<CefBrowser> browser = g_handler->GetCurrentBrowser();
	if (browser != NULL)
	{
		CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
		if (hwnd)
		{
			::ShowWindow(hwnd, SW_HIDE);
		}
	}
}

void CCefBrowserFrameWnd::OnSelChanged(CControlUI* pSender)
{
	CTabLayoutUI* pTabTest = static_cast <CTabLayoutUI*>(m_pm.FindControl(_T("tabTest")));
	COptionUI *pCefControlTest = static_cast<COptionUI*>(m_pm.FindControl(_T("CEF_Control_Test")));
	if (pTabTest == NULL)
	{
		return;
	}
	CDuiString strSelName = pSender->GetName();
	if (strSelName == _T("CEF_Control_Test")) 
	{
		CefRefPtr<CefBrowser> browser = g_handler->GetCurrentBrowser();
		if (browser != NULL)
		{
			CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
			if (hwnd)
			{
				::ShowWindow(hwnd, SW_SHOW);
				browser->GetMainFrame()->LoadURL(_T("http://dict.youdao.com/?keyfrom=cidian"));
			}
		}
		pTabTest->SelectItem(0);
	} else if (strSelName == _T("OptionDemo2")) {
		HideCefWindow();
		pTabTest->SelectItem(1);
	}
}
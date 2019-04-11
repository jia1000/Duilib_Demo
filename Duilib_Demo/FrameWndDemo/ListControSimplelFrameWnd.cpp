#include "ListControSimplelFrameWnd.h"

CListControlSimpleFrameWnd::CListControlSimpleFrameWnd(void)
{
}

CListControlSimpleFrameWnd::~CListControlSimpleFrameWnd(void)
{
	PostQuitMessage(0);
}

LPCTSTR CListControlSimpleFrameWnd::GetWindowClassName() const 
{ 
	return _T("CefBrowserFrameWnd"); 
}

void CListControlSimpleFrameWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this; 
}

void CListControlSimpleFrameWnd::InitWindow()
{
	CDuiString str;
	CListUI* pList = static_cast<CListUI*>(m_pm.FindControl(_T("ListDemo1")));

	// 添加List列表内容，必须先Add(pListElement)，再SetText
	for (int i = 0; i < 100; i++)
	{
		CListTextElementUI* pListElement = new CListTextElementUI;
		pListElement->SetTag(i);
		pList->Add(pListElement);

		str.Format(_T("%d"), i);
		pListElement->SetText(0, str);
		pListElement->SetText(1, _T("haha"));
	}
}

CControlUI* CListControlSimpleFrameWnd::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

CDuiString CListControlSimpleFrameWnd::GetSkinFile()
{
	return _T("frame_wnd_demo\\list_control_simple_frame.xml");
}

CDuiString CListControlSimpleFrameWnd::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");

}

LRESULT CListControlSimpleFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//
	__super::OnSize(uMsg, wParam, lParam, bHandled);
	return 0;
}

void    CListControlSimpleFrameWnd::Notify(TNotifyUI& msg) 
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
		//
	}
}

LRESULT CListControlSimpleFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
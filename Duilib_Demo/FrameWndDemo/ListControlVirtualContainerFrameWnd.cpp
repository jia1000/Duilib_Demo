#include "ListControlVirtualContainerFrameWnd.h"

#include "extended_control/list_pro_ui.h"

CListControlVirtualContainerFrameWnd::CListControlVirtualContainerFrameWnd(void)
{
}

CListControlVirtualContainerFrameWnd::~CListControlVirtualContainerFrameWnd(void)
{
	PostQuitMessage(0);
}

LPCTSTR CListControlVirtualContainerFrameWnd::GetWindowClassName() const 
{ 
	return _T("CefBrowserFrameWnd"); 
}

void CListControlVirtualContainerFrameWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this; 
}

void CListControlVirtualContainerFrameWnd::InitWindow()
{
	CDuiString str;
	ListPro* pList = static_cast<ListPro*>(m_pm.FindControl(_T("ListDemo1")));

	// 添加List列表内容，必须先Add(pListElement)，再SetText
	for (int i = 0; i < 1000; i++)
	{
		CListTextElementUI* pListElement = new CListTextElementUI;
		pListElement->SetTag(i);
		pList->Add(pListElement);

		str.Format(_T("%d"), i);
		pListElement->SetText(0, str);
		pListElement->SetText(1, _T("hello"));
		pListElement->SetText(2, _T("man"));
		pListElement->SetText(3, _T("50"));
	}
}

CControlUI* CListControlVirtualContainerFrameWnd::CreateControl(LPCTSTR pstrClass)
{
	if ( _tcsicmp(pstrClass, _T("ListPro")) == 0 ) {
		return new ListPro;
	}
	return NULL;
}

CDuiString CListControlVirtualContainerFrameWnd::GetSkinFile()
{
	return _T("frame_wnd_demo\\list_control_virtual_container_frame.xml");
}

CDuiString CListControlVirtualContainerFrameWnd::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");

}

LRESULT CListControlVirtualContainerFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//
	__super::OnSize(uMsg, wParam, lParam, bHandled);
	return 0;
}

void    CListControlVirtualContainerFrameWnd::Notify(TNotifyUI& msg) 
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

LRESULT CListControlVirtualContainerFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
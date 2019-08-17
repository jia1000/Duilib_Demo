#include "EntryFrameWnd.h"

#include "FrameWndDemo/FirstFrameWnd.h"
#include "FrameWndDemo/SecondFrameWnd.h"
#include "FrameWndDemo/CefBrowserSimpleFrameWnd.h"
#include "FrameWndDemo/ListControSimplelFrameWnd.h"
#include "FrameWndDemo/ListControlVirtualContainerFrameWnd.h"
#include "FrameWndDemo/CefBrowserClientFrameWnd.h"
#include "FrameWndDemo/DuilibFrameDemoWnd.h"
#include "FrameWndDemo/ImageDemoFrameWnd.h"
#include "FrameWndDemo/VtkDemoFrameWnd.h"
#include "FrameWndDemo/VtkFunctionDemoFrameWnd.h"
#include "FrameWndDemo/VtkDicomViewDemoFrameWnd.h"
#include "FrameWndDemo/VtkMprDemoFrameWnd.h"
#include "FrameWndDemo/DcmtkDemoFrameWnd.h"
#include "FrameWndDemo/OpencvDemoFrameWnd.h"
#include "FrameWndDemo/JsonParseDemoFrameWnd.h"
#include "FrameWndDemo/DcmtkDLDicomDemoFrameWnd.h"

CEntryFrameWnd::CEntryFrameWnd(void)
{
}

CEntryFrameWnd::~CEntryFrameWnd(void)
{
	PostQuitMessage(0);
}

LPCTSTR CEntryFrameWnd::GetWindowClassName() const 
{ 
	return _T("CefBrowserFrameWnd"); 
}

void CEntryFrameWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this; 
}

void CEntryFrameWnd::InitWindow()
{
	
}

CControlUI* CEntryFrameWnd::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

CDuiString CEntryFrameWnd::GetSkinFile()
{
	return _T("frame_wnd_demo\\entry_frame.xml");
}

CDuiString CEntryFrameWnd::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");

}

LRESULT CEntryFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//
	__super::OnSize(uMsg, wParam, lParam, bHandled);
	return 0;
}

void    CEntryFrameWnd::Notify(TNotifyUI& msg) 
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
		} else if (_tcscmp(pszCtrlName, _T("Button1")) == 0) {
			CFirstFrameWnd* duiFrame = new CFirstFrameWnd();
			duiFrame->Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, 0L);
			duiFrame->CenterWindow();
			duiFrame->ShowWindow();
		} else if (_tcscmp(pszCtrlName, _T("Button2")) == 0) {
			CSecondFrameWnd* duiFrame = new CSecondFrameWnd();
			duiFrame->Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, 0L);
			duiFrame->CenterWindow();
			duiFrame->ShowWindow();
		} else if (_tcscmp(pszCtrlName, _T("Button3")) == 0) {
			CCefBrowserSimpleFrameWnd* duiFrame = new CCefBrowserSimpleFrameWnd();
			duiFrame->Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, 0L);
			duiFrame->CenterWindow();
			duiFrame->ShowWindow();
		} else if (_tcscmp(pszCtrlName, _T("Button4")) == 0) {
			CListControlSimpleFrameWnd* duiFrame = new CListControlSimpleFrameWnd();
			duiFrame->Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, 0L);
			duiFrame->CenterWindow();
			duiFrame->ShowWindow();
		} else if (_tcscmp(pszCtrlName, _T("Button5")) == 0) {
			CListControlVirtualContainerFrameWnd* duiFrame = new CListControlVirtualContainerFrameWnd();
			duiFrame->Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, 0L);
			duiFrame->CenterWindow();
			duiFrame->ShowWindow();
		} else if (_tcscmp(pszCtrlName, _T("Button6")) == 0) {
			CCefBrowserClientFrameWnd* duiFrame = new CCefBrowserClientFrameWnd();
			duiFrame->Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, 0L);
			duiFrame->CenterWindow();
			duiFrame->ShowWindow();
		} else if (_tcscmp(pszCtrlName, _T("Button7")) == 0) {
			COpencvDemoFrameWnd* duiFrame = new COpencvDemoFrameWnd();
			duiFrame->Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, 0L);
			duiFrame->CenterWindow();
			duiFrame->ShowWindow();
		} else if (_tcscmp(pszCtrlName, _T("Button8")) == 0) {
			CDuilibFrameDemoWnd* duiFrame = new CDuilibFrameDemoWnd();
			duiFrame->Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, 0L);
			duiFrame->CenterWindow();
			duiFrame->ShowWindow();
		} else if (_tcscmp(pszCtrlName, _T("Button9")) == 0) {
			CImageDemoFrameWnd* duiFrame = new CImageDemoFrameWnd();
			duiFrame->Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, 0L);
			duiFrame->CenterWindow();
			duiFrame->ShowWindow();
		} else if (_tcscmp(pszCtrlName, _T("Button10")) == 0) {
			CVtkDemoFrameWnd* duiFrame = new CVtkDemoFrameWnd();
			duiFrame->Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, 0L);
			duiFrame->CenterWindow();
			duiFrame->ShowWindow();
		} else if (_tcscmp(pszCtrlName, _T("Button11")) == 0) {
			CVtkFunctionDemoFrameWnd* duiFrame = new CVtkFunctionDemoFrameWnd();
			duiFrame->Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, 0L);
			duiFrame->CenterWindow();
			duiFrame->ShowWindow();
		} else if (_tcscmp(pszCtrlName, _T("Button12")) == 0) {
			CVtkDicomViewDemoFramWnd* duiFrame = new CVtkDicomViewDemoFramWnd();
			duiFrame->Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, 0L);
			duiFrame->CenterWindow();
			duiFrame->ShowWindow();
		} else if (_tcscmp(pszCtrlName, _T("Button13")) == 0) {
			CVtkMprDemoFrameWnd* duiFrame = new CVtkMprDemoFrameWnd();
			duiFrame->Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, 0L);
			duiFrame->CenterWindow();
			duiFrame->ShowWindow();
		} else if (_tcscmp(pszCtrlName, _T("Button14")) == 0) {
			CDcmtkDemoFrameWnd* duiFrame = new CDcmtkDemoFrameWnd();
			duiFrame->Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, 0L);
			duiFrame->CenterWindow();
			duiFrame->ShowWindow();
		} else if (_tcscmp(pszCtrlName, _T("Button15")) == 0) {
			CJsonParseDemoFrameWnd* duiFrame = new CJsonParseDemoFrameWnd();
			duiFrame->Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, 0L);
			duiFrame->CenterWindow();
			duiFrame->ShowWindow();
		} else if (_tcscmp(pszCtrlName, _T("Button16")) == 0) {
			DcmtkDLDicomDemoFrameWnd* duiFrame = new DcmtkDLDicomDemoFrameWnd();
			duiFrame->Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, 0L);
			duiFrame->CenterWindow();
			duiFrame->ShowWindow();
		}


	} else if (_tcsicmp(msg.sType, _T("selectchanged")) == 0) {
		//
	}
}

LRESULT CEntryFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
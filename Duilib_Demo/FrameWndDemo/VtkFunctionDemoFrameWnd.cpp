#include "VtkFunctionDemoFrameWnd.h"


CVtkFunctionDemoFrameWnd::CVtkFunctionDemoFrameWnd(void)
{
}


CVtkFunctionDemoFrameWnd::~CVtkFunctionDemoFrameWnd(void)
{
}

LPCTSTR CVtkFunctionDemoFrameWnd::GetWindowClassName() const 
{ 
	return _T("VtkDemoFrameWnd"); 
}

void CVtkFunctionDemoFrameWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this; //退出时，会有异常
}

CDuiString CVtkFunctionDemoFrameWnd::GetSkinFile()
{
	return _T("frame_wnd_demo\\vtk_function_demo_frame.xml");
}

CDuiString CVtkFunctionDemoFrameWnd::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");

}

CControlUI* CVtkFunctionDemoFrameWnd::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CVtkFunctionDemoFrameWnd::InitWindow()
{
	
}

LRESULT CVtkFunctionDemoFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	__super::OnSize(uMsg, wParam, lParam, bHandled);
	return 0;
}


void    CVtkFunctionDemoFrameWnd::Notify(TNotifyUI& msg) 
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
	}
}

LRESULT CVtkFunctionDemoFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg, wParam, lParam);
}

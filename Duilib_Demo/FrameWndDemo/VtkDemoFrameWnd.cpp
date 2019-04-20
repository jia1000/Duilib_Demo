#include "VtkDemoFrameWnd.h"

#include "extended_control/tile_layout_extended.h"
#include "vtk/vtk_window.h"

CVtkDemoFrameWnd::CVtkDemoFrameWnd(void)
{
}


CVtkDemoFrameWnd::~CVtkDemoFrameWnd(void)
{
}

LPCTSTR CVtkDemoFrameWnd::GetWindowClassName() const 
{ 
	return _T("VtkDemoFrameWnd"); 
}

void CVtkDemoFrameWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	//delete this; //退出时，会有异常
}

CDuiString CVtkDemoFrameWnd::GetSkinFile()
{
	return _T("frame_wnd_demo\\vtk_frame_demo.xml");
}

CDuiString CVtkDemoFrameWnd::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");

}

CControlUI* CVtkDemoFrameWnd::CreateControl(LPCTSTR pstrClass)
{
	if ( _tcsicmp(pstrClass, _T("TileLayoutExtended")) == 0 ) {
		return new TileLayoutExtended;
	}

	return NULL;
}

void CVtkDemoFrameWnd::InitWindow()
{
	m_pTileLayoutExtended = static_cast<TileLayoutExtended*>(m_pm.FindControl(L"vtk_window_container"));

}

LRESULT CVtkDemoFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	__super::OnSize(uMsg, wParam, lParam, bHandled);
	return 0;
}


void    CVtkDemoFrameWnd::Notify(TNotifyUI& msg) 
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
		} else if (_tcscmp(pszCtrlName, _T("btn_test")) == 0) {
			DuiVtkWindow* vtk_window = new DuiVtkWindow();
			vtk_window->Create(m_hWnd, L"DuiVtkWindow", WS_CHILD | WS_VISIBLE, 0, 0, 0, 200, 150);
			vtk_window->ShowWindow();
			static int index = 1;
			CDuiString text;
			text.Format(_T("%2d"), index);
			vtk_window->SetBtnText(text);
			index++;
			m_pTileLayoutExtended->AddVtkWindow(vtk_window);
			
		}

	}
}

LRESULT CVtkDemoFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg, wParam, lParam);
}

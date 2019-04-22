#include "VtkDicomViewDemoFrameWnd.h"

#include "vtk_dicom_demo/DicomView.h"
#include "vtk_dicom_demo/DicomSplitView.h"

CVtkDicomViewDemoFramWnd::CVtkDicomViewDemoFramWnd(void)
	: m_view(NULL)
	, m_split_view(NULL)
	, m_pbtn_vtk_show(NULL)
{
}


CVtkDicomViewDemoFramWnd::~CVtkDicomViewDemoFramWnd(void)
{
}

LPCTSTR CVtkDicomViewDemoFramWnd::GetWindowClassName() const 
{ 
	return _T("VtkDemoFrameWnd"); 
}

void CVtkDicomViewDemoFramWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this; //退出时，会有异常
}

CDuiString CVtkDicomViewDemoFramWnd::GetSkinFile()
{
	return _T("frame_wnd_demo\\vtk_dicom_view_demo_frame.xml");
}

CDuiString CVtkDicomViewDemoFramWnd::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");

}

CControlUI* CVtkDicomViewDemoFramWnd::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CVtkDicomViewDemoFramWnd::InitWindow()
{
	m_pbtn_vtk_show = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button_vtk_show")));
}

LRESULT CVtkDicomViewDemoFramWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	__super::OnSize(uMsg, wParam, lParam, bHandled);

	RECT wnd_rc;
	GetClientRect(m_hWnd, &wnd_rc);

	RECT rc;
	
	if (m_pbtn_vtk_show) {
		rc = m_pbtn_vtk_show->GetClientPos();
		rc.right = wnd_rc.right;
		rc.bottom = wnd_rc.bottom;
	}
	if (m_view) {
		m_view->UpdateView(rc);
	}
	if (m_split_view) {
		// 目前，多view的VTK，不能运行到这里。？
		m_split_view->ResizeAndPosition(rc);
	}
	return 0;
}


void    CVtkDicomViewDemoFramWnd::Notify(TNotifyUI& msg) 
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
			if (!m_view) {
				RECT rc;
				CButtonUI* pControl = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button_vtk_show")));
				if (pControl) {
					rc = pControl->GetClientPos();
				}
				m_view = new CDicomView(this->m_hWnd, vtkImageViewer2::SLICE_ORIENTATION_XY, rc);
			}
			m_view->ShowDicomFile("..\\Bin\\Skin\\data\\slices1");
		}
		else if (_tcscmp(pszCtrlName, _T("Button2")) == 0) {
			RECT rc;
			CButtonUI* pControl = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button_vtk_show")));
			if (pControl) {
				rc = pControl->GetClientPos();
			}
			m_split_view = new CDicomSplitView(m_hWnd, rc) ;
		}
	}
}

LRESULT CVtkDicomViewDemoFramWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg, wParam, lParam);
}

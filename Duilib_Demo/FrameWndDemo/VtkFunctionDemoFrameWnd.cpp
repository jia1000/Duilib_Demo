#include "VtkFunctionDemoFrameWnd.h"

#include "vtk_function_demo/VtkFunctionDemo.h"

#include "vtk_dicom_demo/DicomSplitView.h"

CVtkFunctionDemoFrameWnd::CVtkFunctionDemoFrameWnd(void)
	: m_vtk_function_demo(NULL)
{
}


CVtkFunctionDemoFrameWnd::~CVtkFunctionDemoFrameWnd(void)
{
}

LPCTSTR CVtkFunctionDemoFrameWnd::GetWindowClassName() const 
{ 
	return _T("VtkFunctionDemoFrameWnd"); 
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
	}else if (msg.sType == _T("itemclick")){        
		// 改双击收缩为单击收缩
		CTreeNodeUI * pTreeNode = static_cast<CTreeNodeUI*>(msg.pSender);
		CDuiString item_name = msg.pSender->GetName();
		if (pTreeNode && _tcsicmp(pTreeNode->GetClass(), _T("TreeNodeUI")) == 0) {
			if (pTreeNode->IsHasChild()) {
				// 如果是一级菜单
				CTreeViewUI	* pTreeView = pTreeNode->GetTreeView();
				if (pTreeView) {
					CCheckBoxUI* pFolder = pTreeNode->GetFolderButton();
					pFolder->Selected(!pFolder->IsSelected());
					pTreeNode->SetVisibleTag(!pFolder->GetCheck());
					pTreeView->SetItemExpand(!pFolder->GetCheck(), pTreeNode);
				}
				// 点击第一级菜单，会选择第二级菜单的第一个
				if (item_name.CompareNoCase(L"vtk_function_test") == 0) {
					CButtonUI* pControl = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button1")));
					if (pControl) {
						pControl->SetText(_T("Group click."));
					}
				}
			} else {
				// 如果是二级菜单
				RECT rc;
				CButtonUI* pVtkShowBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button_vtk_function_show")));
				if (pVtkShowBtn) {
					rc = pVtkShowBtn->GetClientPos();
				}
				CButtonUI* pControl = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button1")));
				if (item_name.CompareNoCase(L"function_1") == 0) {
					m_vtk_function_demo = new CVtkFunctionDemo(this->m_hWnd, rc);
					m_vtk_function_demo->Function1();
				} else if (item_name.CompareNoCase(L"function_2") == 0) {
					CDicomSplitView* split_view = new CDicomSplitView(this->m_hWnd, rc);
				} else if (item_name.CompareNoCase(L"function_3") == 0) {
					if (pControl) {
						pControl->SetText(_T("btn 2 click."));
					}
				} 
				// 其它二级菜单
			}
		}
	}
}

LRESULT CVtkFunctionDemoFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg, wParam, lParam);
}

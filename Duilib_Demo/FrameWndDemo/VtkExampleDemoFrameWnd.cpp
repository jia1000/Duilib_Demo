#include "VtkExampleDemoFrameWnd.h"

#include "vtk_function_demo/VtkFunctionDemo.h"
#include "vtk_dicom_demo/DicomSplitView.h"
#include "vtk_dicom_demo/DicomView.h"
#include "vtk_function_demo/TextWidgetsDemo.h"
#include "vtk_function_demo/CaptionWidgetsDemo.h"
#include "vtk_function_demo/BoxWidgetsDemo.h"
#include "vtk_function_demo/ContourWidgetsDemo.h"
#include "vtk_function_demo/ImagePlaneWidgetsDemo.h"
#include "vtk_function_demo/Rib3DView.h"
#include "vtk_function_demo/MultiSlicesImageDemo.h"
#include "vtk_example_demo/StructuredGrid_BlankPoint_Demo.h"
#include "vtk_example_demo/red_book_volume_of_Interest_demo.h"
#include "vtk_example_demo/reslice_demo.h"
#include "vtk_example_demo/vtkExamplesContext.h"
#include "vtk_example_demo/vtkExampleStrategy.h"

CVtkExampleDemoFrameWnd::CVtkExampleDemoFrameWnd(void)
	: m_vtk_function_demo(NULL)
	, dicom_view(NULL)
	, box_widgets(NULL)
{
}


CVtkExampleDemoFrameWnd::~CVtkExampleDemoFrameWnd(void)
{
}

LPCTSTR CVtkExampleDemoFrameWnd::GetWindowClassName() const 
{ 
	return _T("VtkFunctionDemoFrameWnd"); 
}

void CVtkExampleDemoFrameWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this; //退出时，会有异常
}

CDuiString CVtkExampleDemoFrameWnd::GetSkinFile()
{
	return _T("frame_wnd_demo\\vtk_example_demo_frame.xml");
}

CDuiString CVtkExampleDemoFrameWnd::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");

}

CControlUI* CVtkExampleDemoFrameWnd::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CVtkExampleDemoFrameWnd::InitWindow()
{
	
}

LRESULT CVtkExampleDemoFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	__super::OnSize(uMsg, wParam, lParam, bHandled);
	return 0;
}


void    CVtkExampleDemoFrameWnd::Notify(TNotifyUI& msg) 
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
				if (item_name.CompareNoCase(L"Vtk_Example_StructuredGrid") == 0) {
					CButtonUI* pControl = static_cast<CButtonUI*>(m_pm.FindControl(_T("StructuredGrid_1")));
					if (pControl) {
						pControl->SetText(_T("Group click."));
					}
				}
			} else {
				// 如果是二级菜单
				RECT rc;
				CButtonUI* pVtkShowBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("Button_vtk_function_show")));
				if (!pVtkShowBtn) {
					return;
				}
				rc = pVtkShowBtn->GetClientPos();
				if (item_name.CompareNoCase(L"StructuredGrid_1") == 0) {
					StructuredGrid_BlankPoint_Demo* demo = new StructuredGrid_BlankPoint_Demo(this->m_hWnd, pVtkShowBtn);
					demo->InitVtkExampleDemo();
					demo->ShowVtkExampleDemo();
				} else if (item_name.CompareNoCase(L"Red_Book_VolumeOfInterest") == 0) {
					Red_Book_VolumeOfInterest_Demo* demo = new Red_Book_VolumeOfInterest_Demo(this->m_hWnd, pVtkShowBtn);
					demo->InitVtkExampleDemo();
					demo->ShowVtkExampleDemo();					
				} else if (item_name.CompareNoCase(L"Red_Book_Reslice") == 0) {
					Reslice_Demo* demo = new Reslice_Demo(this->m_hWnd, pVtkShowBtn);
					demo->InitVtkExampleDemo();
					demo->ShowVtkExampleDemo();					
				} else if (item_name.CompareNoCase(L"Red_Book_Shrink_Magnify") == 0) {
					vtkExamplesContext* demo = new vtkExamplesContext(new vtkShrinkAndMaginifyStrategy);
					demo->DoAction();					
				} else if (item_name.CompareNoCase(L"Red_Book_Mathematics") == 0) {
					vtkExamplesContext* demo = new vtkExamplesContext(new vtkMathematicsStrategy);
					demo->DoAction();					
				} else if (item_name.CompareNoCase(L"Red_Book_MathLogic") == 0) {
					vtkExamplesContext* demo = new vtkExamplesContext(new vtkMathLogicStrategy);
					demo->DoAction();					
				} else if (item_name.CompareNoCase(L"End_01") == 0) {
					if (pVtkShowBtn) {
						pVtkShowBtn->SetText(_T("Please wait..."));
					}					
				}
				// 其它二级菜单
			}
		}
	}
}

LRESULT CVtkExampleDemoFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg, wParam, lParam);
}

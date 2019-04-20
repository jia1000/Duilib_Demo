#include "vtk_window.h"

#include "extended_control/tile_layout_extended.h"
#include "vtk/dui_vistasimple.h"

DuiVtkWindow::DuiVtkWindow(void)
	:m_layout_manager(NULL)
	, m_btn(NULL)
	, m_vtk(NULL)
	, m_win(NULL)
{
}


DuiVtkWindow::~DuiVtkWindow(void)
{
	if (m_layout_manager) {
		m_layout_manager->RemoveVtkWinodw(this);
	}
}

LPCTSTR DuiVtkWindow::GetWindowClassName() const 
{ 
	return _T("DuiVtkWindow"); 
}

void DuiVtkWindow::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this; //退出时，会有异常
}

CDuiString DuiVtkWindow::GetSkinFile()
{
	return _T("frame_wnd_demo\\vtk_window_container.xml");
}

CDuiString DuiVtkWindow::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");

}

CControlUI* DuiVtkWindow::CreateControl(LPCTSTR pstrClass)
{
	if ( _tcsicmp(pstrClass, _T("DuiVistaSimple")) == 0) {
		m_win = new CDuiVistaSimple;
		return m_win;
	} 
	return NULL;
}

void DuiVtkWindow::InitWindow()
{
	m_btn = static_cast<CButtonUI*>(m_pm.FindControl(L"btn_test"));
	m_vtk = static_cast<CButtonUI*>(m_pm.FindControl(L"vtkWindow"));
}

LRESULT DuiVtkWindow::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	__super::OnSize(uMsg, wParam, lParam, bHandled);
	return 0;
}


void    DuiVtkWindow::Notify(TNotifyUI& msg) 
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
			test_vtk_show();	
		}
	}
}

LRESULT DuiVtkWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg, wParam, lParam);
}

void DuiVtkWindow::SetLayoutManager(TileLayoutExtended* layout_manager)
{
	m_layout_manager = layout_manager;
}


void DuiVtkWindow::SetBtnText(CDuiString text)
{
	if (m_btn) {
		m_btn->SetText(text);
	}
}


void DuiVtkWindow::vtk_init()
{
	reader					= vtkSmartPointer<vtkJPEGReader>::New();
	shrinkFilter			= vtkSmartPointer<vtkImageShrink3D>::New();
	originalActor			= vtkSmartPointer<vtkImageActor>::New();
	shrinkActor				= vtkSmartPointer<vtkImageActor>::New();
	originalRenderer		= vtkSmartPointer<vtkRenderer>::New();
	shrinkRenderer			= vtkSmartPointer<vtkRenderer>::New();
	renderWindow			= vtkSmartPointer<vtkRenderWindow>::New();
	renderWindowInteractor	= vtkSmartPointer<vtkRenderWindowInteractor>::New();
	style					=	vtkSmartPointer<vtkInteractorStyleImage>::New();
}

void DuiVtkWindow::test_vtk_show()
{
	vtk_init();

	reader->SetFileName( "..\\Bin\\Skin\\image\\ct.jpg");
	reader->Update();

	shrinkFilter->SetInputConnection(reader->GetOutputPort());
	shrinkFilter->SetShrinkFactors(20,20,1);
	shrinkFilter->Update();

	int originalDims[3];
	reader->GetOutput()->GetDimensions(originalDims);

	double originalSpace[3];
	reader->GetOutput()->GetSpacing(originalSpace);

	int shrinkDims[3];
	shrinkFilter->GetOutput()->GetDimensions(shrinkDims);

	double shrinkSpace[3];
	shrinkFilter->GetOutput()->GetSpacing(shrinkSpace);

	originalActor->SetInput( reader->GetOutput());

	shrinkActor->SetInput(shrinkFilter->GetOutput());

	double originalViewport[4] = {0.0, 0.0, 0.5, 1.0};
	double shrinkViewport[4] = {0.5, 0.0, 1.0, 1.0};

	originalRenderer->SetViewport(originalViewport);
	originalRenderer->AddActor(originalActor);
	originalRenderer->ResetCamera();
	originalRenderer->SetBackground(1.0, 1.0, 1.0);

	shrinkRenderer->SetViewport(shrinkViewport);
	shrinkRenderer->AddActor(shrinkActor);
	shrinkRenderer->ResetCamera();
	shrinkRenderer->SetBackground(1.0, 1.0, 1.0);

	CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("vtkWindow")));
	if( pControl ) {
		renderWindow->SetParentId(reinterpret_cast<void *>(m_hWnd));
	}


	renderWindow->AddRenderer(originalRenderer);
	renderWindow->AddRenderer(shrinkRenderer);

	RECT rect;
	::GetClientRect(this->m_hWnd, &rect);
	renderWindow->SetSize(rect.right - rect.left, rect.bottom - rect.top);
	int title_height = 32;
	int button_test_height = 30;
	renderWindow->SetPosition(rect.left, rect.top + title_height + button_test_height);
	renderWindow->Render();
	//renderWindow->SetWindowName("ImageShrink3DExample");

	renderWindowInteractor->SetInteractorStyle(style);
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();
}
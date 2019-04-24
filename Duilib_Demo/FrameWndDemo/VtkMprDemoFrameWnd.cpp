#include "VtkMprDemoFrameWnd.h"

#include "FourViews\\vtkViewImage2D.h"
#include "FourViews\\vtkViewImage3D.h"

CVtkMprDemoFrameWnd::CVtkMprDemoFrameWnd(void)
{
}


CVtkMprDemoFrameWnd::~CVtkMprDemoFrameWnd(void)
{
}

LPCTSTR CVtkMprDemoFrameWnd::GetWindowClassName() const 
{ 
	return _T("VtkMprDemoFrameWnd"); 
}

void CVtkMprDemoFrameWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	//delete this; //退出时，会有异常
}

CDuiString CVtkMprDemoFrameWnd::GetSkinFile()
{
	return _T("frame_wnd_demo\\vtk_mpr_frame_demo.xml");
}

CDuiString CVtkMprDemoFrameWnd::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");

}

CControlUI* CVtkMprDemoFrameWnd::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CVtkMprDemoFrameWnd::InitWindow()
{
	
}

LRESULT CVtkMprDemoFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	__super::OnSize(uMsg, wParam, lParam, bHandled);
	return 0;
}


void    CVtkMprDemoFrameWnd::Notify(TNotifyUI& msg) 
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
		} else if (_tcscmp(pszCtrlName, _T("btn_test1")) == 0) {
			initViews();
		}
	}
}

LRESULT CVtkMprDemoFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg, wParam, lParam);
}

void CVtkMprDemoFrameWnd::initViews()
{
	m_AxialRenderder     = vtkSmartPointer< vtkRenderer >::New();
	m_SagittalRenderder  = vtkSmartPointer< vtkRenderer >::New();
	m_CoronalRenderder = vtkSmartPointer< vtkRenderer >::New();
	m_3DRenderder         = vtkSmartPointer< vtkRenderer >::New();

	m_AxialRenWin      = vtkSmartPointer< vtkRenderWindow >::New();
	m_SagittalRenWin   = vtkSmartPointer< vtkRenderWindow >::New();
	m_CoronalRenWin  = vtkSmartPointer< vtkRenderWindow >::New();
	m_3DRenWin          = vtkSmartPointer< vtkRenderWindow >::New();

	m_3DInteractor         = vtkSmartPointer< vtkRenderWindowInteractor >::New();
	m_AxialInteractor     = vtkSmartPointer< vtkRenderWindowInteractor >::New();
	m_SagittalInteractor  = vtkSmartPointer< vtkRenderWindowInteractor >::New();
	m_CoronalInteractor = vtkSmartPointer< vtkRenderWindowInteractor >::New();

	m_AxialRenWin->AddRenderer(m_AxialRenderder);
	m_SagittalRenWin->AddRenderer(m_SagittalRenderder);
	m_CoronalRenWin->AddRenderer(m_CoronalRenderder);
	m_3DRenWin->AddRenderer(m_3DRenderder);

	m_AxialInteractor->SetRenderWindow(m_AxialRenWin);
	m_SagittalInteractor->SetRenderWindow(m_SagittalRenWin);
	m_CoronalInteractor->SetRenderWindow(m_CoronalRenWin);
	m_3DInteractor->SetRenderWindow(m_3DRenWin);

	//m_MainWindow->m_AxialWidget->SetRenderWindow(m_AxialRenWin);
	//m_MainWindow->m_SagittalWidget->SetRenderWindow(m_SagittalRenWin);
	//m_MainWindow->m_CoronalWidget->SetRenderWindow(m_CoronalRenWin);
	//m_MainWindow->m_3DWidget->SetRenderWindow(m_3DRenWin);

	//Axial视图
	m_AxialImage = vtkSmartPointer< vtkViewImage2D >::New();
	m_AxialImage->SetRenderWindow( m_AxialRenWin ) ;
	m_AxialImage->SetRenderer( m_AxialRenderder ) ;
	m_AxialImage->SetLeftButtonInteractionStyle   (vtkViewImage2D::ZOOM_INTERACTION);
	m_AxialImage->SetMiddleButtonInteractionStyle (vtkViewImage2D::SELECT_INTERACTION);
	m_AxialImage->SetWheelInteractionStyle        (vtkViewImage2D::SELECT_INTERACTION);
	m_AxialImage->SetRightButtonInteractionStyle  (vtkViewImage2D::WINDOW_LEVEL_INTERACTION);
	m_AxialImage->SetBackgroundColor (0.0,0.0,0.0);
	m_AxialImage->SetAboutData("横断面");



	//////////////////////////////////////////////////////////////////////////
	m_AxialImage->SetOrientation (vtkViewImage2D::AXIAL_ID);
	m_AxialInteractor->Initialize();


	//////////////////////////////////////////////////////////////////////////
	// 用vtkMetaImageReader读取图像
	vtkSmartPointer<vtkMetaImageReader> reader = vtkSmartPointer<vtkMetaImageReader>::New();
	reader->SetFileName("..\\Bin\\Skin\\data\\slices1");
	reader->Update();

	m_AxialImage->SetImage(reader->GetOutput());

	m_AxialImage->SyncResetCurrentPoint();
	m_AxialImage->SyncResetWindowLevel();

	m_AxialImage->Render();
}
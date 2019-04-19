#include "VtkDemoFrameWnd.h"

#include "global_include.h"

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
	return NULL;
}

void CVtkDemoFrameWnd::InitWindow()
{
	
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
			test_vtk_show();			
		}

	}
}

LRESULT CVtkDemoFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg, wParam, lParam);
}

void CVtkDemoFrameWnd::test_vtk_show()
{
	vtkSmartPointer<vtkJPEGReader> reader =
		vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFileName( "..\\Bin\\Skin\\image\\ct.jpg");
	//reader->SetFileName( "F:\\dev_study\\duilib_study\\Duilib_Demo\\Bin\\Skin\\image\\ct.jpg");
	//reader->SetFileName( ".\\data\\ct.jpg");
	reader->Update();

	vtkSmartPointer<vtkImageShrink3D> shrinkFilter = 
		vtkSmartPointer<vtkImageShrink3D>::New();
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

	//std::cout<<"原图图像维数      ："<<originalDims[0] << " "<<originalDims[1]<<" "<<originalDims[2]<<std::endl;
	//std::cout<<"原图图像像素间隔  ："<<originalSpace[0] << " "<<originalSpace[1]<<" "<<originalSpace[2]<<std::endl;
	//std::cout<<"重采样图像维数    ："<<shrinkDims[0] << " "<<shrinkDims[1]<<" "<<shrinkDims[2]<<std::endl;
	//std::cout<<"重采样图像像素间隔："<<shrinkSpace[0] << " "<<shrinkSpace[1]<<" "<<shrinkSpace[2]<<std::endl;

	vtkSmartPointer<vtkImageActor> originalActor =
		vtkSmartPointer<vtkImageActor>::New();
	originalActor->SetInput( reader->GetOutput());

	vtkSmartPointer<vtkImageActor> shrinkActor =
		vtkSmartPointer<vtkImageActor>::New();
	shrinkActor->SetInput(shrinkFilter->GetOutput());

	double originalViewport[4] = {0.0, 0.0, 0.5, 1.0};
	double shrinkViewport[4] = {0.5, 0.0, 1.0, 1.0};

	vtkSmartPointer<vtkRenderer> originalRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	originalRenderer->SetViewport(originalViewport);
	originalRenderer->AddActor(originalActor);
	originalRenderer->ResetCamera();
	originalRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> shrinkRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	shrinkRenderer->SetViewport(shrinkViewport);
	shrinkRenderer->AddActor(shrinkActor);
	shrinkRenderer->ResetCamera();
	shrinkRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(originalRenderer);
	renderWindow->AddRenderer(shrinkRenderer);
	renderWindow->SetSize(640, 480);
	renderWindow->Render();
	renderWindow->SetWindowName("ImageShrink3DExample");

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style =
		vtkSmartPointer<vtkInteractorStyleImage>::New();

	renderWindowInteractor->SetInteractorStyle(style);
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();
}
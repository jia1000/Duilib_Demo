#include "stdafx.h"
#include "DicomSplitView.h"


CDicomSplitView::CDicomSplitView(HWND parent, RECT rc)
	: m_parentWnd(parent)
	, m_rc(rc)
{
    std::string folder = "..\\Bin\\Skin\\data\\slices1";

    m_reader = vtkSmartPointer<vtkDICOMImageReader>::New();
    m_reader->SetDirectoryName(folder.c_str());
    m_reader->Update();

    // 创建一个拆2*2布局的窗口
    m_render_left_down  = CreateDicomVtkRender();
    m_render_left_up    = CreateDicomVtkRender();
    m_render_right_down = CreateDicomVtkRender();//CreateDicomRibVtkRender();
    m_render_right_up   = CreateDicomVtkRender();

    // 左下图像
    m_render_left_down->SetBackground(0.3,0.3,0.3);
    m_render_left_down->SetViewport(0.0,0.0,0.5,0.5);
    // 右下图像
    m_render_right_down->SetBackground(0.4,0.4,0.4);
    m_render_right_down->SetViewport(0.5,0.0,1.0,0.5);
    // 左上图像
    m_render_left_up->SetBackground(0.5,0.5,0.5);
    m_render_left_up->SetViewport(0.0,0.5,0.5,1.0);
    // 右上图像
    m_render_right_up->SetBackground(0.6,0.6,0.6);
    m_render_right_up->SetViewport(0.5,0.5,1.0,1.0);


    /*vtkSmartPointer<vtkRenderWindow> */renWin = vtkSmartPointer<vtkRenderWindow>::New();
    renWin->AddRenderer(m_render_left_down );
    renWin->AddRenderer(m_render_left_up   );
    renWin->AddRenderer(m_render_right_down);
    renWin->AddRenderer(m_render_right_up  );
    //renWin->SetSize( 640, 480 );
	ResizeAndPosition(m_rc);
    renWin->Render();
    //renWin->SetWindowName("Viewport");

    vtkSmartPointer<vtkRenderWindowInteractor> interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(renWin);

    // 设置交互器的风格为Image，会比默认的3D风格，缺少旋转等功能.
    vtkSmartPointer<vtkInteractorStyleImage> interactorStyle = vtkSmartPointer<vtkInteractorStyleImage>::New();
    interactor->SetInteractorStyle(interactorStyle);


    renWin->Render();
    //interactor->Initialize();
    interactor->Start();
}


CDicomSplitView::~CDicomSplitView(void)
{
}

vtkSmartPointer<vtkRenderer> CDicomSplitView::CreateVtkRender()
{
    // 这里可以创建其它的几何图形    
    vtkSmartPointer<vtkConeSource> cone = vtkSmartPointer<vtkConeSource>::New();

    vtkSmartPointer<vtkPolyDataMapper> coneMapper = 	vtkSmartPointer<vtkPolyDataMapper>::New();
    coneMapper->SetInput(cone->GetOutput());

    vtkSmartPointer<vtkActor> coneActor = vtkSmartPointer<vtkActor>::New();
    coneActor->SetMapper(coneMapper);

    vtkSmartPointer<vtkRenderer> renderer1 = vtkSmartPointer<vtkRenderer>::New();
    renderer1->AddActor(coneActor);

    return renderer1;
}
vtkSmartPointer<vtkRenderer> CDicomSplitView::CreateDicomVtkRender()
{

    vtkSmartPointer<vtkImageMapToWindowLevelColors> WindowLevel = vtkSmartPointer<vtkImageMapToWindowLevelColors>::New();
    WindowLevel->SetInputConnection(m_reader->GetOutputPort());

    WindowLevel->SetWindow(380);
    WindowLevel->SetLevel(30);

    vtkSmartPointer<vtkImageActor> imageActor = vtkSmartPointer<vtkImageActor>::New();
    imageActor->SetInput(WindowLevel->GetOutput());

    vtkSmartPointer<vtkRenderer> renderer1 = vtkSmartPointer<vtkRenderer>::New();
    renderer1->AddActor(imageActor);

    return renderer1;
}
vtkSmartPointer<vtkRenderer> CDicomSplitView::CreateDicomRibVtkRender()
{
    vtkSmartPointer<vtkDICOMImageReader> v16=vtkSmartPointer<vtkDICOMImageReader>::New();
    v16->SetDataByteOrderToLittleEndian();
    v16->SetDirectoryName(".//data//slices1");// 文件目录path
    //v16->SetDataSpacing(3.2,3.2,1.5);
    v16->Update();


    vtkSmartPointer<vtkContourFilter> skinExtractor=vtkSmartPointer<vtkContourFilter>::New();
    skinExtractor->SetInputConnection(v16->GetOutputPort());
    skinExtractor->SetValue(0,500);

    vtkSmartPointer<vtkPolyDataNormals> skinNormals=vtkSmartPointer<vtkPolyDataNormals>::New();
    skinNormals->SetInputConnection(skinExtractor->GetOutputPort());
    skinNormals->SetFeatureAngle(60.0);

    vtkSmartPointer<vtkPolyDataMapper> coneMapper=vtkSmartPointer<vtkPolyDataMapper>::New();
    coneMapper->SetInputConnection(skinNormals->GetOutputPort());

    vtkSmartPointer<vtkActor> coneActor = vtkSmartPointer<vtkActor>::New();
    coneActor->SetMapper(coneMapper);

    vtkSmartPointer<vtkRenderer> renderer1 = vtkSmartPointer<vtkRenderer>::New();
    renderer1->AddActor(coneActor);

    return renderer1;
}

void CDicomSplitView::ResizeAndPosition(RECT rc)
{
	if (m_parentWnd) {
		renWin->SetParentId(m_parentWnd);
		int title_height = 0;//32;
		int button_test_height = 0;//30;
		RECT rect;
		::GetClientRect(m_parentWnd, &rect);
		rect = rc;
		renWin->SetSize(rect.right - rect.left, rect.bottom - rect.top - title_height - button_test_height);
		renWin->SetPosition(rect.left, rect.top + title_height + button_test_height);
	}
}
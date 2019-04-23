#include "stdafx.h"
#include "ImagePlaneWidgetsDemo.h"

CImagePlaneWidgetsDemo::CImagePlaneWidgetsDemo(HWND parent, RECT rc)
	: m_parentWnd(parent)
	, m_rc(rc)
{
}


CImagePlaneWidgetsDemo::~CImagePlaneWidgetsDemo(void)
{
}

void CImagePlaneWidgetsDemo::ShowWidgets_Test()
{
	// Create the RenderWindow, Renderer and both Actors
	m_renderer        = CreateRenderer();
	m_renderWindow    = CreateRenderWindow(m_renderer);
	m_interactor      = CreateInteractor(m_renderWindow);
	SetMyInteractorStyle(m_interactor);
	// Create a test pipeline
	CreatePipeline(m_renderer);
	// Create the widget
	AddMyActor(m_interactor);

	StartWidgetsRender(m_renderer, m_renderWindow, m_interactor);
}

vtkSmartPointer<vtkRenderer> CImagePlaneWidgetsDemo::CreateRenderer()
{
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

	renderer->SetBackground(0.1, 0.2, 0.4);

	return renderer;
}

vtkSmartPointer<vtkRenderWindow> CImagePlaneWidgetsDemo::CreateRenderWindow(vtkSmartPointer<vtkRenderer> renderer)
{
	vtkSmartPointer< vtkRenderWindow > renderWindow = vtkSmartPointer< vtkRenderWindow >::New();

	renderWindow->AddRenderer(renderer);
	renderWindow->SetWindowName("AnnotationWidget");
	//renderWindow->SetSize(400, 400);

	return renderWindow;
}
vtkSmartPointer<vtkRenderWindowInteractor> CImagePlaneWidgetsDemo::CreateInteractor(vtkSmartPointer<vtkRenderWindow> renderWindow)
{
	vtkSmartPointer< vtkRenderWindowInteractor > interactor = 
		vtkSmartPointer< vtkRenderWindowInteractor >::New();

	interactor->SetRenderWindow(renderWindow);

	return interactor;
}

void CImagePlaneWidgetsDemo::StartWidgetsRender(vtkSmartPointer<vtkRenderer> renderer, 
                                              vtkSmartPointer<vtkRenderWindow> renderWindow, 
                                              vtkSmartPointer<vtkRenderWindowInteractor> interactor)
{
	ResizeAndPosition(m_rc);
    m_interactor->Initialize();
    m_renderWindow->Render();
    planeWidget->On();
    m_interactor->Start();
}

void CImagePlaneWidgetsDemo::SetMyInteractorStyle(vtkSmartPointer<vtkRenderWindowInteractor> interactor)
{
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    interactor->SetInteractorStyle( style );
}

void CImagePlaneWidgetsDemo::CreatePipeline(vtkSmartPointer<vtkRenderer> renderer)
{
    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->Update();

    // Create a mapper and actor
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(sphereSource->GetOutputPort());
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    renderer->AddActor(actor);
}

void CImagePlaneWidgetsDemo::AddMyActor(vtkSmartPointer<vtkRenderWindowInteractor> interactor)
{
    /*vtkSmartPointer<vtkImagePlaneWidget>*/ planeWidget = vtkSmartPointer<vtkImagePlaneWidget>::New();
    planeWidget->SetInteractor(interactor);
    planeWidget->TextureVisibilityOff();

    double origin[3] = {1, 0,0};
    planeWidget->SetOrigin(origin);
    planeWidget->UpdatePlacement();

}

void CImagePlaneWidgetsDemo::ResizeAndPosition(RECT rc)
{
	if (m_parentWnd) {
		m_renderWindow->SetParentId(m_parentWnd);
		int title_height = 0;//32;
		int button_test_height = 0;//30;
		RECT rect;
		::GetClientRect(m_parentWnd, &rect);
		rect = rc;
		m_renderWindow->SetSize(rect.right - rect.left, rect.bottom - rect.top - title_height - button_test_height);
		m_renderWindow->SetPosition(rect.left, rect.top + title_height + button_test_height);
	}
}
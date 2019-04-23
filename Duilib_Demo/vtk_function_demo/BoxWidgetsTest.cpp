#include "stdafx.h"
#include "BoxWidgetsTest.h"


class vtkMyCallback : public vtkCommand
{
public:
    static vtkMyCallback *New()
    {
        return new vtkMyCallback;
    }
    virtual void Execute( vtkObject *caller, unsigned long, void* )
    {
        // Here we use the vtkBoxWidget to transform the underlying coneActor
        // (by manipulating its transformation matrix).
        vtkSmartPointer<vtkTransform> t =
            vtkSmartPointer<vtkTransform>::New();
        vtkBoxWidget *widget = reinterpret_cast<vtkBoxWidget*>(caller);
        widget->GetTransform( t );
        widget->GetProp3D()->SetUserTransform( t );
    }
};

//////////////////////////////////////////////////////////////////////////
CBoxWidgetsTest::CBoxWidgetsTest(HWND parent, RECT rc)
	: m_parentWnd(parent)
	, m_rc(rc)
{
}


CBoxWidgetsTest::~CBoxWidgetsTest(void)
{
}

void CBoxWidgetsTest::ShowWidgets_Test()
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

vtkSmartPointer<vtkRenderer> CBoxWidgetsTest::CreateRenderer()
{
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

	renderer->SetBackground(0.1, 0.2, 0.4);

	return renderer;
}

vtkSmartPointer<vtkRenderWindow> CBoxWidgetsTest::CreateRenderWindow(vtkSmartPointer<vtkRenderer> renderer)
{
	vtkSmartPointer< vtkRenderWindow > renderWindow = vtkSmartPointer< vtkRenderWindow >::New();

	renderWindow->AddRenderer(renderer);
	renderWindow->SetWindowName("AnnotationWidget");
	//renderWindow->SetSize(400, 400);
	
	return renderWindow;
}
vtkSmartPointer<vtkRenderWindowInteractor> CBoxWidgetsTest::CreateInteractor(vtkSmartPointer<vtkRenderWindow> renderWindow)
{
	vtkSmartPointer< vtkRenderWindowInteractor > interactor = 
		vtkSmartPointer< vtkRenderWindowInteractor >::New();

	interactor->SetRenderWindow(renderWindow);

	return interactor;
}

void CBoxWidgetsTest::StartWidgetsRender(vtkSmartPointer<vtkRenderer> renderer, 
                                              vtkSmartPointer<vtkRenderWindow> renderWindow, 
                                              vtkSmartPointer<vtkRenderWindowInteractor> interactor)
{
	ResizeAndPosition(m_rc);
    m_renderWindow->Render();
    boxWidget->On();
    m_interactor->Start();
}

void CBoxWidgetsTest::SetMyInteractorStyle(vtkSmartPointer<vtkRenderWindowInteractor> interactor)
{
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    interactor->SetInteractorStyle( style );
}

void CBoxWidgetsTest::CreatePipeline(vtkSmartPointer<vtkRenderer> renderer)
{
    vtkSmartPointer<vtkConeSource> cone = vtkSmartPointer<vtkConeSource>::New();

    vtkSmartPointer<vtkPolyDataMapper> coneMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    coneMapper->SetInputConnection( cone->GetOutputPort() );

    /*vtkSmartPointer<vtkActor>*/ actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper( coneMapper );

    renderer->AddActor(actor);
}

void CBoxWidgetsTest::AddMyActor(vtkSmartPointer<vtkRenderWindowInteractor> interactor)
{
    /*vtkSmartPointer<vtkBoxWidget> */boxWidget = vtkSmartPointer<vtkBoxWidget>::New();
    boxWidget->SetInteractor( interactor );

    boxWidget->SetProp3D( actor );
    boxWidget->SetPlaceFactor( 1.25 ); // Make the box 1.25x larger than the actor
    boxWidget->PlaceWidget();

    vtkSmartPointer<vtkMyCallback> callback = vtkSmartPointer<vtkMyCallback>::New();
    boxWidget->AddObserver( vtkCommand::InteractionEvent, callback );

    //boxWidget->On();
}

void CBoxWidgetsTest::ResizeAndPosition(RECT rc)
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
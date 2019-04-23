#include "stdafx.h"
#include "CaptionWidgetsDemo.h"


CCaptionWidgetsDemo::CCaptionWidgetsDemo(HWND parent, RECT rc)
	: CWidgetsDemoBase(parent, rc)
{
}


CCaptionWidgetsDemo::~CCaptionWidgetsDemo(void)
{
}


void CCaptionWidgetsDemo::CaptionWidgets_Test()
{
    // Create the RenderWindow, Renderer and both Actors
    m_renderer        = CreateRenderer();
    m_renderWindow    = CreateRenderWindow(m_renderer);
    m_interactor      = CreateInteractor(m_renderWindow);
    // Create a test pipeline
    CreateCaptionPipeline(m_renderer);
    // Create the widget
    AddCaptionActor(m_interactor);

    StartRenderCaption(m_renderer, m_renderWindow, m_interactor);

}


void CCaptionWidgetsDemo::StartRenderCaption(vtkSmartPointer<vtkRenderer> renderer, 
                                   vtkSmartPointer<vtkRenderWindow> renderWindow, 
                                   vtkSmartPointer<vtkRenderWindowInteractor> interactor)
{
    ResizeAndPosition(m_rc);
    m_renderWindow->Render();
    captionWidget->On();
    m_interactor->Start();
}


void CCaptionWidgetsDemo::CreateCaptionPipeline(vtkSmartPointer<vtkRenderer> renderer)
{
    // Sphere
    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    //sphereSource->SetRadius(10);
    sphereSource->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(sphereSource->GetOutputPort());

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    renderer->AddActor(actor);
}


void CCaptionWidgetsDemo::AddCaptionActor(vtkSmartPointer<vtkRenderWindowInteractor> interactor)
{
    // Create the widget and its representation
    vtkSmartPointer<vtkCaptionRepresentation> captionRepresentation = vtkSmartPointer<vtkCaptionRepresentation>::New();
    captionRepresentation->GetCaptionActor2D()->SetCaption("Caption Widgets test");
    captionRepresentation->GetCaptionActor2D()->GetTextActor()->GetTextProperty()->SetFontSize(100);

    double pos[3] = {.2,0,0};
    captionRepresentation->SetAnchorPosition(pos);

    // vtkCaptionWidget变量出了作用域，会失效，所以，在vtkRenderWindowInteractor的start同级作用域，一定要保证存在
    captionWidget = vtkSmartPointer<vtkCaptionWidget>::New();
    captionWidget->SetInteractor(interactor);
    captionWidget->SetRepresentation(captionRepresentation);

    // 需要放在Render之后
    //captionWidget->On();
}

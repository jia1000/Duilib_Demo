#include "stdafx.h"
#include "TextWidgetsDemo.h"


CTextWidgetsDemo::CTextWidgetsDemo(HWND parent, RECT rc)
	: CWidgetsDemoBase(parent, rc)
{
}


CTextWidgetsDemo::~CTextWidgetsDemo(void)
{
}

void CTextWidgetsDemo::ShowWidgets_Test()
{
    // Create the RenderWindow, Renderer and both Actors
    m_renderer        = CreateRenderer();
    m_renderWindow    = CreateRenderWindow(m_renderer);
    m_interactor      = CreateInteractor(m_renderWindow);
    // Create a test pipeline
    CreatePipeline(m_renderer);
    // Create the widget
    AddTextActor(m_interactor);
        
    StartRender(m_renderer, m_renderWindow, m_interactor);
}


void CTextWidgetsDemo::StartRender(vtkSmartPointer<vtkRenderer> renderer, 
                                   vtkSmartPointer<vtkRenderWindow> renderWindow, 
                                   vtkSmartPointer<vtkRenderWindowInteractor> interactor)
{
    //renderWindow->SetSize(600, 600);
	ResizeAndPosition(m_rc);
    interactor->Initialize();
    renderWindow->Render();
    interactor->Start();
}

void CTextWidgetsDemo::CreatePipeline(vtkSmartPointer<vtkRenderer> renderer)
{
    vtkSmartPointer<vtkJPEGReader> reader = vtkSmartPointer<vtkJPEGReader>::New();
    reader->SetFileName("..\\Bin\\Skin\\image\\ct.jpg");
    reader->Update();

    vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
    actor->SetInput(reader->GetOutput());

    renderer->AddActor(actor);
}

void CTextWidgetsDemo::AddTextActor(vtkSmartPointer<vtkRenderWindowInteractor> interactor)
{
    // Create the widget
    vtkSmartPointer<vtkTextActor> textActor = vtkSmartPointer<vtkTextActor>::New();
    textActor->SetInput("This is a Text Actor .");
    textActor->GetTextProperty()->SetColor( 0.1, 0.1, 0.1 );
    int font_size = textActor->GetTextProperty()->GetFontSize();
    // 没起效
    //textActor->GetTextProperty()->SetFontSize()

    // vtkTextWidget变量出了作用域，会失效，所以，在vtkRenderWindowInteractor的start同级作用域，一定要保证存在
    textWidget = vtkSmartPointer<vtkTextWidget>::New();

    vtkSmartPointer<vtkTextRepresentation> textRepresentation = vtkSmartPointer<vtkTextRepresentation>::New();
    textRepresentation->GetPositionCoordinate()->SetValue( .01, .01 );
    textRepresentation->GetPosition2Coordinate()->SetValue( .7, .2 );

    textWidget ->SetRepresentation( textRepresentation );

    textWidget->SetInteractor(interactor);
    textWidget->SetTextActor(textActor);
    textWidget->SelectableOff();

    textWidget->On();
}

#include "stdafx.h"
#include "ContourWidgetsTest.h"


CContourWidgetsTest::CContourWidgetsTest(HWND parent, RECT rc)
	: m_parentWnd(parent)
	, m_rc(rc)
{
}


CContourWidgetsTest::~CContourWidgetsTest(void)
{
}

void CContourWidgetsTest::ShowWidgets_Test()
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

vtkSmartPointer<vtkRenderer> CContourWidgetsTest::CreateRenderer()
{
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

	renderer->SetBackground(0.1, 0.2, 0.4);

	return renderer;
}

vtkSmartPointer<vtkRenderWindow> CContourWidgetsTest::CreateRenderWindow(vtkSmartPointer<vtkRenderer> renderer)
{
	vtkSmartPointer< vtkRenderWindow > renderWindow = vtkSmartPointer< vtkRenderWindow >::New();

	renderWindow->AddRenderer(renderer);
	renderWindow->SetWindowName("AnnotationWidget");
	//renderWindow->SetSize(400, 400);

	return renderWindow;
}
vtkSmartPointer<vtkRenderWindowInteractor> CContourWidgetsTest::CreateInteractor(vtkSmartPointer<vtkRenderWindow> renderWindow)
{
	vtkSmartPointer< vtkRenderWindowInteractor > interactor = 
		vtkSmartPointer< vtkRenderWindowInteractor >::New();

	interactor->SetRenderWindow(renderWindow);

	return interactor;
}

void CContourWidgetsTest::StartWidgetsRender(vtkSmartPointer<vtkRenderer> renderer, 
                                              vtkSmartPointer<vtkRenderWindow> renderWindow, 
                                              vtkSmartPointer<vtkRenderWindowInteractor> interactor)
{
	ResizeAndPosition(m_rc);
    m_renderWindow->Render();
    //该widget的On和Initialize函数，需要放置于此， 且执行顺序固定
    contourWidget->On(); // Turn on the interactor observer
    contourWidget->Initialize(polydata);
    m_interactor->Start();
}

void CContourWidgetsTest::SetMyInteractorStyle(vtkSmartPointer<vtkRenderWindowInteractor> interactor)
{
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    interactor->SetInteractorStyle( style );
}

void CContourWidgetsTest::CreatePipeline(vtkSmartPointer<vtkRenderer> renderer)
{
    // Generate a set of points arranged in a circle
    int numPts = 10;
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    for (int i = 0; i < numPts; i++)
    {
        // Create numPts points evenly spread around a circumference of radius 0.1
        const double angle = 2.0*vtkMath::Pi()*i/numPts;
        points->InsertPoint(static_cast<vtkIdType>(i), 0.1*cos(angle), 0.1*sin(angle), 0.0 );
    }

    // Create a cell array to connect the points into meaningful geometry
    vtkIdType* vertexIndices = new vtkIdType[numPts+1];
    for (int i = 0; i < numPts; i++) 
    { 
        vertexIndices[i] = static_cast<vtkIdType>(i); 
    }
    // Set the last vertex to 0; this means the last line segment will join the 19th point (vertices[19])
    // with the first one (vertices[0]), thus closing the circle.
    vertexIndices[numPts] = 0;
    vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
    lines->InsertNextCell(numPts+1, vertexIndices);

    // Create polydata to hold the geometry just created, and populate it
    /*vtkSmartPointer<vtkPolyData> */polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata->SetPoints(points);
    polydata->SetLines(lines);
}

void CContourWidgetsTest::AddMyActor(vtkSmartPointer<vtkRenderWindowInteractor> interactor)
{
    // Create the contour widget
    /*vtkSmartPointer<vtkContourWidget> */contourWidget =
        vtkSmartPointer<vtkContourWidget>::New();

    // Override the default representation for the contour widget to customize its look
    vtkSmartPointer<vtkOrientedGlyphContourRepresentation> contourRepresentation =
        vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New();
    contourRepresentation->GetLinesProperty()->SetColor(1, 0, 0); // Set color to red
    contourWidget->SetRepresentation(contourRepresentation);

    // Set up the contour widget within the visualization pipeline just assembled
    contourWidget->SetInteractor(interactor);
    //contourWidget->On(); // Turn on the interactor observer
    //contourWidget->Initialize(polydata);

    // 需要放在Render之后
    //captionWidget->On();
}

void CContourWidgetsTest::ResizeAndPosition(RECT rc)
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
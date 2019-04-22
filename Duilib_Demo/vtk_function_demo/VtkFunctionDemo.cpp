#include "stdafx.h"

#include "VtkFunctionDemo.h"

CVtkFunctionDemo::CVtkFunctionDemo(HWND parent, RECT rc)
	: m_parentWnd(parent)
	, m_rc(rc)
{
   
}


CVtkFunctionDemo::~CVtkFunctionDemo(void)
{
}

void CVtkFunctionDemo::ResizeAndPosition(RECT rc)
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

void CVtkFunctionDemo::Function1()
{
	vtkSmartPointer< vtkJPEGReader > reader = 
		vtkSmartPointer< vtkJPEGReader >::New();
	reader->SetFileName("..\\Bin\\Skin\\image\\texture.jpg");

	vtkSmartPointer< vtkTexture > texture = 
		vtkSmartPointer< vtkTexture >::New();
	texture->SetInputConnection( reader->GetOutputPort() );
	texture->InterpolateOn();


	vtkSmartPointer<vtkCylinderSource> cylinder = vtkSmartPointer<vtkCylinderSource>::New();
	cylinder->SetHeight( 3.0 );
	cylinder->SetRadius( 1.0 );
	cylinder->SetResolution( 10 ); 

	vtkSmartPointer<vtkPolyDataMapper> cylinderMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	cylinderMapper->SetInputConnection( cylinder->GetOutputPort() ); 

	vtkSmartPointer<vtkActor> cylinderActor = vtkSmartPointer<vtkActor>::New();
	cylinderActor->SetMapper( cylinderMapper );
	cylinderActor->GetProperty()->SetColor(1.0, 1.0, 1.0);
	cylinderActor->SetTexture(texture);

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor( cylinderActor );
	renderer->SetBackground( 1.0, 1.0, 1.0 );

	renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer( renderer );

	//renWin->SetSize( 640, 480 );
	ResizeAndPosition(m_rc);

	renWin->Render();
	renWin->SetWindowName("RenderCylinder");

	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);

	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	iren->SetInteractorStyle(style);

	iren->Initialize();
	iren->Start();
}
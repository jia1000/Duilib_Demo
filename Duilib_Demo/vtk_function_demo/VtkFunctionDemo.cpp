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

void CVtkFunctionDemo::InitVtk()
{
	/*vtkSmartPointer<vtkJPEGReader>*/ reader = vtkSmartPointer<vtkJPEGReader>::New();
	reader->SetFileName("..\\Bin\\Skin\\image\\ct.jpg");
	reader->Update();

	/*vtkSmartPointer<vtkImageActor>*/ imgActor = vtkSmartPointer<vtkImageActor>::New();
	imgActor->SetInput(reader->GetOutput());

	/*vtkSmartPointer<vtkRenderer>*/ render = vtkSmartPointer<vtkRenderer>::New();
	render->AddActor(imgActor);
	render->SetBackground(0, 0, 0);
	render->ResetCamera();

	renWin = vtkSmartPointer<vtkRenderWindow>::New();
	renWin->AddRenderer(render);
	renWin->SetWindowName("MeasurementDistanceApp");
	//rw->SetSize(320, 320);
	ResizeAndPosition(m_rc);

	/*vtkSmartPointer<vtkRenderWindowInteractor>*/ rwi = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	rwi->SetRenderWindow(renWin);

	/*vtkSmartPointer<vtkInteractorStyleImage>*/ style = vtkSmartPointer<vtkInteractorStyleImage>::New();
	rwi->SetInteractorStyle(style);
	
	renWin->Render();
	rwi->Initialize();
	// 关键点：此处使用Start，则将消息循环，交给了VTK
	//rwi->Start();
	
}

void CVtkFunctionDemo::Function2(int WidgetType)
{	
	//vtkDistanceWidget
	if (WidgetType == 0)
	{
		//实例化Widget
		/*vtkSmartPointer<vtkDistanceWidget>*/ distanceWidget = vtkSmartPointer<vtkDistanceWidget>::New();
		//指定渲染窗口交互器,来监听用户事件
		distanceWidget->SetInteractor(rwi);
		//必要时使用观察者/命令模式创建回调函数(此处没用)
		//创建几何表达实体。用SetRepresentation()把事件与Widget关联起来
		//或者使用Widget默认的几何表达实体
		distanceWidget->CreateDefaultRepresentation();
		static_cast<vtkDistanceRepresentation*> (distanceWidget->GetRepresentation())
			->SetLabelFormat("%-#6.3g px");
		//激活Widget
		distanceWidget->On();
	}
	//vtkAngleWidget
	if (WidgetType == 1)
	{
		/*vtkSmartPointer<vtkAngleWidget>*/ angleWiget = vtkSmartPointer<vtkAngleWidget>::New();
		angleWiget->SetInteractor(rwi);
		//创建个性化的实体图标
		vtkSmartPointer<vtkAngleRepresentation2D> angleRep =
			vtkSmartPointer<vtkAngleRepresentation2D>::New();
		angleRep->GetRay1()->GetProperty()->SetColor(0, 1, 0);
		angleRep->GetRay1()->GetProperty()->SetLineWidth(3);
		angleRep->GetRay2()->GetProperty()->SetColor(0, 1, 0);
		angleRep->GetRay1()->GetProperty()->SetLineWidth(3);
		angleRep->GetArc()->GetProperty()->SetColor(0, 1, 0);
		angleRep->GetArc()->GetProperty()->SetLineWidth(3);
		angleWiget->SetRepresentation(angleRep);
		angleWiget->On();
	}
	//vtkBiDimensionalWidget
	if (WidgetType == 2)
	{
		/*vtkSmartPointer<vtkBiDimensionalWidget>*/ bidimensionalWidget =
			vtkSmartPointer<vtkBiDimensionalWidget>::New();
		bidimensionalWidget->SetInteractor(rwi);
		//采用默认的图标
		bidimensionalWidget->CreateDefaultRepresentation();
		bidimensionalWidget->On();
	}
}

void CVtkFunctionDemo::Function_for_cta()
{
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	points->InsertNextPoint(0.0, 0.0, 0.0);
	points->InsertNextPoint(1.0, 1.0, 0.0);
	points->InsertNextPoint(1.0, 2.0, 0.0);
	points->InsertNextPoint(2.0, 3.0, 0.0);
	points->InsertNextPoint(8.0, 3.0, 0.0);

	vtkSmartPointer<vtkPolyLine> polyLine =
		vtkSmartPointer<vtkPolyLine>::New();
	polyLine->GetPointIds()->SetNumberOfIds(points->GetNumberOfPoints());
	for(unsigned int i = 0; i < points->GetNumberOfPoints(); i++)
	{
		polyLine->GetPointIds()->SetId(i,i);
	}

	vtkSmartPointer<vtkCellArray> cellsArray = vtkSmartPointer<vtkCellArray>::New();
	cellsArray->InsertNextCell(polyLine);
		
	vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
	polyData->SetPoints(points);
	polyData->SetLines(cellsArray);
	polyData->Update();

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInput(polyData);

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetLineWidth(2);
	double color[3] = {1.0, 0.0, 0.0};
	actor->GetProperty()->SetColor(color);

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(actor);
	
	vtkSmartPointer<vtkRenderWindow> renderwindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderwindow->AddRenderer(renderer);

	vtkSmartPointer<vtkRenderWindowInteractor> renderwindowinteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderwindowinteractor->SetRenderWindow(renderwindow);

	renderwindow->Render();
	renderwindowinteractor->Start();
}
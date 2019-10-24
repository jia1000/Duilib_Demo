#include "stdafx.h"
#include "red_book_volume_of_Interest_demo.h"

//////////////////////////////////////////////////////////////////////////
Red_Book_VolumeOfInterest_Demo::Red_Book_VolumeOfInterest_Demo(HWND parent, CButtonUI* vtkShowBtn)
	: vtkExampleBase(parent, vtkShowBtn)
{
}


Red_Book_VolumeOfInterest_Demo::~Red_Book_VolumeOfInterest_Demo(void)
{
}

void Red_Book_VolumeOfInterest_Demo::InitVtkExampleDemo()
{
	m_renderWindow =	vtkSmartPointer<vtkRenderWindow>::New();
	m_interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();

	ResizeAndPosition();	
}

void Red_Book_VolumeOfInterest_Demo::ShowVtkExampleDemo()
{    
	vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetFileName ("..\\Bin\\Skin\\data\\slices1\\2");
	reader->Update();

	int dims[3];
	reader->GetOutput()->GetDimensions(dims);

	// 画布区域
	vtkSmartPointer<vtkImageCanvasSource2D> imageSource = 
		vtkSmartPointer<vtkImageCanvasSource2D>::New();
	imageSource->SetNumberOfScalarComponents(1);
	imageSource->SetScalarTypeToUnsignedChar();
	imageSource->SetExtent(0, dims[0], 0, dims[1], 0, dims[2]);
	imageSource->SetDrawColor(0.0);
	imageSource->FillBox(0, dims[0], 0, dims[1]);
	imageSource->SetDrawColor(255.0);
	imageSource->FillBox(dims[0]/4.,3.*dims[0]/4.,dims[1]/4.,3.*dims[1]/4.);
	imageSource->Update();

	// 这个区域没看到效果..............
	// blend区域
	vtkSmartPointer<vtkImageBlend> imageBlend = vtkSmartPointer<vtkImageBlend>::New();
	imageBlend->SetInput(0, reader->GetOutput());
	imageBlend->SetInput(1, imageSource->GetOutput());
	imageBlend->SetOpacity(0, 0);
	imageBlend->SetOpacity(1, 1);
	imageBlend->Update();

	// 感兴趣区域
	vtkSmartPointer<vtkExtractVOI> extractVOI =	vtkSmartPointer<vtkExtractVOI>::New();
	extractVOI->SetInputConnection(reader->GetOutputPort());
	extractVOI->SetVOI(dims[0]/4.,3.*dims[0]/4.,dims[1]/4.,3.*dims[1]/4., 0, 0);
	extractVOI->Update();

	

	//-------------------------------actor-----------------------------
	// 原始的actor
	vtkSmartPointer<vtkImageActor> originalActor = vtkSmartPointer<vtkImageActor>::New();
	originalActor->SetInput(reader->GetOutput());

	// canvas的actor
	vtkSmartPointer<vtkImageActor> canvasActor = vtkSmartPointer<vtkImageActor>::New();
	canvasActor->SetInput(imageSource->GetOutput());

	// blend的actor
	vtkSmartPointer<vtkImageActor> blendActor =	vtkSmartPointer<vtkImageActor>::New();
	blendActor->SetInput(imageBlend->GetOutput());

	// 感兴趣的actor
	vtkSmartPointer<vtkImageActor> voiActor = vtkSmartPointer<vtkImageActor>::New();
	voiActor->SetInput(extractVOI->GetOutput());
	
	//--------------------------------renderer--------------------------------
	double originalViewport[4] = {0.0, 0.0, 0.25, 1.0};
	double canvasViewport[4] = {0.25, 0.0, 0.5, 1.0};
	double blendViewport[4] = {0.5, 0.0, 0.75, 1.0};
	double voiviewport[4] = {0.75, 0.0, 1.0, 1.0};

	// 原始的Renderer
	vtkSmartPointer<vtkRenderer> originalRenderer =	vtkSmartPointer<vtkRenderer>::New();
	originalRenderer->SetViewport(originalViewport);
	originalRenderer->AddActor(originalActor);
	originalRenderer->ResetCamera();
	originalRenderer->SetBackground(1.0, 1.0, 1.0);

	// canvas的Renderer
	vtkSmartPointer<vtkRenderer> canvasRenderer = vtkSmartPointer<vtkRenderer>::New();
	canvasRenderer->SetViewport(canvasViewport);
	canvasRenderer->AddActor(canvasActor);
	canvasRenderer->ResetCamera();
	canvasRenderer->SetBackground(1.0, 1.0, 1.0);

	// blend的Renderer
	vtkSmartPointer<vtkRenderer> blendRenderer = vtkSmartPointer<vtkRenderer>::New();
	blendRenderer->SetViewport(blendViewport);
	blendRenderer->AddActor(blendActor);
	blendRenderer->ResetCamera();
	blendRenderer->SetBackground(1.0, 1.0, 1.0);

	// 感兴趣的Renderer
	vtkSmartPointer<vtkRenderer> shiftscaleRenderer = vtkSmartPointer<vtkRenderer>::New();
	shiftscaleRenderer->SetViewport(voiviewport);
	shiftscaleRenderer->AddActor(voiActor);
	shiftscaleRenderer->ResetCamera();
	shiftscaleRenderer->SetBackground(1.0, 1.0, 1.0);

	//-------------------------renderwindos-------------------------------
	// renderWindow窗口
	m_renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	m_renderWindow->AddRenderer(originalRenderer);
	m_renderWindow->AddRenderer(canvasRenderer);
	m_renderWindow->AddRenderer(blendRenderer);
	m_renderWindow->AddRenderer(shiftscaleRenderer);
	m_renderWindow->SetSize(900, 300);
	m_renderWindow->Render();
	m_renderWindow->SetWindowName("ExtractVOIExample");

	// interactor交互形式	
	vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
	m_interactor->SetInteractorStyle(style);
	m_interactor->SetRenderWindow(m_renderWindow);
	m_interactor->Initialize();
	m_interactor->Start();

}


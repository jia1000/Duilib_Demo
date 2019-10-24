#include "stdafx.h"
#include "Reslice_Demo.h"

//////////////////////////////////////////////////////////////////////////
Reslice_Demo::Reslice_Demo(HWND parent, CButtonUI* vtkShowBtn)
	: vtkExampleBase(parent, vtkShowBtn)
{
}


Reslice_Demo::~Reslice_Demo(void)
{
}

void Reslice_Demo::InitVtkExampleDemo()
{
	m_renderWindow =	vtkSmartPointer<vtkRenderWindow>::New();
	m_interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();

	ResizeAndPosition();	
}

void Reslice_Demo::ShowVtkExampleDemo()
{    
	vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetDirectoryName("..\\Bin\\Skin\\data\\slices1\\");
	reader->Update();

	int extent[6];
	double spacing[3];
	double origin[3];

	//reader->GetOutput()->GetDimensions(extent);
	//reader->GetOutput()->GetSpacing(spacing);
	//reader->GetOutput()->GetOrigin(origin);

	reader->GetOutput()->GetExtent(extent);
	reader->GetOutput()->GetSpacing(spacing);
	reader->GetOutput()->GetOrigin(origin);

	double center[3];
	center[0] = origin[0] + spacing[0] * 0.5 * (extent[0] + extent[1]);
	center[1] = origin[1] + spacing[1] * 0.5 * (extent[2] + extent[3]);
	center[2] = origin[2] + spacing[2] * 0.5 * (extent[4] + extent[5]);

	static double axialElements[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 
	};

	vtkSmartPointer<vtkMatrix4x4> resliceAxes =	vtkSmartPointer<vtkMatrix4x4>::New();
	resliceAxes->DeepCopy(axialElements);
	resliceAxes->SetElement(0, 3, center[0]);
	resliceAxes->SetElement(1, 3, center[1]);
	resliceAxes->SetElement(2, 3, center[2]);

	vtkSmartPointer<vtkImageReslice> reslice = vtkSmartPointer<vtkImageReslice>::New();
	reslice->SetInputConnection(reader->GetOutputPort());
	reslice->SetOutputDimensionality(2);
	reslice->SetResliceAxes(resliceAxes);
	reslice->SetInterpolationModeToLinear();

	vtkSmartPointer<vtkLookupTable> colorTable = vtkSmartPointer<vtkLookupTable>::New();
	colorTable->SetRange(0, 1000); 
	colorTable->SetValueRange(0.0, 1.0);
	colorTable->SetSaturationRange(0.0, 0.0);
	colorTable->SetRampToLinear();
	colorTable->Build();

	vtkSmartPointer<vtkImageMapToColors> colorMap =	vtkSmartPointer<vtkImageMapToColors>::New();
	colorMap->SetLookupTable(colorTable);
	colorMap->SetInputConnection(reslice->GetOutputPort());

	vtkSmartPointer<vtkImageActor> imgActor = vtkSmartPointer<vtkImageActor>::New();
	imgActor->SetInput(colorMap->GetOutput());

	vtkSmartPointer<vtkRenderer> renderer =	vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(imgActor);
	renderer->SetBackground(1.0, 1.0, 1.0);

	m_renderWindow->AddRenderer(renderer);
	m_renderWindow->Render();
	m_renderWindow->SetWindowName("ImageResliceExample");

	vtkSmartPointer<vtkInteractorStyleImage> imagestyle = vtkSmartPointer<vtkInteractorStyleImage>::New();

	m_interactor->SetInteractorStyle(imagestyle);
	m_interactor->SetRenderWindow(m_renderWindow);
	m_interactor->Initialize();
	m_interactor->Start();

}


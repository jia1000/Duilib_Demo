#include "stdafx.h"
#include "StructuredGrid_BlankPoint_Demo.h"

//////////////////////////////////////////////////////////////////////////
StructuredGrid_BlankPoint_Demo::StructuredGrid_BlankPoint_Demo(HWND parent, CButtonUI* vtkShowBtn)
	: vtkExampleBase(parent, vtkShowBtn)
{
}


StructuredGrid_BlankPoint_Demo::~StructuredGrid_BlankPoint_Demo(void)
{
}

void StructuredGrid_BlankPoint_Demo::InitVtkExampleDemo()
{
	m_renderWindow =	vtkSmartPointer<vtkRenderWindow>::New();

	ResizeAndPosition();	
}

void StructuredGrid_BlankPoint_Demo::ShowVtkExampleDemo()
{    
	// Create a grid
	vtkSmartPointer<vtkStructuredGrid> structuredGrid =
		vtkSmartPointer<vtkStructuredGrid>::New();

	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();

	unsigned int gridSize = 8;
	unsigned int counter = 0;
	// Create a 5x5 grid of points
	for(unsigned int j = 0; j < gridSize; j++)
	{
		for(unsigned int i = 0; i < gridSize; i++)
		{
			if(i == 3 && j == 3) // Make one point higher than the rest
			{
				points->InsertNextPoint(i, j, 2);
				std::cout << "The different point is number " << counter << std::endl;
			}
			else
			{
				points->InsertNextPoint(i, j, 0); // Make most of the points the same height
			}
			counter++;
		}
	}

	// Specify the dimensions of the grid
	structuredGrid->SetDimensions(gridSize,gridSize,1);

	structuredGrid->SetPoints(points);

	structuredGrid->BlankPoint(27);
	structuredGrid->Modified();

	// Create a mapper and actor
	vtkSmartPointer<vtkDataSetMapper> gridMapper =
		vtkSmartPointer<vtkDataSetMapper>::New();
	//gridMapper->SetInputData(structuredGrid);
	gridMapper->SetInput(structuredGrid);


	vtkSmartPointer<vtkActor> gridActor =
		vtkSmartPointer<vtkActor>::New();
	gridActor->SetMapper(gridMapper);
	gridActor->GetProperty()->EdgeVisibilityOn();
	gridActor->GetProperty()->SetEdgeColor(0,0,1);

	// Create a renderer, render window, and interactor
	vtkSmartPointer<vtkRenderer> renderer =
		vtkSmartPointer<vtkRenderer>::New();
	
	m_renderWindow->AddRenderer(renderer);
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(m_renderWindow);

	// Add the actor to the scene
	renderer->AddActor(gridActor);
	renderer->SetBackground(.3, .6, .3); // Background color green

	// Render and interact
	m_renderWindow->Render();
	renderWindowInteractor->Start();

}
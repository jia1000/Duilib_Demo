#pragma once
#include "global_include.h"


class CVtkFunctionDemo
{
public:
    CVtkFunctionDemo(HWND parent, RECT rc);
    ~CVtkFunctionDemo(void);

	void ResizeAndPosition(RECT rc);
	void Function1();
	void Function2(int WidgetType);

	void Function_for_cta();
	void InitVtk();

private:
	vtkSmartPointer<vtkRenderer> render ;
	vtkSmartPointer<vtkRenderWindow> renWin;
	vtkSmartPointer<vtkRenderWindowInteractor> rwi;

	vtkSmartPointer<vtkJPEGReader> reader;
	vtkSmartPointer<vtkImageActor> imgActor;
	vtkSmartPointer<vtkInteractorStyleImage> style;

	vtkSmartPointer<vtkDistanceWidget> distanceWidget;
	vtkSmartPointer<vtkAngleWidget> angleWiget;
	vtkSmartPointer<vtkBiDimensionalWidget> bidimensionalWidget;

	HWND m_parentWnd;	
	RECT m_rc;
};


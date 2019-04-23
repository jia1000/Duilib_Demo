#pragma once
#include "WidgetsDemoBase.h"

#include "global_include.h"

class CContourWidgetsDemo : public CWidgetsDemoBase
{
public:
    CContourWidgetsDemo(HWND parent, RECT rc);
    ~CContourWidgetsDemo(void);

	virtual void SetMyInteractorStyle(vtkSmartPointer<vtkRenderWindowInteractor> interactor);

    virtual void StartWidgetsRender(vtkSmartPointer<vtkRenderer> renderer, 
        vtkSmartPointer<vtkRenderWindow> renderWindow, 
        vtkSmartPointer<vtkRenderWindowInteractor> interactor);
    virtual void CreatePipeline(vtkSmartPointer<vtkRenderer> renderer);
    virtual void AddMyActor(vtkSmartPointer<vtkRenderWindowInteractor> interactor);

private:
	vtkSmartPointer<vtkContourWidget> contourWidget;
	vtkSmartPointer<vtkPolyData> polydata;
};


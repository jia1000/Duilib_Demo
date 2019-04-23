#pragma once
#include "WidgetsDemoBase.h"

#include "global_include.h"

class CImagePlaneWidgetsDemo : public CWidgetsDemoBase
{
public:
    CImagePlaneWidgetsDemo(HWND parent, RECT rc);
    ~CImagePlaneWidgetsDemo(void);	

    virtual void SetMyInteractorStyle(vtkSmartPointer<vtkRenderWindowInteractor> interactor);

    virtual void StartWidgetsRender(vtkSmartPointer<vtkRenderer> renderer, 
        vtkSmartPointer<vtkRenderWindow> renderWindow, 
        vtkSmartPointer<vtkRenderWindowInteractor> interactor);
    virtual void CreatePipeline(vtkSmartPointer<vtkRenderer> renderer);
    virtual void AddMyActor(vtkSmartPointer<vtkRenderWindowInteractor> interactor);
		
private:	
    vtkSmartPointer<vtkImagePlaneWidget> planeWidget;

};


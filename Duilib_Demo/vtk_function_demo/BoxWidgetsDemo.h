#pragma once
#include "WidgetsDemoBase.h"

#include "global_include.h"

class CBoxWidgetsDemo  : public CWidgetsDemoBase
{
public:
    CBoxWidgetsDemo(HWND parent, RECT rc);
    ~CBoxWidgetsDemo(void);

	virtual void SetMyInteractorStyle(vtkSmartPointer<vtkRenderWindowInteractor> interactor);

    virtual void StartWidgetsRender(vtkSmartPointer<vtkRenderer> renderer, 
        vtkSmartPointer<vtkRenderWindow> renderWindow, 
        vtkSmartPointer<vtkRenderWindowInteractor> interactor);
    virtual void CreatePipeline(vtkSmartPointer<vtkRenderer> renderer);
    virtual void AddMyActor(vtkSmartPointer<vtkRenderWindowInteractor> interactor);	
private:
	vtkSmartPointer<vtkBoxWidget> boxWidget;
    vtkSmartPointer<vtkActor> actor;
};


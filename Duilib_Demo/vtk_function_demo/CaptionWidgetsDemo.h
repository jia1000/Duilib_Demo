#pragma once
#include "WidgetsDemoBase.h"

#include "global_include.h"

class CCaptionWidgetsDemo : public CWidgetsDemoBase
{
public:
    CCaptionWidgetsDemo(HWND parent, RECT rc);
    ~CCaptionWidgetsDemo(void);

    void CaptionWidgets_Test();

    
    void StartRenderCaption(vtkSmartPointer<vtkRenderer> renderer, 
        vtkSmartPointer<vtkRenderWindow> renderWindow, 
        vtkSmartPointer<vtkRenderWindowInteractor> interactor);
	    
    virtual void CreateCaptionPipeline(vtkSmartPointer<vtkRenderer> renderer);

    virtual void AddCaptionActor(vtkSmartPointer<vtkRenderWindowInteractor> interactor);
		
protected:    
    vtkSmartPointer<vtkCaptionWidget> captionWidget;
};


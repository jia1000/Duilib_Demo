#pragma once
#include "WidgetsDemoBase.h"

#include "global_include.h"

class CTextWidgetsDemo : public CWidgetsDemoBase
{
public:
    CTextWidgetsDemo(HWND parent, RECT rc);
    ~CTextWidgetsDemo(void);

    void ShowWidgets_Test();

    void StartRender(vtkSmartPointer<vtkRenderer> renderer, 
        vtkSmartPointer<vtkRenderWindow> renderWindow, 
        vtkSmartPointer<vtkRenderWindowInteractor> interactor);
	    
    virtual void CreatePipeline(vtkSmartPointer<vtkRenderer> renderer);
    virtual void AddTextActor(vtkSmartPointer<vtkRenderWindowInteractor> interactor);

protected:
    vtkSmartPointer<vtkTextWidget> textWidget;

};


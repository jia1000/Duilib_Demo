#pragma once
#include "global_include.h"

class CCaptionWidgetsDemo
{
public:
    CCaptionWidgetsDemo(HWND parent, RECT rc);
    ~CCaptionWidgetsDemo(void);

    void CaptionWidgets_Test();

    
    void StartRenderCaption(vtkSmartPointer<vtkRenderer> renderer, 
        vtkSmartPointer<vtkRenderWindow> renderWindow, 
        vtkSmartPointer<vtkRenderWindowInteractor> interactor);

    vtkSmartPointer<vtkRenderer> CreateRenderer();
    vtkSmartPointer<vtkRenderWindow> CreateRenderWindow(vtkSmartPointer<vtkRenderer> renderer);
    vtkSmartPointer<vtkRenderWindowInteractor> CreateInteractor(vtkSmartPointer<vtkRenderWindow> renderWindow);

    virtual void CreateCaptionPipeline(vtkSmartPointer<vtkRenderer> renderer);

    virtual void AddCaptionActor(vtkSmartPointer<vtkRenderWindowInteractor> interactor);

	void ResizeAndPosition(RECT rc);
protected:
    vtkSmartPointer<vtkRenderer> m_renderer;
    vtkSmartPointer<vtkRenderWindow> m_renderWindow;
    vtkSmartPointer<vtkRenderWindowInteractor> m_interactor;

    vtkSmartPointer<vtkCaptionWidget> captionWidget;
	HWND m_parentWnd;
	RECT m_rc;
};


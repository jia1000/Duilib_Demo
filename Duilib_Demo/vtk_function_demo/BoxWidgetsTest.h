#pragma once

#include "global_include.h"

class CBoxWidgetsTest 
{
public:
    CBoxWidgetsTest(HWND parent, RECT rc);
    ~CBoxWidgetsTest(void);

	void ShowWidgets_Test();

	virtual vtkSmartPointer<vtkRenderer> CreateRenderer();
	virtual vtkSmartPointer<vtkRenderWindow> CreateRenderWindow(vtkSmartPointer<vtkRenderer> renderer);
	virtual vtkSmartPointer<vtkRenderWindowInteractor> CreateInteractor(vtkSmartPointer<vtkRenderWindow> renderWindow);
	
    virtual void SetMyInteractorStyle(vtkSmartPointer<vtkRenderWindowInteractor> interactor);

    virtual void StartWidgetsRender(vtkSmartPointer<vtkRenderer> renderer, 
        vtkSmartPointer<vtkRenderWindow> renderWindow, 
        vtkSmartPointer<vtkRenderWindowInteractor> interactor);
    virtual void CreatePipeline(vtkSmartPointer<vtkRenderer> renderer);
    virtual void AddMyActor(vtkSmartPointer<vtkRenderWindowInteractor> interactor);
	void ResizeAndPosition(RECT rc);
private:
	vtkSmartPointer<vtkRenderer>                m_renderer;
	vtkSmartPointer<vtkRenderWindow>            m_renderWindow;
	vtkSmartPointer<vtkRenderWindowInteractor>  m_interactor;

    vtkSmartPointer<vtkBoxWidget> boxWidget;
    vtkSmartPointer<vtkActor> actor;

	HWND m_parentWnd;
	RECT m_rc;
};


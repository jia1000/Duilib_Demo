#pragma once
#include "global_include.h"

class vtkExampleBase
{
public:
    vtkExampleBase(HWND parent, CButtonUI* vtkShowBtn);
    ~vtkExampleBase(void);

	virtual void InitVtkExampleDemo();
	
	virtual void ShowVtkExampleDemo();
	
	void ResizeAndPosition();

protected:
    vtkSmartPointer<vtkRenderWindow>            m_renderWindow;

	HWND m_parentWnd;
	CButtonUI* m_vtkShowBtn;
};


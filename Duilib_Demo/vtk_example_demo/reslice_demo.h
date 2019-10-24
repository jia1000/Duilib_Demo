#pragma once
#include "vtkExampleBase.h"

#include "global_include.h"

class Reslice_Demo  : public vtkExampleBase
{
public:
    Reslice_Demo(HWND parent, CButtonUI* vtkShowBtn);
    ~Reslice_Demo(void);

	virtual void InitVtkExampleDemo();

	virtual void ShowVtkExampleDemo();
		
private:
	vtkSmartPointer<vtkRenderer>                m_renderer;	
	vtkSmartPointer<vtkRenderWindowInteractor>  m_interactor;
};


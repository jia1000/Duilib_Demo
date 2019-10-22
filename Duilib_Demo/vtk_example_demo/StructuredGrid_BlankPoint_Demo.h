#pragma once
#include "vtkExampleBase.h"

#include "global_include.h"

class StructuredGrid_BlankPoint_Demo  : public vtkExampleBase
{
public:
    StructuredGrid_BlankPoint_Demo(HWND parent, CButtonUI* vtkShowBtn);
    ~StructuredGrid_BlankPoint_Demo(void);

	virtual void InitVtkExampleDemo();

	virtual void ShowVtkExampleDemo();

private:
	vtkSmartPointer<vtkRenderer>                m_renderer;	
	vtkSmartPointer<vtkRenderWindowInteractor>  m_interactor;
};


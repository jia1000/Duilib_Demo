#pragma once
#include "vtkExampleBase.h"

#include "global_include.h"

class Red_Book_VolumeOfInterest_Demo  : public vtkExampleBase
{
public:
    Red_Book_VolumeOfInterest_Demo(HWND parent, CButtonUI* vtkShowBtn);
    ~Red_Book_VolumeOfInterest_Demo(void);

	virtual void InitVtkExampleDemo();

	virtual void ShowVtkExampleDemo();
		
private:
	vtkSmartPointer<vtkRenderer>                m_renderer;	
	vtkSmartPointer<vtkRenderWindowInteractor>  m_interactor;

	vtkSmartPointer<vtkContourWidget> contourWidget;
};


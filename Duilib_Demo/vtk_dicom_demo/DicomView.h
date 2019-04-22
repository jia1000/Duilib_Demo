#pragma once
#include "global_include.h"

//#include <vtkSmartPointer.h>
//#include <vtkImageViewer2.h>

class CDicomView
{
public:
    CDicomView(HWND parent, int orientation, RECT rc);
    ~CDicomView(void);

    void ShowDicomFile(std::string folder);
	void UpdateView(RECT rc);

private:
    vtkSmartPointer<vtkTextMapper> SetLeftDownTextAnonationMapper(vtkSmartPointer<vtkImageViewer2> imageViewer);
    vtkSmartPointer<vtkTextMapper> SetLeftUpAnonationMapper();
    vtkSmartPointer<vtkTextMapper> SetRightDownTextAnonationMapper(vtkSmartPointer<vtkImageViewer2> imageViewer);

    void AddLeftDownSliceTextActor(vtkSmartPointer<vtkTextMapper> sliceTextMapper);
    void AddLeftUpTextActor(vtkSmartPointer<vtkTextMapper> usageTextMapper);
    void AddRightDownSliceTextActor(vtkSmartPointer<vtkTextMapper> sliceTextMapper);


    vtkSmartPointer< vtkImageViewer2 >           m_imageViewer;
    vtkSmartPointer<vtkDICOMImageReader> m_reader;

	void ResizeAndPosition(RECT rc);

private:
	HWND m_parentWnd;
	int use_orientation;
	RECT m_rc;
};


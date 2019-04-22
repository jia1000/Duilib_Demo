#pragma once
#include "global_include.h"


class CVtkFunctionDemo
{
public:
    CVtkFunctionDemo(HWND parent, RECT rc);
    ~CVtkFunctionDemo(void);

	void ResizeAndPosition(RECT rc);
	void Function1();


private:
	vtkSmartPointer<vtkRenderWindow> renWin;

	HWND m_parentWnd;	
	RECT m_rc;
};


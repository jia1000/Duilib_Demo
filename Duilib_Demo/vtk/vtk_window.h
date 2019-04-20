#pragma once
#include <UIlib.h>
using namespace DuiLib;

#include "global_include.h"

//////////////////////////////////////////////////////////////////////////
// 该类使用xml文件创建窗口
//////////////////////////////////////////////////////////////////////////

class TileLayoutExtended;
class CDuiVistaSimple;

class DuiVtkWindow : public WindowImplBase
{
public:
	DuiVtkWindow(void);
	~DuiVtkWindow(void);

	virtual LPCTSTR GetWindowClassName() const ;
	virtual void OnFinalMessage(HWND hWnd);
	virtual void InitWindow();
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual void    Notify(TNotifyUI& msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//
	void SetBtnText(CDuiString text);
	void SetLayoutManager(TileLayoutExtended* layout_manager);
private:
	TileLayoutExtended* m_layout_manager;
	CButtonUI* m_btn;
	CButtonUI* m_vtk;
	CDuiVistaSimple* m_win;

	void test_vtk_show();
	void vtk_init();

	// vtk相关的变量
	vtkSmartPointer<vtkJPEGReader> reader;
	vtkSmartPointer<vtkImageShrink3D> shrinkFilter;
	vtkSmartPointer<vtkImageActor> originalActor;
	vtkSmartPointer<vtkImageActor> shrinkActor;
	vtkSmartPointer<vtkRenderer> originalRenderer;
	vtkSmartPointer<vtkRenderer> shrinkRenderer;
	vtkSmartPointer<vtkRenderWindow> renderWindow;
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;
	vtkSmartPointer<vtkInteractorStyleImage> style;

};


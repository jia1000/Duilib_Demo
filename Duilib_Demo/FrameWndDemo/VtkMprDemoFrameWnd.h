#pragma once
#include <UIlib.h>
using namespace DuiLib;

#include "global_include.h"

//////////////////////////////////////////////////////////////////////////
// 该类使用xml文件创建窗口
//////////////////////////////////////////////////////////////////////////

class vtkViewImage2D;
class vtkViewImage3D;

class CVtkMprDemoFrameWnd : public WindowImplBase
{
public:
	CVtkMprDemoFrameWnd(void);
	~CVtkMprDemoFrameWnd(void);

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
	void initViews();
private:
	vtkSmartPointer< vtkRenderer >                           m_AxialRenderder;
	vtkSmartPointer< vtkRenderer >                           m_SagittalRenderder;
	vtkSmartPointer< vtkRenderer >                           m_CoronalRenderder;
	vtkSmartPointer< vtkRenderer >                           m_3DRenderder;

	vtkSmartPointer< vtkRenderWindow >                 m_AxialRenWin;
	vtkSmartPointer< vtkRenderWindow >                 m_SagittalRenWin;
	vtkSmartPointer< vtkRenderWindow >                 m_CoronalRenWin;
	vtkSmartPointer< vtkRenderWindow >                 m_3DRenWin;

	vtkSmartPointer< vtkRenderWindowInteractor >  m_AxialInteractor;
	vtkSmartPointer< vtkRenderWindowInteractor >  m_SagittalInteractor;
	vtkSmartPointer< vtkRenderWindowInteractor >  m_CoronalInteractor;
	vtkSmartPointer< vtkRenderWindowInteractor >  m_3DInteractor;

	vtkSmartPointer< vtkViewImage2D >           m_AxialImage;
	vtkSmartPointer< vtkViewImage2D >           m_SagittalImage;
	vtkSmartPointer< vtkViewImage2D >           m_CoronalImage;
	vtkSmartPointer< vtkViewImage3D >           m_3DImage;
};


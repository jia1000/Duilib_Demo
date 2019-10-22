#pragma once
#include <UIlib.h>
using namespace DuiLib;

//////////////////////////////////////////////////////////////////////////
// 该类使用xml文件创建窗口
//////////////////////////////////////////////////////////////////////////

class CVtkFunctionDemo;
class CDicomView;
class MultiSlicesImageDemo;

class CVtkExampleDemoFrameWnd : public WindowImplBase
{
public:
	CVtkExampleDemoFrameWnd(void);
	~CVtkExampleDemoFrameWnd(void);

	virtual LPCTSTR GetWindowClassName() const ;
	virtual void OnFinalMessage(HWND hWnd);
	virtual void InitWindow();
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual void    Notify(TNotifyUI& msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	CVtkFunctionDemo* m_vtk_function_demo;
	CDicomView* dicom_view;

	MultiSlicesImageDemo *box_widgets;
};


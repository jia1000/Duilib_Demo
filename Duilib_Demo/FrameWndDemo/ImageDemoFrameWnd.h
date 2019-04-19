#pragma once
#include <UIlib.h>
using namespace DuiLib;

//////////////////////////////////////////////////////////////////////////
// 该类使用xml文件创建窗口
//////////////////////////////////////////////////////////////////////////

struct Image_Rect_Info
{
	int x_center;
	int y_center;
	int height;
	int widht;
};

class CImageDemoFrameWnd : public WindowImplBase
{
public:
	CImageDemoFrameWnd(void);
	~CImageDemoFrameWnd(void);

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
	void UpdateImageControl();
	RECT GetImageRect();
	void ModifyImageWidth(int delta);
	void ModifyImageCenter(int x_delta, int y_delta);
	
	std::wstring m_file_name;
	int file_index;
	std::vector<std::wstring> m_vFiles;

	Image_Rect_Info m_image_rect_info;

	int m_move_step;

	CButtonUI* m_pImageControl;
};


#pragma once
#include <UIlib.h>
using namespace DuiLib;

#include "FrameWndDemo/FirstFrameWnd.h"
#include "FrameWndDemo/SecondFrameWnd.h"

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_ud.lib")
#   else
#       pragma comment(lib, "DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_u.lib")
#   else
#       pragma comment(lib, "DuiLib.lib")
#   endif
#endif


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	 // 设置资源的默认路径（此处设置为和exe在同一目录）
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("..\\..\\Bin\\Skin\\"));  

	//CFirstFrameWnd duiFrame;
	CSecondFrameWnd duiFrame;
	duiFrame.Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, 0L);
	duiFrame.CenterWindow();
	duiFrame.ShowModal();

	return 0;
}
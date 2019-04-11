#pragma once
#include <UIlib.h>
using namespace DuiLib;

#include "FrameWndDemo/FirstFrameWnd.h"
#include "FrameWndDemo/SecondFrameWnd.h"
#include "FrameWndDemo/CefBrowserFrameWnd.h"

#include "cef\simple_app.h"


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

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;

	// CEF 初始化 相关
	CefMainArgs args(hInstance);
	//创建CefApp实例
	CefRefPtr<SimpleApp> app(new SimpleApp);
	// 这个函数的执行在browser进程时会立即返回，
	// 在子进程时会堵塞直到退出时返回。
	int exitCode = CefExecuteProcess(args, app, NULL);
	if (exitCode >= 0)
	{
		return exitCode;
	}
	//填充这个结构体，用于定制CEF的行为。
	CefSettings settings;
	//初始化CEF
	CefInitialize(args, settings, app, NULL);



	//CFirstFrameWnd duiFrame;
	//CSecondFrameWnd duiFrame;
	CCefBrowserFrameWnd duiFrame;
	duiFrame.Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, 0L);
	duiFrame.CenterWindow();
	duiFrame.ShowWindow();


	// CEF 结束处理相关
	//执行消息循环,此时会堵塞，直到CefQuitMessageLoop()函数被调用。
	CefRunMessageLoop();
	// 关闭CEF，释放资源  
	CefShutdown();


	/*CPaintManagerUI::MessageLoop();*/

	return 0;
}
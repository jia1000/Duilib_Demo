#pragma once
#include <UIlib.h>
using namespace DuiLib;

#include "FrameWndDemo/EntryFrameWnd.h"
#include "FrameWndDemo/CefBrowserClientFrameWnd.h"
#include "FrameWndDemo/DcmtkDLDicomDemoFrameWnd.h"

#include "cef_simple/simple_app.h"
#include "base/Cef3/common/client_app.h"
#include "base/Cef3/browser/client_browser_app.h"
#include "base/Cef3/renderer/client_renderer_app.h"
#include "base/Cef3/browser/ClientSchemeHandler.h"

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

#ifdef _DEBUG
#ifdef _USED_CHROME_69_3497
#pragma comment(lib, "libcef_wrapper_wrapper_69_3497/libcef_wrapper_wrapper_d.lib")
#else
#pragma comment(lib, "libcef_wrapper_wrapper_49_2623/libcef_wrapper_wrapper_d.lib")
#endif
#else	
#ifdef _USED_CHROME_69_3497
#pragma comment(lib, "libcef_wrapper_wrapper_69_3497/libcef_wrapper_wrapper.lib")
#else
#pragma comment(lib, "libcef_wrapper_wrapper_49_2623/libcef_wrapper_wrapper.lib")
#endif
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
	//CefRefPtr<SimpleApp> app(new SimpleApp);
	// 根据进程来创建不同的CefApp实例
	CefRefPtr<ClientApp> app;

	// 获取进程类型
	ClientApp::ProcessType processType = ClientApp::GetProcessType();

	// 建立Render进程，尝试Render进程和Browser进程进行数据交互，即，JS和C++交互
	// browser进程：创建ClientAppBrowser实例
	if (processType == ClientApp::ProcessType::BrowserProcess)
	{
		app = new ClientAppBrowser;
	}
	// render进程：创建ClientAppRender实例
	else if (processType == ClientApp::ProcessType::RendererProcess)
	{
		app = new ClientAppRender;
	}
	// 其他进程：创建ClientApp实例
	else
	{
		app = new ClientApp;
	}


#if 1 //def RENDERER_DEBUG
	if (processType == ClientApp::ProcessType::RendererProcess){
		MessageBox(NULL, _T("To attach renderer process!"), _T("test"), 0);
	}
#endif

	// 这个函数的执行在browser进程时会立即返回，
	// 在子进程时会堵塞直到退出时返回。
	int exitCode = CefExecuteProcess(args, app, NULL);
	if (exitCode >= 0)
	{
		return exitCode;
	}
	//填充这个结构体，用于定制CEF的行为。
	CefSettings settings;
	CefSettingsTraits::init(&settings);
	CefString(&settings.locale).FromString("zh-CN");
	//初始化CEF
	bool ret = CefInitialize(args, settings, app, NULL);

	if (ret){
		// 注册ClientSchemeHandler
		// 格式：client://resources/
		//CefRegisterSchemeHandlerFactory(HANDLER_SCHEME_NAME, HANDLER_DOMAIN_NAME,
		//	new ClientSchemeHandlerFactory());
		// 使用XMLHttpRequest进行交互：第1步：注册Scheme
		CefRegisterSchemeHandlerFactory("http", HANDLER_DOMAIN_NAME, new ClientSchemeHandlerFactory);
	};

	//CFirstFrameWnd duiFrame;
	//CSecondFrameWnd duiFrame;
	//CCefBrowserFrameWnd duiFrame;
	//CEntryFrameWnd duiFrame;
	DcmtkDLDicomDemoFrameWnd duiFrame;
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
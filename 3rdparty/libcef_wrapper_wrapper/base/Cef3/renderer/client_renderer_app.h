#pragma once

#include "../common/client_app.h"

class ClientAppRender
	: public ClientApp
	, public CefRenderProcessHandler
{
public:
	
	// CefApp methods:
	virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE{ return this; }

	//CefRenderProcessHandler methods
	virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) OVERRIDE;

	//消息接收
	virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
		CefProcessId source_process,
		CefRefPtr<CefProcessMessage> message);

	virtual void OnContextReleased(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefV8Context> context) OVERRIDE 
	{
		// JS和C++异步通信：第04.2步
		// 调用MessageRoute的同名方法
		message_router_->OnContextReleased(browser,  frame, context);
	}

	virtual void OnWebKitInitialized() OVERRIDE ;

private:
	// Handles the renderer side of query routing.
	CefRefPtr<CefMessageRouterRendererSide> message_router_;

	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(ClientAppRender);
};

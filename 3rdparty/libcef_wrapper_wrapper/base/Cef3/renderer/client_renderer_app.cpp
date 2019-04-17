#include "stdafx.h"

#include "client_renderer_app.h"
#include "client_v8handler.h"
#include "v8handler_function.h"

void ClientAppRender::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
	// Asynchronous JavaScript Bindings
	message_router_->OnContextCreated(browser, frame, context);

	// Retrieve the context's window object.
	CefRefPtr<CefV8Value> object = context->GetGlobal();
	// Create an instance of my CefV8Handler object.
	CefRefPtr<CefV8Handler> handler = new V8Handler();
	
	for (const auto& funName : V8WindowFunciotn::Functions)
	{
		// Create the "myfunc" function.
		CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction(funName, handler);
		// Add the "myfunc" function to the "window" object.
		object->SetValue(funName, func, V8_PROPERTY_ATTRIBUTE_NONE);
	}

	{
		// 需要向JS引擎，注册函数。目的，JS在执行某函数时，Browser进程的OnProcessMessageReceived函数，能正确解析
		// Create the "myfunc" function.
		CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction("MaximizeWnd", handler);
		// Add the "myfunc" function to the "window" object.
		object->SetValue("MaximizeWnd", func, V8_PROPERTY_ATTRIBUTE_NONE);

		// Create the "myfunc" function.
		CefRefPtr<CefV8Value> func2 = CefV8Value::CreateFunction("RestoreWnd", handler);
		// Add the "myfunc" function to the "window" object.
		object->SetValue("RestoreWnd", func2, V8_PROPERTY_ATTRIBUTE_NONE);
	}
	// C++ 和 JS 交互 方式之：窗体绑定
	// 官方 窗体绑定 demo
	{
		// Retrieve the context's window object.
		CefRefPtr<CefV8Value> object = context->GetGlobal();
	
		// Create a new V8 string value. See the "Basic JS Types" section below.
		CefRefPtr<CefV8Value> str = CefV8Value::CreateString("This is window binding 1.");
	
		// Add the string to the window object as "window.myval". See the "JS Objects" section below.
		object->SetValue("myval", str, V8_PROPERTY_ATTRIBUTE_NONE);
	}
}

void ClientAppRender::OnWebKitInitialized()  
{
	// Create the renderer-side router for query handling.
	CefMessageRouterConfig config;
	message_router_ = CefMessageRouterRendererSide::Create(config);

	{
		// C++ 和 JS 交互 方式之：窗体绑定 扩展
		// Define the extension contents.
		std::string extensionCode =
			"var test;"
			"if (!test)"
			"  test = {};"
			"(function() {"
			"  test.myval = 'This is window binding extension!';"
			"})();";

		// Register the extension.
		CefRegisterExtension("v8/test", extensionCode, NULL);
	}
}

bool ClientAppRender::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
		CefProcessId source_process,
		CefRefPtr<CefProcessMessage> message) {

	if (message_router_->OnProcessMessageReceived(
		browser, source_process, message))
		return true;
	return false;
}
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

	// C++ 和 JS 交互 方式之：窗体绑定
	// 官方 窗体绑定 demo
	{
		// Retrieve the context's window object.
		CefRefPtr<CefV8Value> object = context->GetGlobal();
	
		// Create a new V8 string value. See the "Basic JS Types" section below.
		CefRefPtr<CefV8Value> str = CefV8Value::CreateString("My Value! this is test.");
	
		// Add the string to the window object as "window.myval". See the "JS Objects" section below.
		object->SetValue("myval", str, V8_PROPERTY_ATTRIBUTE_NONE);
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
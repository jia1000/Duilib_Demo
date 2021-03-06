#include "StdAfx.h"
#include "client_v8handler.h"

//#define USE_AES

bool V8Handler::Execute( const CefString& name, CefRefPtr<CefV8Value> object, \
							const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception )
{
	CefRefPtr<CefV8Context> context = CefV8Context::GetCurrentContext();

	CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create( name );

	// Retrieve the argument list object.
	CefRefPtr<CefListValue> args = msg->GetArgumentList();

	// Populate the argument values.
	int _size = arguments.size();

	args->SetSize(_size);

	for (int i = 0; i < _size; i++)
	{
		if (arguments[i]->IsString())
		{
			args->SetString(i, arguments[i]->GetStringValue());
		}
		else if (arguments[i]->IsInt())
		{
			args->SetInt(i, arguments[i]->GetIntValue());
		}
		else if (arguments[i]->IsDouble())
		{
			args->SetDouble(i, arguments[i]->GetDoubleValue());
		}
		else if (arguments[i]->IsBool())
		{
			args->SetBool(i, arguments[i]->GetBoolValue());
		}
		else
		{
			args->SetString(i, arguments[i]->GetStringValue());
		}
	}

	//handle custom functions for js call
	if (msg->GetName() == V8WindowFunciotn::loginFunc){
		_size = arguments.size();

		std::string d1 = args->GetString(0);
		std::string d2 = args->GetString(1);

		retval = CefV8Value::CreateString(d1 + " " + d2);

	}else if(msg->GetName() == V8WindowFunciotn::domTest){

		/*_size = arguments.size();		
		CefRefPtr<CefDOMVisitor> visitor = new SimpleDomVisitor();
		context->GetBrowser()->GetMainFrame()->VisitDOM(visitor);*/
	} else if(msg->GetName() == "my_test_js_func"){  //接收JS函数
		// Return my string value.
		retval = CefV8Value::CreateString("This is JS function!");
		return true;
	}
	// 浏览器会分发侦听到的事件，如果是JS有关的，则交由V8Handler处理
	context->GetBrowser()->SendProcessMessage(PID_BROWSER, msg);

	return true;
}

std::string V8Handler::GenerateKey(const string& str)
{
	return str;
}

/////////////////////////////////////////////////////////////////////////////////

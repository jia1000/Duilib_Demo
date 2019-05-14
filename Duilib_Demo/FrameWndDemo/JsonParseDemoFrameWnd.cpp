#include "JsonParseDemoFrameWnd.h"

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/reader.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

CJsonParseDemoFrameWnd::CJsonParseDemoFrameWnd(void)
{
}


CJsonParseDemoFrameWnd::~CJsonParseDemoFrameWnd(void)
{
}

LPCTSTR CJsonParseDemoFrameWnd::GetWindowClassName() const 
{ 
	return _T("JsonParseDemoFrameWnd"); 
}

void CJsonParseDemoFrameWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	//delete this; //退出时，会有异常
}

CDuiString CJsonParseDemoFrameWnd::GetSkinFile()
{
	return _T("frame_wnd_demo\\json_parse_demo_frame.xml");
}

CDuiString CJsonParseDemoFrameWnd::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");

}

CControlUI* CJsonParseDemoFrameWnd::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CJsonParseDemoFrameWnd::InitWindow()
{
	
}

LRESULT CJsonParseDemoFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	__super::OnSize(uMsg, wParam, lParam, bHandled);
	return 0;
}


void    CJsonParseDemoFrameWnd::Notify(TNotifyUI& msg) 
{
	if (msg.sType == _T("click")) {
		LPCTSTR pszCtrlName = msg.pSender->GetName().GetData();
		if (_tcscmp(pszCtrlName, _T("btn_sysmin")) == 0) {
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		} else if (_tcscmp(pszCtrlName, _T("btn_sysmax")) == 0) {
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
			// 最大化和还原2个button之间的切换
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_sysmax")));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_restore")));
			if( pControl ) pControl->SetVisible(true);
		} else if (_tcscmp(pszCtrlName, _T("btn_restore")) == 0) {
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
			// 最大化和还原2个button之间的切换
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_sysmax")));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_restore")));
			if( pControl ) pControl->SetVisible(false);
		} else if (_tcscmp(pszCtrlName, _T("btn_sysclose")) == 0) {
			Close(IDOK);
		}else if (_tcscmp(pszCtrlName, _T("btn_test")) == 0) {
			ParseJsonData();
		}
	}
}

LRESULT CJsonParseDemoFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg, wParam, lParam);
}

void CJsonParseDemoFrameWnd::ParseJsonData()
{
	const char* str = "{\"name\":\"xiaoming\",\"age\":18,\"job\":\"coder\",\"a\":{\"b\":1}}";

	Document doc;
	// 在DOM中解析json字符串
	doc.Parse(str);

	// 读取json数据
	const Value& age = doc["age"];	
	int age_1 = age.GetInt();

	const  Value& name = doc["name"];
	std::string s_name = name.GetString();

	printf("haah");
}
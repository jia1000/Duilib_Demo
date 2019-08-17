#include "DcmtkDLDicomSetDemoFrameWnd.h"


DcmtkDLDicomSetDemoFrameWnd::DcmtkDLDicomSetDemoFrameWnd(void)
{
}


DcmtkDLDicomSetDemoFrameWnd::~DcmtkDLDicomSetDemoFrameWnd(void)
{
}

LPCTSTR DcmtkDLDicomSetDemoFrameWnd::GetWindowClassName() const 
{ 
	return _T("DcmtkDLDicomSetDemoFrameWnd"); 
}

void DcmtkDLDicomSetDemoFrameWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	//delete this; //退出时，会有异常
}

CDuiString DcmtkDLDicomSetDemoFrameWnd::GetSkinFile()
{
	return _T("frame_wnd_demo\\dcmtk_dl_dicom_set_demo_frame.xml");
}

CDuiString DcmtkDLDicomSetDemoFrameWnd::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");

}

CControlUI* DcmtkDLDicomSetDemoFrameWnd::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void DcmtkDLDicomSetDemoFrameWnd::InitWindow()
{
	m_pNumberEdit = static_cast<CEditUI*>(m_pm.FindControl(L"dicom_node_number"));
	m_pAetEdit = static_cast<CEditUI*>(m_pm.FindControl(L"dicom_node_aet"));
	m_pHostEdit = static_cast<CEditUI*>(m_pm.FindControl(L"dicom_node_host"));
	m_pDicomPortEdit = static_cast<CEditUI*>(m_pm.FindControl(L"dicom_node_port"));
	//m_pPduSpinner = static_cast<CSpinnerUI*>(m_pm.FindControl(L"dicom_node_pdu_spinner"));
	m_pSerchMethodCmb = static_cast<CComboUI*>(m_pm.FindControl(L"dicom_node_serch_method"));

	m_pNumberEdit->SetText(L"253");
	m_pAetEdit->SetText(L"DEEPWISESCP");
	m_pHostEdit->SetText(L"192.168.1.253");
	m_pDicomPortEdit->SetText(L"22222");
}

LRESULT DcmtkDLDicomSetDemoFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	__super::OnSize(uMsg, wParam, lParam, bHandled);
	return 0;
}


void    DcmtkDLDicomSetDemoFrameWnd::Notify(TNotifyUI& msg) 
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
		}  else if (_tcscmp(pszCtrlName, _T("dicom_node_test_connect")) == 0) {
			DoConnectTest();
		} 
	}
}

LRESULT DcmtkDLDicomSetDemoFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg, wParam, lParam);
}
//
//#include"dcmtk\config\osconfig.h"
//#include"dcmtk\dcmdata\dctk.h"
//
//#include "utility_tool/string_converse.h"

void DcmtkDLDicomSetDemoFrameWnd::DoConnectTest()
{
	std::wstring aet_title = m_pAetEdit->GetText().GetData();
	std::wstring host_addr = m_pHostEdit->GetText().GetData();
	std::wstring port	   = m_pDicomPortEdit->GetText().GetData();
	//EchoAssociation as("C-ECHO");
	//CONDITION cond;
}


#include "DcmtkDLDicomSetDemoFrameWnd.h"

#include "main/controllers/dcmtk/dicomnetwork.h"
#include "main/controllers/dcmtk/dicomechoassociation.h"

#include "utility_tool/string_converse.h"

#include "controller/configcontroller.h"

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
	m_pDicomPdutEdit = static_cast<CEditUI*>(m_pm.FindControl(L"dicom_node_pdu_spinner"));
	m_pLocalNameEdit = static_cast<CEditUI*>(m_pm.FindControl(L"dicom_node_local_name"));
	//m_pPduSpinner = static_cast<CSpinnerUI*>(m_pm.FindControl(L"dicom_node_pdu_spinner"));
	m_pSerchMethodCmb = static_cast<CComboUI*>(m_pm.FindControl(L"dicom_node_serch_method"));
	m_pLabelStatus = static_cast<CLabelUI*>(m_pm.FindControl(L"dicom_node_test_status"));

	ST_Filter_Condition filter_condition;

	std::wstring text = L"";
	if (m_pNumberEdit) {
		text = ConfigController::Instance()->GetAETNumber();
		m_pNumberEdit->SetText(text.c_str());
	}

	if (m_pAetEdit) {
		text = ConfigController::Instance()->GetAETTitle();
		m_pAetEdit->SetText(text.c_str());
	}

	if (m_pHostEdit) {
		text = ConfigController::Instance()->GetAETHost();
		m_pHostEdit->SetText(text.c_str());
	}

	if (m_pDicomPortEdit) {
		text = ConfigController::Instance()->GetAETPort();
		m_pDicomPortEdit->SetText(text.c_str());
	}

	if (m_pDicomPdutEdit) {
		text = ConfigController::Instance()->GetAETPdu();
		m_pDicomPdutEdit->SetText(text.c_str());
	} 
	if (m_pLocalNameEdit) {
		text = ConfigController::Instance()->GetAETLocalName();
		m_pLocalNameEdit->SetText(text.c_str());
	}

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
		} else if (_tcscmp(pszCtrlName, _T("dicom_node_cancel")) == 0) {
			Close(IDOK);
		} else if (_tcscmp(pszCtrlName, _T("dicom_node_test_connect")) == 0) {
			DoConnectTest();
		} else if (_tcscmp(pszCtrlName, _T("dicom_node_accept")) == 0) {
			DoSaveConfigTest();
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
	std::wstring ws_aet_title	= m_pAetEdit->GetText().GetData();
	std::wstring ws_host_addr	= m_pHostEdit->GetText().GetData();
	std::wstring ws_port		= m_pDicomPortEdit->GetText().GetData();
	std::wstring ws_pdu			= m_pDicomPdutEdit->GetText().GetData();
	std::wstring ws_local_name	= m_pLocalNameEdit->GetText().GetData();

	bool success = true;
	std::ostringstream errorMsg;
	std::string errorTitle;

	EchoAssociation as("C-ECHO");
	CONDITION cond;

	std::string aet_title	= toString(ws_aet_title);
	std::string host_addr	= toString(ws_host_addr);
	std::string port		= toString(ws_port);
	std::string aet_local	= toString(ws_local_name);
	std::string pdu = toString(ws_pdu);
	
	int psdu_length			= atoi(pdu.c_str());

	as.Create(aet_title
		, host_addr
		, atoi(port.c_str())
		, aet_local
		, UID_VerificationSOPClass
		);
	GIL::DICOM::DCMTK::Network* pNetwork = NULL;
	try {
		pNetwork = GIL::DICOM::DCMTK::Network::Instance(this);
	}
	catch(const std::exception& ex)
	{
		success = false;
		//errorMsg << _Std("Unable to connect: Could not start network support.") << ":\n" << ex.what();
		errorMsg << "Unable to connect: Could not start network support." << ":\n" << ex.what();
	}

	if (success) {
		as.SetTimeout(10);
		pNetwork->InitializeNetwork(as.GetTimeout());
		cond = pNetwork->ConnectAssociation(&as, psdu_length);//m_pPduSpinner->GetValue());
		if (cond.good()) {
			cond = as.SendEchoRequest();
			if (cond.bad())	{
				success = false;
				//errorMsg << _Std("The service did not respond correctly to the C-ECHO") << ":\n" << cond.text();
				errorMsg << "The service did not respond correctly to the C-ECHO" << ":\n" << cond.text();

			}	
		}
		else {
			success = false;
			//errorMsg << _Std("The service did not respond correctly to the DICOM standard") << ":\n" << cond.text();
			errorMsg << "The service did not respond correctly to the DICOM standard" << ":\n" << cond.text();

		}
		as.Drop(cond);
		GIL::DICOM::DCMTK::Network::FreeInstance(this);
		pNetwork = NULL;
				
		if (m_pLabelStatus) {
			if (success) {
				m_pLabelStatus->SetText(L"成功");
			} else {
				m_pLabelStatus->SetText(L"失败");
			}
		}
	}
}

void DcmtkDLDicomSetDemoFrameWnd::DoSaveConfigTest()
{
	std::wstring ws = L"";
	if (m_pNumberEdit) {
		ws = m_pNumberEdit->GetText().GetData();
		ConfigController::Instance()->SetAETNumber(ws);
	}
	if (m_pAetEdit) {
		ws = m_pAetEdit->GetText().GetData();
		ConfigController::Instance()->SetAETTitle(ws);
	}
	if (m_pHostEdit) {
		ws = m_pHostEdit->GetText().GetData();
		ConfigController::Instance()->SetAETHost(ws);
	}
	if (m_pDicomPortEdit) {
		ws = m_pDicomPortEdit->GetText().GetData();
		ConfigController::Instance()->SetAETPort(ws);
	}
	if (m_pDicomPdutEdit) {
		ws = m_pDicomPdutEdit->GetText().GetData();
		ConfigController::Instance()->SetAETPduLength(ws);
	}
	if (m_pLocalNameEdit) {
		ws = m_pLocalNameEdit->GetText().GetData();
		ConfigController::Instance()->SetAETLocalName(ws);
	}
	ConfigController::Instance()->SaveFile();
}
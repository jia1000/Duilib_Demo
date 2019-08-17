#include "DcmtkDLDicomDemoFrameWnd.h"


DcmtkDLDicomDemoFrameWnd::DcmtkDLDicomDemoFrameWnd(void)
{
}


DcmtkDLDicomDemoFrameWnd::~DcmtkDLDicomDemoFrameWnd(void)
{
}

LPCTSTR DcmtkDLDicomDemoFrameWnd::GetWindowClassName() const 
{ 
	return _T("VtkDemoFrameWnd"); 
}

void DcmtkDLDicomDemoFrameWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	//delete this; //退出时，会有异常
}

CDuiString DcmtkDLDicomDemoFrameWnd::GetSkinFile()
{
	return _T("frame_wnd_demo\\dcmtk_dl_dicomdemo_frame.xml");
}

CDuiString DcmtkDLDicomDemoFrameWnd::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");

}

CControlUI* DcmtkDLDicomDemoFrameWnd::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void DcmtkDLDicomDemoFrameWnd::InitWindow()
{
	
}

LRESULT DcmtkDLDicomDemoFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	__super::OnSize(uMsg, wParam, lParam, bHandled);
	return 0;
}


void    DcmtkDLDicomDemoFrameWnd::Notify(TNotifyUI& msg) 
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
		} else if (_tcscmp(pszCtrlName, _T("btn_test")) == 0) {
			test_dcmtk();
		}
	}
}

LRESULT DcmtkDLDicomDemoFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg, wParam, lParam);
}

#include"dcmtk\config\osconfig.h"
#include"dcmtk\dcmdata\dctk.h"

#include "utility_tool/string_converse.h"

void DcmtkDLDicomDemoFrameWnd::test_dcmtk()
{
	DcmFileFormat dfile;
	dfile.loadFile("..\\Bin\\Skin\\data\\slices1\\2");

	DcmMetaInfo* Metalnfo = dfile.getMetaInfo();
	
	DcmTag Tag;
	Tag=Metalnfo->getTag();

	Uint16 G_tag=Tag.getGTag();
	DcmDataset* data=dfile.getDataset();
	DcmElement*element=NULL;
	data->findAndGetElement(DCM_PixelData,element);
	double data_len=data->getLength();
	double element_len=element->getLength();
	
	OFString patientName;
	data->findAndGetOFString(DCM_PatientName,patientName);

	OFString patientId;
	data->findAndGetOFString(DCM_PatientID,patientId);

	OFString patientAge;
	data->findAndGetOFString(DCM_PatientAge,patientAge);

	OFString PatientPosition;
	data->findAndGetOFString(DCM_PatientPosition,PatientPosition);

	OFString ImagePositionPatient;
	data->findAndGetOFString(DCM_ImagePositionPatient,ImagePositionPatient);

	OFString PixelSpacing;
	data->findAndGetOFString(DCM_PixelSpacing,PixelSpacing);

	Uint16* pixData16;
	element->getUint16Array(pixData16);

	//cv::Matimag=cv::Mat(512,512,CV_8U,pixData16);
	//cv::imshow("image",imag);
	//cv::waitKey(0);

	std::string str = patientName.c_str();
	std::wstring ws = toWString(str);
	std::wstring ws2 = toWString(patientId.c_str());
	CDuiString text;
	text.Format(_T("Patient  Name :%s  patientId : %s"), ws.c_str(), ws2.c_str());
	CButtonUI* pControl = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_test")));
	if (pControl) {
		pControl->SetText(text);
	}
}
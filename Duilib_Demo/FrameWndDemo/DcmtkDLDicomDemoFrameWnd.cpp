#include "DcmtkDLDicomDemoFrameWnd.h"

#include "DcmtkDLDicomSetDemoFrameWnd.h"
#include "main/controllers/pacscontroller.h"

#include "api/dicom/dcmdictionary.h"
#include "api/dicom/dicomdataset.h"GetConnection

#include "main/controllers/dcmtk/dicomfindassociation.h"
#include "main/controllers/dcmtk/dicomgetassociation.h"
#include "main/controllers/dcmtk/dicomnetwork.h"
#include "main/controllers/dcmtk/dicomnetclient.h"

#include "utility_tool/string_converse.h"
#include "utility_tool/common_utils.h"
#include "controller/configcontroller.h"

#include <dcmtk/dcmdata/dcdeftag.h>

#include <thread>

#define LOCAL_AE_TITLE	"DEEPWISE_002"


DcmtkDLDicomDemoFrameWnd::DcmtkDLDicomDemoFrameWnd(void)
	: m_downloading_dicom_index(0)
	, m_is_stoped(false)
{
	
}


DcmtkDLDicomDemoFrameWnd::~DcmtkDLDicomDemoFrameWnd(void)
{
}

LPCTSTR DcmtkDLDicomDemoFrameWnd::GetWindowClassName() const 
{ 
	return _T("DcmtkDLDicomDemoFrameWnd"); 
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
	if ( _tcsicmp(pstrClass, _T("ListPro")) == 0 ) {
		return new ListPro;
	}
	return NULL;
}

void DcmtkDLDicomDemoFrameWnd::InitWindow()
{
	m_pPatientCsvPathEdit = static_cast<CEditUI*>(m_pm.FindControl(L"edit_patient_csv_path"));
	m_pPatientIdEdit = static_cast<CEditUI*>(m_pm.FindControl(L"edit_find"));
	m_pBodyPartEdit = static_cast<CEditUI*>(m_pm.FindControl(L"edit_filter_part"));
	m_pThicknessEdit = static_cast<CEditUI*>(m_pm.FindControl(L"edit_filter_thickness"));
	m_pMOdalityiesInStudyEdit = static_cast<CEditUI*>(m_pm.FindControl(L"edit_filter_modality"));
	m_pDownloadPathEdit = static_cast<CEditUI*>(m_pm.FindControl(L"edit_download_path"));
	m_pSexEdit = static_cast<CEditUI*>(m_pm.FindControl(L"edit_filter_sex"));
	m_pFilterRANGE = static_cast<COptionUI*>(m_pm.FindControl(_T("op_device_date_range")));
	m_pFilterFROM  = static_cast<CDateTimeUI*>(m_pm.FindControl(L"filter_range_from"));
	m_pFilterTO  = static_cast<CDateTimeUI*>(m_pm.FindControl(L"filter_range_to"));

	m_pFilterBodyPart = static_cast<COptionUI*>(m_pm.FindControl(_T("op_device_part")));
	m_pFilterThickness = static_cast<COptionUI*>(m_pm.FindControl(_T("op_device_thickness")));
	m_pFilterModality = static_cast<COptionUI*>(m_pm.FindControl(_T("op_device_modality")));
	m_pFilterSex = static_cast<COptionUI*>(m_pm.FindControl(_T("op_device_sex")));

	m_listPro = static_cast<ListPro*>(m_pm.FindControl(L"list_download_result"));

#ifdef _DEBUG
	m_pPatientIdEdit->SetText(L"1008621671,0170952,0003852666");// 1个ct
	if (m_pDownloadPathEdit) {
		m_pDownloadPathEdit->SetText(L"G:\\temp1");
		m_dicom_saved_path = "G:\\temp1";
	}
#endif // _DEBUG
	
	ST_Filter_Condition filter_condition;
	ConfigController::Instance()->ReloadFile(".\\deepwise_pacs_download_config.ini");
	// 日期筛选
	ConfigController::Instance()->GetFilterDate(filter_condition);
	if (m_pFilterRANGE) {
		m_pFilterRANGE->Selected(filter_condition.is_checked);
	}
	if (m_pFilterFROM) {
		m_pFilterFROM->SetEnabled(filter_condition.is_checked);
	}	
	if (m_pFilterTO) {
		m_pFilterTO->SetEnabled(filter_condition.is_checked);
	}
	// 部位筛选
	ConfigController::Instance()->GetFilterBodyPart(filter_condition);
	if (m_pFilterBodyPart) {
		m_pFilterBodyPart->Selected(filter_condition.is_checked);
	}
	if (m_pBodyPartEdit) {
		m_pBodyPartEdit->SetEnabled(filter_condition.is_checked);
		std::wstring ws = toWString(filter_condition.condition_text);
		m_pBodyPartEdit->SetText(ws.c_str());
	}
	// 层厚筛选
	ConfigController::Instance()->GetFilterThickness(filter_condition);
	if (m_pFilterThickness) {
		m_pFilterThickness->Selected(filter_condition.is_checked);
	}
	if (m_pThicknessEdit) {
		m_pThicknessEdit->SetEnabled(filter_condition.is_checked);
		std::wstring ws = toWString(filter_condition.condition_text);
		m_pThicknessEdit->SetText(ws.c_str());
	}
	// 设备筛选
	ConfigController::Instance()->GetFilterModality(filter_condition);
	if (m_pFilterModality) {
		m_pFilterModality->Selected(filter_condition.is_checked);
	}
	if (m_pMOdalityiesInStudyEdit) {
		m_pMOdalityiesInStudyEdit->SetEnabled(filter_condition.is_checked);
		std::wstring ws = toWString(filter_condition.condition_text);
		m_pMOdalityiesInStudyEdit->SetText(ws.c_str());
	}

	UpdateDownloadStaticsText(0);
	UpdateDownloadListProAll();
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
		} else if (_tcscmp(pszCtrlName, _T("dicom_set")) == 0) {
			DcmtkDLDicomSetDemoFrameWnd* duiFrame = new DcmtkDLDicomSetDemoFrameWnd();
			duiFrame->Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, 0L);
			duiFrame->CenterWindow();
			duiFrame->ShowWindow();
		} else if (_tcscmp(pszCtrlName, _T("btn_filter")) == 0) {
			DoSearchStudyTest();
		} else if (_tcscmp(pszCtrlName, _T("btn_download")) == 0) {
			m_is_stoped = false;
			std::thread th(&DcmtkDLDicomDemoFrameWnd::DoDownloadTest, this);
			th.detach();
		} else if (_tcscmp(pszCtrlName, _T("btn_patient_csv_path")) == 0) {
			OnOpenPatientIDListFile();
		} else if (_tcscmp(pszCtrlName, _T("btn_download_path")) == 0) {
			OnOpenDownloadPath(); 
		} else if (_tcscmp(pszCtrlName, _T("btn_download_stop")) == 0) {
			SetDownloadStop(true); 
		} else if (_tcscmp(pszCtrlName, _T("btn_filter_save")) == 0) {
			SaveFilterCondition(); 
		} else if (_tcscmp(pszCtrlName, _T("btn_filter_clear")) == 0) {
			ClearFilterCondition(); 
		}    
	} else if (_tcsicmp(msg.sType, _T("selectchanged")) == 0) {
		OnSelChanged(msg.pSender);
	} 
}

void DcmtkDLDicomDemoFrameWnd::SaveFilterCondition()
{
	ST_Filter_Condition filter_condition;
	// 部位筛选
	filter_condition.is_checked = m_pFilterBodyPart->IsSelected();
	filter_condition.condition_text = toString(m_pBodyPartEdit->GetText().GetData());
	ConfigController::Instance()->SetFilterBodyPart(filter_condition);
	// 日期筛选
	filter_condition.is_checked = m_pFilterRANGE->IsSelected();
	filter_condition.condition_text = "";
	ConfigController::Instance()->SetFilterDate(filter_condition);
	// 层厚筛选
	filter_condition.is_checked = m_pFilterThickness->IsSelected();
	filter_condition.condition_text = toString(m_pThicknessEdit->GetText().GetData());
	ConfigController::Instance()->SetFilterThickness(filter_condition);
	// 设备筛选
	filter_condition.is_checked = m_pFilterModality->IsSelected();
	filter_condition.condition_text = toString(m_pMOdalityiesInStudyEdit->GetText().GetData());
	ConfigController::Instance()->SetFilterModality(filter_condition);

	ConfigController::Instance()->SaveFile();
}
void DcmtkDLDicomDemoFrameWnd::ClearFilterCondition()
{
	ST_Filter_Condition filter_condition;
	filter_condition.is_checked = false;
	filter_condition.condition_text = "";

	// 部位筛选
	m_pFilterBodyPart->Selected(false);
	m_pBodyPartEdit->SetText(L"");
	ConfigController::Instance()->SetFilterBodyPart(filter_condition);
	// 日期筛选
	m_pFilterRANGE->Selected(false);
	ConfigController::Instance()->SetFilterDate(filter_condition);
	// 层厚筛选
	m_pFilterThickness->Selected(false);
	m_pThicknessEdit->SetText(L"");
	ConfigController::Instance()->SetFilterThickness(filter_condition);
	// 设备筛选
	m_pFilterModality->Selected(false);
	m_pMOdalityiesInStudyEdit->SetText(L"");
	ConfigController::Instance()->SetFilterModality(filter_condition);

	ConfigController::Instance()->SaveFile();
}

LRESULT DcmtkDLDicomDemoFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT DcmtkDLDicomDemoFrameWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int index = 0;
	std::string status = "";
	switch (uMsg)
	{
	case WM_USER_UPDATE_DOWNLOAD_DICOM_FILE:
		index = lParam;
		UpdateDownloadStaticsText(index);
		break;
	case WM_USER_UPDATE_DOWNLOAD_STATUS:
		index = wParam;
		status = (char*)lParam;
		UpdateDownloadListProItem(index, status);
		break;
	default:
		break;
	}
	bHandled = FALSE;
	return 0;
}

void DcmtkDLDicomDemoFrameWnd::OnSelChanged(CControlUI* pSender)
{
	CDuiString strSelName = pSender->GetName();
	if (strSelName == _T("op_device_date_range")) {
		COptionUI* pOptionUI = static_cast<COptionUI*>(m_pm.FindControl(_T("op_device_date_range")));
		if (pOptionUI) {
			bool is_sel = pOptionUI->IsSelected();
			if (m_pFilterFROM) {
				m_pFilterFROM->SetEnabled(is_sel);
			}
			if (m_pFilterTO) {
				m_pFilterTO->SetEnabled(is_sel);
			}
		}
	} else if (strSelName == _T("op_device_part")) {
		COptionUI* pOptionUI = static_cast<COptionUI*>(m_pm.FindControl(_T("op_device_part")));
		if (pOptionUI) {
			bool is_sel = pOptionUI->IsSelected();
			if (m_pBodyPartEdit) {
				m_pBodyPartEdit->SetEnabled(is_sel);
			}
		}
	} else if (strSelName == _T("op_device_thickness")) {
		COptionUI* pOptionUI = static_cast<COptionUI*>(m_pm.FindControl(_T("op_device_thickness")));
		if (pOptionUI) {
			bool is_sel = pOptionUI->IsSelected();
			if (m_pThicknessEdit) {
				m_pThicknessEdit->SetEnabled(is_sel);
			}
		}
	} else if (strSelName == _T("op_device_modality")) {
		COptionUI* pOptionUI = static_cast<COptionUI*>(m_pm.FindControl(_T("op_device_modality")));
		if (pOptionUI) {
			bool is_sel = pOptionUI->IsSelected();
			if (m_pMOdalityiesInStudyEdit) {
				m_pMOdalityiesInStudyEdit->SetEnabled(is_sel);
			}
		}
	} else if (strSelName == _T("op_device_sex")) {
		COptionUI* pOptionUI = static_cast<COptionUI*>(m_pm.FindControl(_T("op_device_sex")));
		if (pOptionUI) {
			bool is_sel = pOptionUI->IsSelected();
			if (m_pSexEdit) {
				m_pSexEdit->SetEnabled(is_sel);
			}
		}
	}
}

void DcmtkDLDicomDemoFrameWnd::OnOpenPatientIDListFile()
{
	std::wstring def_path = L"";
	OPENFILENAME ofn = { 0 };  
    TCHAR str_filename[MAX_PATH] = { 0 };
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = m_hWnd;
    ofn.lpstrFilter = TEXT("CSV Flies(*.csv)\0*.csv\0All Files(*.*)\0*.*\0\0");
	ofn.nFilterIndex = 1;
    ofn.lpstrFile = str_filename;
    ofn.nMaxFile = sizeof(str_filename);
    ofn.lpstrInitialDir = def_path.c_str();
    ofn.lpstrTitle = NULL;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
    if (GetOpenFileName(&ofn)){
        if (m_pPatientCsvPathEdit) {
			// 在edit框上，显示CSV的路径
			m_pPatientCsvPathEdit->SetText(str_filename);
			// 读取csv文件，并在edit框上，显示patient id列表
			std::ifstream inFile(str_filename, ios::in);
			std::string line;
			std::wstring ws_patient_ids_edit(L"");
			while (getline(inFile, line)) {
				std::stringstream ss(line);
				std::string word;
				while (getline(ss, word, ','))
				{
					std::wstring ws_word = toWString(word);
					ws_patient_ids_edit += ws_word;
					ws_patient_ids_edit += L",";
				}
			}
			if (m_pPatientIdEdit) {
				m_pPatientIdEdit->SetText(ws_patient_ids_edit.c_str());
			}
        }
    }  
}



void DcmtkDLDicomDemoFrameWnd::OnOpenDownloadPath()
{
	LPITEMIDLIST pil = NULL;
	INITCOMMONCONTROLSEX init_ctrls = {0};
	TCHAR sz_buffer[4096] = {0};
	BROWSEINFO bi = {0};
	bi.hwndOwner = m_hWnd;
	bi.iImage = 0;
	bi.lParam = NULL;
	bi.lpfn = NULL;
	bi.lpszTitle = NULL;
	bi.pszDisplayName =  sz_buffer;
	bi.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;//有新建文件夹按钮

	InitCommonControlsEx(&init_ctrls);
	pil = SHBrowseForFolder(&bi);
	if (NULL != pil){
		SHGetPathFromIDList(pil, sz_buffer);
		if (m_pDownloadPathEdit) {
			m_pDownloadPathEdit->SetText(sz_buffer);
			m_dicom_saved_path = toString(sz_buffer);
		}
	} 
}
void DcmtkDLDicomDemoFrameWnd::DoSearchStudyTest()
{
	// 搜索前，先清空待下载的病历列表
	m_study_ids.clear();
	m_patient_ids.clear();
	m_patient_infos2.clear();
	
	GIL::DICOM::DicomDataset queryWrapper;
	GIL::DICOM::PACSController::Instance()->InitFindQueryWrapper(queryWrapper);
	GIL::DICOM::PACSController::Instance()->SetWrapper(queryWrapper, GKDCM_QueryRetrieveLevel, "STUDY");

	// 日期范围过滤
	if (m_pFilterRANGE && m_pFilterRANGE->IsSelected()) {
		std::ostringstream ostr;
		SYSTEMTIME time;
		if (m_pFilterFROM) {
			time = m_pFilterFROM->GetTime();
			ostr << time.wYear << std::setw( 2 ) << std::setfill( '0' ) << time.wMonth << std::setw( 2 ) << std::setfill( '0' ) << time.wDay;
		}
		ostr << "-";
		if (m_pFilterTO){
			time = m_pFilterTO->GetTime();
			ostr << time.wYear << std::setw( 2 ) << std::setfill( '0' ) << time.wMonth << std::setw( 2 ) << std::setfill( '0' ) << time.wDay;
		}
		GIL::DICOM::PACSController::Instance()->SetWrapper(queryWrapper, GKDCM_StudyDate, ostr.str());
	}

	std::wstring ws_patient_ids = m_pPatientIdEdit->GetText().GetData();	
	std::vector<std::string> patient_ids;
	if (ws_patient_ids.size() > 0) {
		patient_ids = testSplit(toString(ws_patient_ids), ",");
	}
	else {
		// patientid列表如果为空，意味着要搜索所有。
		patient_ids.push_back("");
	}

	for (int patient_index = 0; patient_index < patient_ids.size() ; patient_index++) {
		std::string patient_id = patient_ids[patient_index];
		GIL::DICOM::PACSController::Instance()->SetWrapper(queryWrapper, GKDCM_PatientID, patient_id);
		std::list< GNC::GCS::Ptr<GIL::DICOM::DicomDataset> > resultsWrapper;
		bool ret_status_obtain = GIL::DICOM::PACSController::Instance()->ObtainDicomDataSet(this, queryWrapper, resultsWrapper);

		if (ret_status_obtain) {			
			for (auto iter : resultsWrapper) {
				GNC::GCS::Ptr<GIL::DICOM::DicomDataset> item = iter;
				std::string study_id("");					
				if (item->getTag(GKDCM_StudyInstanceUID, study_id)) {
					m_study_ids.push_back(study_id);
				}
				// 如果patientid为空，表示搜索所有
				if (patient_id.size() == 0) {
					std::string p_id("");					
					if (item->getTag(GKDCM_PatientID, p_id)) {
						m_patient_ids.push_back(p_id);
					}
				}
				else {
					m_patient_ids.push_back(patient_id);
				}
			}			
		}
		else
		{
			// 统计无法找到series id的patient id
			m_patient_infos2.push_back(patient_id);
		}
	}

	DoSearchSeriesTest();
}
void DcmtkDLDicomDemoFrameWnd::DoSearchSeriesTest()
{
	m_patient_infos1.clear();
	m_listPro->RemoveAll();
	m_downloading_dicom_index = 0;

	GetAllControlValue();
		
	auto iter = m_patient_ids.begin();
	auto item = m_study_ids.begin();

	for (; item != m_study_ids.end() && iter != m_patient_ids.end(); ++item, ++iter) 
	{
		GIL::DICOM::DicomDataset base;
		GIL::DICOM::PACSController::Instance()->InitFindQueryWrapper(base);
		GIL::DICOM::PACSController::Instance()->SetWrapper(base, GKDCM_QueryRetrieveLevel, "SERIES");
		GIL::DICOM::PACSController::Instance()->SetWrapper(base, GKDCM_StudyInstanceUID, *item);
		GIL::DICOM::PACSController::Instance()->SetWrapper(base, GKDCM_PatientID, *iter);

		std::list< GNC::GCS::Ptr<GIL::DICOM::DicomDataset> > resultsWrapper;
		bool ret_status_obtain = GIL::DICOM::PACSController::Instance()->ObtainDicomDataSet(this, base, resultsWrapper);

		if (ret_status_obtain) {
			PatientInfo patient_info;
			for (auto iter : resultsWrapper) {
				GNC::GCS::Ptr<GIL::DICOM::DicomDataset> item = iter;
				std::string series_id("");					
				if (item->getTag(GKDCM_SeriesInstanceUID, series_id)) {
					bool match_condition = CheckedMatchConditions(*item);
					if (match_condition) {
						std::string series_modality("");					
						Series_Info series_info;
						series_info.download_status = DOWNLOAD_STATUS_NOT_FOUND;
						if (item->getTag(GKDCM_Modality, series_modality)) {
							series_info.series_id = series_id;
							series_info.modality = series_modality;	
							series_info.download_status = DOWNLOAD_STATUS_HAS_FOUND;
							patient_info.sereis_infos.push_back(series_info);
						}
					}
				}				
			}
			if (patient_info.sereis_infos.size() > 0) {				
				patient_info.study_id = *item;
				patient_info.patiend_id = *iter;
				m_patient_infos1.push_back(patient_info);
			}
		}
	}

	UpdateDownloadStaticsText(0);
	UpdateDownloadListProAll();
}

void DcmtkDLDicomDemoFrameWnd::UpdateDownloadStaticsText(int index)
{
	m_pStatiscResultLabel = static_cast<CLabelUI*>(m_pm.FindControl(L"label_result_statics"));
	if (m_pStatiscResultLabel) {
		std::stringstream ss;
		ss << "Patient ";
		ss << GetPatienCount();
		ss << " , ";
		ss << "Study ";
		ss << GetStudyCount();
		ss << " , ";
		ss << "Series ";
		ss << GetSeriesCount();
		ss << " , ";
		ss << "Success's Series ";
		ss << index;
		ss << " ";
		std::string s = ss.str();
		std::wstring ws_result = toWString(s);
		m_pStatiscResultLabel->SetText(ws_result.c_str());
	}
}

void DcmtkDLDicomDemoFrameWnd::UpdateDownloadListProItem(int index, std::string status_text)
{
	if (m_listPro) {
		std::wstring ws = toWString(status_text);
		int list_count = m_listPro->GetCount();
		if (index < list_count && index >= 0) {
			CListTextElementUI* pListElement = (CListTextElementUI*)(m_listPro->GetItemAt(index));
			if (pListElement) {
				pListElement->SetText(4, ws.c_str());
			}
		}		
	}
}

void DcmtkDLDicomDemoFrameWnd::UpdateDownloadListProAll()
{
	CDuiString str;
	if (!m_listPro) {
		return;
	}
	m_listPro->RemoveAll();

	int number_index = 0;
	for (int i = 0; i < m_patient_infos1.size() ; i++) {
		PatientInfo patient_info = m_patient_infos1[i];
		for (int j = 0; j < patient_info.sereis_infos.size() ; j++) {
			
			CListTextElementUI* pListElement = new CListTextElementUI;
			if (pListElement) {
				pListElement->SetTag(number_index);
				m_listPro->Add(pListElement);
	
				std::wstring ws = L"";
				str.Format(_T("%d"), number_index);
				pListElement->SetText(0, str);

				ws = toWString(patient_info.patiend_id);
				pListElement->SetText(1, ws.c_str());

				ws = toWString(patient_info.study_id);
				pListElement->SetText(2, ws.c_str());

				ws = toWString(patient_info.sereis_infos[j].series_id);
				pListElement->SetText(3, ws.c_str());

				ws = toWString(patient_info.sereis_infos[j].download_status);
				pListElement->SetText(4, ws.c_str());

				number_index++;
			}
		}
	}
}
int DcmtkDLDicomDemoFrameWnd::GetPatienCount()
{
	std::set<std::string> patient_ids;
	for (auto patient_info : m_patient_infos1) {
		patient_ids.insert(patient_info.patiend_id);
	}

	return patient_ids.size();
}

int DcmtkDLDicomDemoFrameWnd::GetStudyCount()
{
	std::set<std::string> study_ids;
	for (auto patient_info : m_patient_infos1) {
		study_ids.insert(patient_info.study_id);
	}

	return study_ids.size();
}

int DcmtkDLDicomDemoFrameWnd::GetSeriesCount()
{
	int count = 0;
	for (auto patient_info : m_patient_infos1) {
		count += patient_info.sereis_infos.size();
	}
	return count;
}

bool DcmtkDLDicomDemoFrameWnd::CheckedMatchConditions(GIL::DICOM::DicomDataset& data)
{
	// 过滤时间 ：放在了开始搜索时，进行过滤
	
	// 过滤层厚
	std::string value = "";					
	if (m_pFilterThickness && m_pFilterThickness->IsSelected() && data.getTag(GKDCM_SliceThickness, value)) {
		if (value.size() > 0 && m_filter_thickness.size() > 0) {
			if (value != m_filter_thickness) {
				return false;
			}
		}
	}
	// 过滤部位
	value = "";					
	if (m_pFilterBodyPart && m_pFilterBodyPart->IsSelected() && data.getTag(GKDCM_BodyPartExamined, value)) {
		if (value.size() > 0 && m_bodyPartExamined.size() > 0) {
			if (value != m_bodyPartExamined) {
				return false;
			}
		}
	}
	// 过滤设备
	value = "";					
	if (m_pFilterModality && m_pFilterModality->IsSelected() && data.getTag(GKDCM_Modality, value)) {
		if (value.size() > 0 && m_filter_modality.size() > 0) {
			if (value != m_filter_modality) {
				return false;
			}
		}
	}

	return true;
}
void DcmtkDLDicomDemoFrameWnd::DoDownloadTest()
{
	// 下载series计数器，清零
	m_downloading_dicom_index = 0;
	UpdateDownloadStaticsText(0);

	// 获取最新的控件值，主要是获取Dicom保存路径
	GetAllControlValue();

	for (auto patient_info : m_patient_infos1) {
		for (auto sereis_info : patient_info.sereis_infos) {
			// 创建seriesid的文件夹
			std::string series_path = "";
			series_path += m_dicom_saved_path + "\\" + patient_info.patiend_id + "\\";
			series_path += patient_info.study_id + "\\";
			series_path += sereis_info.series_id + "\\" ;
			TryCreateDir(series_path);
		}
	}

	for (auto& patient_info : m_patient_infos1) {
		for (auto& series_info : patient_info.sereis_infos) {
			if (m_is_stoped) {
				//更新list控件中，对应的series的状态为stopped
				series_info.download_status = DOWNLOAD_STATUS_STOPPED;
				SendMessage(WM_USER_UPDATE_DOWNLOAD_STATUS, m_downloading_dicom_index, (LPARAM)DOWNLOAD_STATUS_STOPPED);
				m_downloading_dicom_index++;
				continue; 
			}
			GIL::DICOM::DicomDataset base;
			GIL::DICOM::PACSController::Instance()->InitFindQueryWrapper(base);
			GIL::DICOM::PACSController::Instance()->SetWrapper(base, GKDCM_QueryRetrieveLevel, "SERIES");
			GIL::DICOM::PACSController::Instance()->SetWrapper(base, GKDCM_StudyInstanceUID, patient_info.study_id);
			GIL::DICOM::PACSController::Instance()->SetWrapper(base, GKDCM_PatientID, patient_info.patiend_id);
			GIL::DICOM::PACSController::Instance()->SetWrapper(base, GKDCM_SeriesInstanceUID, series_info.series_id);
			GIL::DICOM::PACSController::Instance()->SetWrapper(base, GKDCM_Modality, series_info.modality);

			std::string patient_path = m_dicom_saved_path + "\\" + patient_info.patiend_id + "\\";
			std::string study_path = patient_path + patient_info.study_id + "\\";
			std::string series_path = study_path + series_info.series_id + "\\" ;
			//更新list控件中，对应的series的状态为downloading
			SendMessage(WM_USER_UPDATE_DOWNLOAD_STATUS, m_downloading_dicom_index, (LPARAM)DOWNLOAD_STATUS_DOWNLOADING);

			if (GIL::DICOM::PACSController::Instance()->DownloadDicomFilesBySeries(this, base, series_path)) {
				series_info.download_status = DOWNLOAD_STATUS_SUCCESS;
				SendMessage(WM_USER_UPDATE_DOWNLOAD_DICOM_FILE, 0, m_downloading_dicom_index + 1);
				//更新list控件中，对应的series的状态为success
				SendMessage(WM_USER_UPDATE_DOWNLOAD_STATUS, m_downloading_dicom_index, (LPARAM)DOWNLOAD_STATUS_SUCCESS);
			}
			else
			{
				series_info.download_status = DOWNLOAD_STATUS_FAILURE;
				//更新list控件中，对应的series的状态为failure
				SendMessage(WM_USER_UPDATE_DOWNLOAD_STATUS, m_downloading_dicom_index, (LPARAM)DOWNLOAD_STATUS_FAILURE);
			}
			m_downloading_dicom_index++;
		}
	}
	//保存下载结果到文件
	OutputResultStaticsToFile(m_dicom_saved_path);
}

void DcmtkDLDicomDemoFrameWnd::SetDownloadStop(bool is_stopped)
{
	m_is_stoped = is_stopped;
}

void DcmtkDLDicomDemoFrameWnd::OutputResultStaticsToFile(std::string path)
{
	// 格式化结果文件名
	SYSTEMTIME st;
	GetLocalTime(&st);
	std::stringstream filename;
	filename << m_dicom_saved_path;
	filename << "\\result";
	filename << "_";
	filename << st.wYear;
	filename << std::setw(2) << std::setfill('0') << st.wMonth;
	filename << std::setw(2) << std::setfill('0') << st.wDay;
	filename << "_";
	filename << std::setw(2) << std::setfill('0') << st.wHour;
	filename << std::setw(2) << std::setfill('0') << st.wMinute;
	filename << std::setw(2) << std::setfill('0') << st.wSecond;
	filename << ".csv";

	TryCreateDir(m_dicom_saved_path);

	std::ofstream os_file;
	os_file.open(filename.str(), ios::out);

	//  插入表头
	os_file << "patiend_id" << ',' ;
	os_file << "study_id" << ',' ;
	os_file <<"series_id" << ',' ;
	os_file << "status" << ',' ;
	os_file << endl;

	// 写入 能够根据patiendid 找到 seriesid的数据
	for (auto patient_info : m_patient_infos1) {
		for (auto series_info : patient_info.sereis_infos) {
			os_file << patient_info.patiend_id << ',' ;
			os_file << patient_info.study_id << ',' ;
			os_file << series_info.series_id << ',' ;
			os_file << series_info.download_status << ',' ;
			os_file << endl;
		}
	}
	// 写入 不能够根据patiendid 找到 seriesid的数据
	for (auto patient_id : m_patient_infos2) {
		os_file << patient_id << ',' ;
		os_file << "" << ',' ;
		os_file << "" << ',' ;
		os_file << "not found" << ',' ;
		os_file << endl;
	}
	os_file.close();
}

std::vector<std::string> DcmtkDLDicomDemoFrameWnd::testSplit(std::string srcStr, const std::string& delim)
{
	int nPos = 0;
	std::vector<std::string> vec;
	nPos = srcStr.find(delim.c_str());
	while(-1 != nPos)
	{
		string temp = srcStr.substr(0, nPos);
		if (temp.size() > 0) {
			vec.push_back(temp);
		}
		srcStr = srcStr.substr(nPos+1);
		nPos = srcStr.find(delim.c_str());
	}
	if (srcStr.size() > 0) {
		vec.push_back(srcStr);
	}
	return vec;
}

void DcmtkDLDicomDemoFrameWnd::GetAllControlValue()
{
	std::wstring ws_body_part = m_pBodyPartEdit->GetText().GetData();
	std::wstring ws_thickness = m_pThicknessEdit->GetText().GetData();
	std::wstring ws_modallity = m_pMOdalityiesInStudyEdit->GetText().GetData();
	std::wstring ws_save_path = m_pDownloadPathEdit->GetText().GetData();

	m_bodyPartExamined	= toString(ws_body_part);
	m_filter_thickness	= toString(ws_thickness);
	m_filter_modality	= toString(ws_modallity);
	m_dicom_saved_path  = toString(ws_save_path);
}
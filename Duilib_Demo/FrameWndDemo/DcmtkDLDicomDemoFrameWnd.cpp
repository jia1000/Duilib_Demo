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

#include <dcmtk/dcmdata/dcdeftag.h>

#include <thread>

#define LOCAL_AE_TITLE	"DEEPWISE_002"

#define SCP_IDENTIFIER   "253"
#define AET_TITLE		"DEEPWISESCP"
#define HOST_ADDR		"192.168.1.253"
#define AET_PORT	22222
#define PSDU_LENGTH 16384

DcmtkDLDicomDemoFrameWnd::DcmtkDLDicomDemoFrameWnd(void)
	//: server(new DicomServer(SCP_IDENTIFIER, AET_TITLE, HOST_ADDR, AET_PORT, PSDU_LENGTH))
	: m_downloading_dicom_index(0)
	, m_is_stoped(false)
{
	//retrieve_method = GET;
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
	return NULL;
}

void DcmtkDLDicomDemoFrameWnd::InitWindow()
{
	m_pPatientCsvPathEdit = static_cast<CEditUI*>(m_pm.FindControl(L"edit_patient_csv_path"));
	m_pPatientIdEdit = static_cast<CEditUI*>(m_pm.FindControl(L"edit_find"));
	m_pResearchResultLabel = static_cast<CEditUI*>(m_pm.FindControl(L"edit_research_result"));
	m_pBodyPartEdit = static_cast<CEditUI*>(m_pm.FindControl(L"edit_filter_part"));
	m_pThicknessEdit = static_cast<CEditUI*>(m_pm.FindControl(L"edit_filter_thickness"));
	m_pMOdalityiesInStudyEdit = static_cast<CEditUI*>(m_pm.FindControl(L"edit_filter_modality"));
	m_pDownloadPathEdit = static_cast<CEditUI*>(m_pm.FindControl(L"edit_download_path"));
	//m_pPatientIdEdit->SetText(L"1007733445,0060388,0170713,0171033");// 2个ct 2个dx
	m_pPatientIdEdit->SetText(L"1008621671,0170952,0003852666");// 1个ct

	if (m_pDownloadPathEdit) {
		m_pDownloadPathEdit->SetText(L"G:\\temp1");
		m_dicom_saved_path = "G:\\temp1";
	}
	UpdateDownloadStaticsText();
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
			//DoDownloadTest();
			m_is_stoped = false;
			std::thread th(&DcmtkDLDicomDemoFrameWnd::DoDownloadTest, this);
			th.detach();
		} else if (_tcscmp(pszCtrlName, _T("btn_patient_csv_path")) == 0) {
			OnOpenPatientIDListFile();
		} else if (_tcscmp(pszCtrlName, _T("btn_download_path")) == 0) {
			OnOpenDownloadPath(); 
		}    
	} else if (_tcsicmp(msg.sType, _T("selectchanged")) == 0) {
		OnSelChanged(msg.pSender);
	} 
}

LRESULT DcmtkDLDicomDemoFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT DcmtkDLDicomDemoFrameWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (uMsg)
	{
	case WM_USER_UPDATE_DOWNLOAD_DICOM_FILE:
		UpdateDownloadStaticsText();
		break;
	default:
		break;
	}
	bHandled = FALSE;
	return 0;
}

//#include"dcmtk\config\osconfig.h"
//#include"dcmtk\dcmdata\dctk.h"
//
//#include "utility_tool/string_converse.h"

void DcmtkDLDicomDemoFrameWnd::OnSelChanged(CControlUI* pSender)
{
	CTabLayoutUI* pTabResultShow = static_cast <CTabLayoutUI*>(m_pm.FindControl(_T("tab_result_show")));
	if (pTabResultShow == NULL)
	{
		return;
	}
	CDuiString strSelName = pSender->GetName();
	if (strSelName == _T("op_success")) 
	{		
		pTabResultShow->SelectItem(0);
	} else if (strSelName == _T("op_failure")) {
		pTabResultShow->SelectItem(1);
	}
}

void DcmtkDLDicomDemoFrameWnd::OnOpenPatientIDListFile()
{
	std::wstring def_path = L"";
	OPENFILENAME ofn = { 0 };  
    TCHAR str_filename[MAX_PATH] = { 0 };
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = m_hWnd;
    ofn.lpstrFilter = TEXT("All Files(*.*)\0*.*\0CSV Flies(*.csv)\0*.csv\0\0");
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
			// 在edit框上，显示patient id列表
			std::ifstream inFile(str_filename, ios::in);
			std::string line;
			//std::vector<std::wstring> strArray;
			std::wstring ws_patient_ids_edit(L"");
			while (getline(inFile, line)) {
				std::stringstream ss(line);
				std::string word;
				while (getline(ss, word, ','))
				{
					std::wstring ws_word = toWString(word);
					//strArray.push_back(ws_word);
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
			//GIL::DICOM::PACSController::Instance()->SetDicomSavedPath(path);
		}
	} 
}
void DcmtkDLDicomDemoFrameWnd::DoSearchStudyTest()
{
	// 搜索前，先清空待下载的病历列表
	m_study_ids.clear();
	m_patient_ids.clear();

	m_patient_infos2.clear();

	std::wstring ws_patient_ids = m_pPatientIdEdit->GetText().GetData();	

	GIL::DICOM::DicomDataset queryWrapper;
	GIL::DICOM::PACSController::Instance()->InitFindQueryWrapper(queryWrapper);
	GIL::DICOM::PACSController::Instance()->SetWrapper(queryWrapper, GKDCM_QueryRetrieveLevel, "STUDY");

	std::vector<std::string> patient_ids = testSplit(toString(ws_patient_ids), ",");

	std::string result = "";
	for (int patient_index = 0; patient_index < patient_ids.size() ; patient_index++) {
		std::string patient_id = patient_ids[patient_index];
		GIL::DICOM::PACSController::Instance()->SetWrapper(queryWrapper, GKDCM_PatientID, patient_id);
		std::list< GNC::GCS::Ptr<GIL::DICOM::DicomDataset> > resultsWrapper;
		bool ret_status_obtain = GIL::DICOM::PACSController::Instance()->ObtainDicomDataSet(this, SCP_IDENTIFIER, queryWrapper, resultsWrapper, false);

		if (ret_status_obtain) {
			
			for (auto iter : resultsWrapper) {
				GNC::GCS::Ptr<GIL::DICOM::DicomDataset> item = iter;

				std::string patient_name("");					
				if (item->getTag(GKDCM_PatientName, patient_name)) {
					result += patient_name;
					result += "   ";
				}

				std::string number_study_instances("");					
				if (item->getTag(GKDCM_NumberOfStudyRelatedInstances, number_study_instances)) {
					result += number_study_instances;
					result += "   ";
					result += "\r\n";
				}

				std::string study_id("");					
				if (item->getTag(GKDCM_StudyInstanceUID, study_id)) {
					result += study_id;
					result += "   ";
					result += "\r\n";
					m_study_ids.push_back(study_id);
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
	if (m_pResearchResultLabel) {
		std::wstring ws_result = toWString(result);
		m_pResearchResultLabel->SetText(ws_result.c_str());
	}

	DoSearchSeriesTest();
}
void DcmtkDLDicomDemoFrameWnd::DoSearchSeriesTest()
{
	m_patient_infos1.clear();
	m_downloading_dicom_index = 0;

	std::wstring ws_body_part = m_pBodyPartEdit->GetText().GetData();
	std::wstring ws_thickness = m_pThicknessEdit->GetText().GetData();
	std::wstring ws_modallity = m_pMOdalityiesInStudyEdit->GetText().GetData();

	m_bodyPartExamined	= toString(ws_body_part);
	m_filter_thickness	= toString(ws_thickness);
	m_filter_modality	= toString(ws_modallity);


	//GIL::DICOM::PACSController::Instance()->SetWrapper(queryWrapper, GKDCM_BodyPartExamined, m_bodyPartExamined);
	//GIL::DICOM::PACSController::Instance()->SetWrapper(queryWrapper, GKDCM_ModalitiesInStudy, m_filter_modality);
	//GIL::DICOM::PACSController::Instance()->SetWrapper(queryWrapper, GKDCM_SliceThickness, m_filter_thickness);

	GIL::DICOM::PACSController::Instance()->SetThickness(m_filter_thickness);
	GIL::DICOM::PACSController::Instance()->SetBodyPartExamined(m_bodyPartExamined);

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
		bool ret_status_obtain = GIL::DICOM::PACSController::Instance()->ObtainDicomDataSet(this, SCP_IDENTIFIER, base, 
			resultsWrapper, false);

		if (ret_status_obtain) {
			PatientInfo patient_info;
			//std::vector<std::string> sereis_ids;
			for (auto iter : resultsWrapper) {
				GNC::GCS::Ptr<GIL::DICOM::DicomDataset> item = iter;
				std::string series_id("");					
				if (item->getTag(GKDCM_SeriesInstanceUID, series_id)) {
					bool match_condition = CheckedMatchConditions(*item);
					if (match_condition) {
						std::string series_modality("");					
						if (item->getTag(GKDCM_Modality, series_modality)) {
							Series_Info series_info;
							series_info.series_id = series_id;
							series_info.modality = series_modality;	
							series_info.is_downloaded = false;
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

	UpdateDownloadStaticsText();
}

void DcmtkDLDicomDemoFrameWnd::UpdateDownloadStaticsText()
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
		ss << m_downloading_dicom_index;
		ss << " ";
		std::string s = ss.str();
		std::wstring ws_result = toWString(s);
		m_pStatiscResultLabel->SetText(ws_result.c_str());
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
	// 过滤时间
	// 过滤层厚
	std::string value = "";					
	if (data.getTag(GKDCM_SliceThickness, value)) {
		if (value.size() > 0 && m_filter_thickness.size() > 0) {
			if (value != m_filter_thickness) {
				return false;
			}
		}
	}
	// 过滤部位
	value = "";					
	if (data.getTag(GKDCM_BodyPartExamined, value)) {
		if (value.size() > 0 && m_bodyPartExamined.size() > 0) {
			if (value != m_bodyPartExamined) {
				return false;
			}
		}
	}
	// 过滤设备
	value = "";					
	if (data.getTag(GKDCM_Modality, value)) {
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
	UpdateDownloadStaticsText();

	for (auto patient_info : m_patient_infos1) {
		// 创建患者编号的文件夹
		std::string patient_path = m_dicom_saved_path + "\\" + patient_info.patiend_id + "\\";
		TryCreateDir(patient_path);
		// 创建studyid的文件夹
		std::string study_path = patient_path + patient_info.study_id + "\\";
		TryCreateDir(study_path);
		for (auto sereis_info : patient_info.sereis_infos) {
			// 创建seriesid的文件夹
			std::string series_path = study_path + sereis_info.series_id + "\\" ;
			TryCreateDir(series_path);
		}
	}

	for (auto& patient_info : m_patient_infos1) {
		if (m_is_stoped) {
			break;
		}
		for (auto& series_info : patient_info.sereis_infos) {
			if (m_is_stoped) {
				break;
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
			if (GIL::DICOM::PACSController::Instance()->DownloadDicomFilesBySeries(this, SCP_IDENTIFIER, base, series_path)) {
				m_downloading_dicom_index++;
				series_info.is_downloaded = true;
				//UpdateDownloadStaticsText();
				SendMessage(WM_USER_UPDATE_DOWNLOAD_DICOM_FILE, 0, m_downloading_dicom_index);
			}
			else
			{
				series_info.is_downloaded = false;
			}
		}
	}

	OutputResultStaticsToFile(m_dicom_saved_path);
}

void DcmtkDLDicomDemoFrameWnd::OutputResultStaticsToFile(std::string path)
{
	std::string file_name= m_dicom_saved_path + "\\" + "result.csv";
	std::ofstream os_file;
	os_file.open(file_name, ios::out);

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
			if (series_info.is_downloaded) {
				os_file << "success" << ',' ;
			} else {
				os_file << "failure" << ',' ;
			}
			os_file << endl;
		}
	}
	// 写入 能够根据patiendid 找到 seriesid的数据
	for (auto patient_id : m_patient_infos2) {
		os_file << patient_id << ',' ;
		os_file << "" << ',' ;
		os_file << "" << ',' ;
		os_file << "not found" << ',' ;
		os_file << endl;
	}
	os_file.close();
}

void DcmtkDLDicomDemoFrameWnd::DoDownloadTest2()
{
	auto iter = m_patient_ids.begin();
	auto item = m_study_ids.begin();

	for (; item != m_study_ids.end() && iter != m_patient_ids.end(); ++item, ++iter) {		
		GIL::DICOM::DicomDataset base;
		//base.tags[GKDCM_QueryRetrieveLevel] = "STUDY";	//"0008|0052"
		//base.tags[GKDCM_StudyInstanceUID] = item;		//"0020|000d"
		GIL::DICOM::PACSController::Instance()->InitFindQueryWrapper(base);
		GIL::DICOM::PACSController::Instance()->SetWrapper(base, GKDCM_StudyInstanceUID, *item);
		GIL::DICOM::PACSController::Instance()->SetWrapper(base, GKDCM_PatientID, *iter);

		GIL::DICOM::PACSController::Instance()->ObtenerEstudio(this, SCP_IDENTIFIER, base, false);
	}
}
DcmElement* DcmtkDLDicomDemoFrameWnd::CrearElementoConValor(const char* s)
{
	unsigned int g = 0xffff;
	unsigned int e = 0xffff;
	int n = 0;
	char* val = new char[strlen(s)+1];
	OFString dicName, valStr;
	OFString msg;

	val[0] = '\0';

	// try to parse group and element number
	n = sscanf(s, "%x|%x=%s", &g, &e, val);
	OFString toParse = s;
	size_t eqPos = toParse.find('=');
	if (n < 2) // if at least no tag could be parsed
	{
		// if value is given, extract it (and extrect dictname)
		if (eqPos != OFString_npos) {
			dicName = toParse.substr(0, eqPos).c_str();
			valStr = toParse.substr(eqPos + 1, toParse.length());
		} else // no value given, just dictionary name
			dicName = s; // only dictionary name given (without value)
		// try to lookup in dictionary
		DcmTagKey key(0xffff, 0xffff);
		const DcmDataDictionary& globalDataDict = dcmDataDict.rdlock();
		const DcmDictEntry *dicent = globalDataDict.findEntry(dicName.c_str());
		dcmDataDict.unlock();
		if (dicent != NULL) {
			// found dictionary name, copy group and element number
			key = dicent->getKey();
			g = key.getGroup();
			e = key.getElement();
		} else {
			// not found in dictionary
			std::cerr <<  "bad key format or dictionary name not found in dictionary: " << dicName << std::endl;
			delete[] val;
			return NULL;
		}
	}// tag could be parsed, copy value if it exists
	else {
		if (eqPos != OFString_npos) {
			valStr = toParse.substr(eqPos + 1, toParse.length());
		}
	}
	DcmTag tag(g, e);
	if (tag.error() != EC_Normal) {
		std::cerr << "Tag desconocido: (" <<
			std::hex << std::setw(4) << std::setfill('0') << g << "|" <<
			std::hex << std::setw(4) << std::setfill('0') << e << ")" << std::endl;
		delete[] val;
		return NULL;
	}
	DcmElement *elem = newDicomElement(tag);
	if (elem == NULL) {
		std::cerr << "No se pudo crear el elemento para el tag: (" <<
			std::hex << std::setw(4) << std::setfill('0') << g << "|" <<
			std::hex << std::setw(4) << std::setfill('0') << e << ")" << std::endl;
		delete[] val;
		return NULL;
	}
	if (valStr.length() > 0) {
		if (elem->putString(valStr.c_str()).bad()) {
			std::cerr << "No se pudo asignar el valor al elemento: (" <<
				std::hex << std::setw(4) << std::setfill('0') << g << "|" <<
				std::hex << std::setw(4) << std::setfill('0') << e << ")=" << valStr.c_str() << std::endl;
			delete elem;
			delete[] val;
			return NULL;
		}
	}
	delete[] val;
	return elem;

}

std::vector<std::string> DcmtkDLDicomDemoFrameWnd::testSplit(std::string srcStr, const std::string& delim)
{
	int nPos = 0;
	std::vector<std::string> vec;
	nPos = srcStr.find(delim.c_str());
	while(-1 != nPos)
	{
		string temp = srcStr.substr(0, nPos);
		vec.push_back(temp);
		srcStr = srcStr.substr(nPos+1);
		nPos = srcStr.find(delim.c_str());
	}
	vec.push_back(srcStr);
	return vec;
}

//void DcmtkDLDicomDemoFrameWnd::GetConnection(void* connectionKey)
//{
//	GIL::DICOM::DCMTK::Network::Instance(connectionKey);
//}
//
//void DcmtkDLDicomDemoFrameWnd::ReleaseConnection(void* connectionKey)
//{
//	GIL::DICOM::DCMTK::Network::FreeInstance(connectionKey);
//}


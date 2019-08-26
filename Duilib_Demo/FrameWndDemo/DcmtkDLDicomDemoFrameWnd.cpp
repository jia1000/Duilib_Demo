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

#define LOCAL_AE_TITLE	"DEEPWISE_002"

#define SCP_IDENTIFIER   "253"
#define AET_TITLE		"DEEPWISESCP"
#define HOST_ADDR		"192.168.1.253"
#define AET_PORT	22222
#define PSDU_LENGTH 16384

DcmtkDLDicomDemoFrameWnd::DcmtkDLDicomDemoFrameWnd(void)
	//: server(new DicomServer(SCP_IDENTIFIER, AET_TITLE, HOST_ADDR, AET_PORT, PSDU_LENGTH))
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
	m_pFindResultLabel = static_cast<CEditUI*>(m_pm.FindControl(L"edit_research_result"));
	m_pBodyPartEdit = static_cast<CEditUI*>(m_pm.FindControl(L"edit_filter_part"));
	m_pThicknessEdit = static_cast<CEditUI*>(m_pm.FindControl(L"edit_filter_thickness"));
	m_pMOdalityiesInStudyEdit = static_cast<CEditUI*>(m_pm.FindControl(L"edit_filter_modality"));
	m_pDownloadPathEdit = static_cast<CEditUI*>(m_pm.FindControl(L"edit_download_path"));
	//m_pPatientIdEdit->SetText(L"1007733445,0060388,0170713,0171033");// 2个ct 2个dx
	m_pPatientIdEdit->SetText(L"1008621671,0170952,0003852666");// 1个ct

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
			DoSearchTest();
		} else if (_tcscmp(pszCtrlName, _T("btn_download")) == 0) {
			DoDownloadTest();
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

			std::string path = toString(sz_buffer);
			GIL::DICOM::PACSController::Instance()->SetDicomSavedPath(path);
		}
	} 
}
void DcmtkDLDicomDemoFrameWnd::DoSearchTest()
{
	// 搜索前，先清空待下载的病历列表
	m_study_ids.clear();

	m_patient_ids.clear();

	//GNC::GCS::StoredQuery pStoredQuery;

	std::wstring ws_patient_ids = m_pPatientIdEdit->GetText().GetData();
	std::wstring ws_body_part = m_pBodyPartEdit->GetText().GetData();
	std::wstring ws_thickness = m_pThicknessEdit->GetText().GetData();
	std::wstring ws_modallity = m_pMOdalityiesInStudyEdit->GetText().GetData();


	//std::string patient_ids	= toString(ws_patient_ids);
	std::string body_part	= toString(ws_body_part);
	std::string thickness	= toString(ws_thickness);
	std::string modallity	= toString(ws_modallity);

	GIL::DICOM::PACSController::Instance()->SetThickness(thickness);//atoi(thickness.c_str()));
	GIL::DICOM::PACSController::Instance()->SetBodyPartExamined(body_part);

	GIL::DICOM::DicomDataset queryWrapper;
	//queryWrapper.tags[GKDCM_QueryRetrieveLevel] = "STUDY";
	//queryWrapper.tags[GKDCM_PatientName] = "";
	//queryWrapper.tags[GKDCM_PatientID] = "";
	//queryWrapper.tags[GKDCM_StudyDate] = "";
	//queryWrapper.tags[GKDCM_NumberOfStudyRelatedInstances] = ""; // 影像数量
	//queryWrapper.tags[GKDCM_ModalitiesInStudy] = "";
	//queryWrapper.tags[GKDCM_StudyInstanceUID] = "";
	//queryWrapper.tags[GKDCM_BodyPartExamined] = body_part;//CHEST  BREAST
	//queryWrapper.tags[GKDCM_ModalitiesInStudy] = modallity;//CT MG DX  etc.
	//queryWrapper.tags[GKDCM_SliceThickness] = thickness;//CT MG DX  etc.

	GIL::DICOM::PACSController::Instance()->InitStudyFindQueryWrapper(queryWrapper);
	GIL::DICOM::PACSController::Instance()->SetWrapper(queryWrapper, GKDCM_BodyPartExamined, body_part);
	GIL::DICOM::PACSController::Instance()->SetWrapper(queryWrapper, GKDCM_ModalitiesInStudy, modallity);
	GIL::DICOM::PACSController::Instance()->SetWrapper(queryWrapper, GKDCM_SliceThickness, thickness);


	//query.addConditionIfNotExists(GKDCM_QueryRetrieveLevel, "STUDY");
	//query.addConditionIfNotExists(GKDCM_PatientName);
	//query.addConditionIfNotExists(GKDCM_PatientID);
	//query.addConditionIfNotExists(GKDCM_ModalitiesInStudy);
	//query.addConditionIfNotExists(GKDCM_PatientBirthDate);
	//query.addConditionIfNotExists(GKDCM_PatientSex);
	//query.addConditionIfNotExists(GKDCM_StudyDate);
	//query.addConditionIfNotExists(GKDCM_StudyTime);
	//query.addConditionIfNotExists(GKDCM_StudyID);
	//query.addConditionIfNotExists(GKDCM_StudyInstanceUID);
	//query.addConditionIfNotExists(GKDCM_StudyDescription);
	//query.addConditionIfNotExists(GKDCM_AccessionNumber);
	//query.addConditionIfNotExists(GKDCM_StationName);
	//query.addConditionIfNotExists(GKDCM_ReferringPhysicianName);
	//query.addConditionIfNotExists(GKDCM_NumberOfStudyRelatedInstances);

	bool success = true;
	std::ostringstream errorMsg;
	std::string errorTitle;

	std::string aet_title	= AET_TITLE;//"DEEPWISESCP";
	std::string host_addr	= HOST_ADDR;
	//std::string port		= AET_PORT;//"22222";
	std::string aet_local	= LOCAL_AE_TITLE;
	int psdu_length			= PSDU_LENGTH;//16384;

	DcmDataset query;
	GIL::DICOM::PACSController::Instance()->FillInQuery(queryWrapper, &query);

	FindAssociation fs("C-FIND");
	fs.SetAbstractSyntax("1.2.840.10008.5.1.4.1.2.2.1");
	fs.SetMaxResults(500);

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
		pNetwork->InitializeNetwork(fs.GetTimeout());

		fs.Create(aet_title
			, host_addr
			, AET_PORT//atoi(port.c_str())
			, aet_local
			);

		std::list< GNC::GCS::Ptr<GIL::DICOM::DicomDataset> > resultsWrapper;
		//DicomServer server;
		fs.SetCallbackInfo(&resultsWrapper);

		CONDITION r = fs.Connect(pNetwork, 16384);

		CONDITION c = ECC_Normal;

		if (r.good() == true) {
			//LOG_DEBUG(ambitolog, "Requesting object:" << std::endl << DumpDataset(query));
			c = fs.SendObject(&query);
		}
		else {
			//LOG_DEBUG(ambitolog, "Error connecting:" << r.text());
			fs.Drop();
			fs.Destroy();
			//LOG_INFO(ambitolog, "Disconnected");
			//throw GIL::DICOM::PACSException(r.text());
		}

		if (!c.good()) {
			//LOG_DEBUG(ambitolog, "Error requesting object: " << c.text());
			fs.Drop();
			fs.Destroy();
			//LOG_INFO(ambitolog, "Disconnected");
			//throw GIL::DICOM::PACSException(c.text());
		}
		else
		{
			std::string result("");
			std::vector<std::string> patient_ids = testSplit(toString(ws_patient_ids), ",");
			//std::list< GNC::GCS::Ptr<GIL::DICOM::DicomDataset> > resultsWrapper;
			//int count = 0;
			for (auto iter : resultsWrapper) {
				GNC::GCS::Ptr<GIL::DICOM::DicomDataset> item = iter;
				std::string patient_id("");
				
				bool ret = item->getTag(GKDCM_PatientID, patient_id);
				auto iter = std::find(patient_ids.begin(), patient_ids.end(), patient_id);
				if (ret && iter != patient_ids.end()) {

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
					//if (count > 10) {
					//	break;
					//}
					//count++;
				}
			}
			if (m_pFindResultLabel) {
				std::wstring ws_result = toWString(result);
				m_pFindResultLabel->SetText(ws_result.c_str());
			}
		}

		//LOG_INFO(ambitolog, "Disconnected");
		fs.Drop();
	}
}

void DcmtkDLDicomDemoFrameWnd::DoDownloadTest()
{
	auto iter = m_patient_ids.begin();
	for (auto item : m_study_ids) {
		if (iter != m_patient_ids.end()) {
			//m_cur_patient_id = *iter;
			GIL::DICOM::PACSController::Instance()->SetCurPatientId(*iter);
		}
		iter++;
		GIL::DICOM::DicomDataset base;
		//base.tags[GKDCM_QueryRetrieveLevel] = "STUDY";	//"0008|0052"
		//base.tags[GKDCM_StudyInstanceUID] = item;		//"0020|000d"
		GIL::DICOM::PACSController::Instance()->InitStudyFindQueryWrapper(base);
		GIL::DICOM::PACSController::Instance()->SetWrapper(base, GKDCM_StudyInstanceUID, item);
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


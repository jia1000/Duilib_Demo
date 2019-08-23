#include "DcmtkDLDicomDemoFrameWnd.h"

#include "DcmtkDLDicomSetDemoFrameWnd.h"

#include "api/dicom/dcmdictionary.h"
#include "api/dicom/dicomdataset.h"

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
	: server(new DicomServer(SCP_IDENTIFIER, AET_TITLE, HOST_ADDR, AET_PORT, PSDU_LENGTH))
{
	retrieve_method = GET;
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
	m_pPatientIdEdit = static_cast<CEditUI*>(m_pm.FindControl(L"edit_find"));
	m_pFindResultLabel = static_cast<CEditUI*>(m_pm.FindControl(L"edit_research_result"));
	m_pBodyPartEdit = static_cast<CEditUI*>(m_pm.FindControl(L"edit_filter_part"));
	m_pThicknessEdit = static_cast<CEditUI*>(m_pm.FindControl(L"edit_filter_thickness"));
	m_pMOdalityiesInStudyEdit = static_cast<CEditUI*>(m_pm.FindControl(L"edit_filter_modality"));
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

	m_filter_thickness = atoi(thickness.c_str());
	m_bodyPartExamined = body_part;

	GIL::DICOM::DicomDataset queryWrapper;
	queryWrapper.tags[GKDCM_QueryRetrieveLevel] = "STUDY";
	queryWrapper.tags[GKDCM_PatientName] = "";
	queryWrapper.tags[GKDCM_PatientID] = "";
	queryWrapper.tags[GKDCM_StudyDate] = "";
	queryWrapper.tags[GKDCM_NumberOfStudyRelatedInstances] = ""; // 影像数量
	queryWrapper.tags[GKDCM_ModalitiesInStudy] = "";
	queryWrapper.tags[GKDCM_StudyInstanceUID] = "";
	queryWrapper.tags[GKDCM_BodyPartExamined] = body_part;//CHEST  BREAST
	queryWrapper.tags[GKDCM_ModalitiesInStudy] = modallity;//CT MG DX  etc.
	queryWrapper.tags[GKDCM_SliceThickness] = thickness;//CT MG DX  etc.




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
	FillInQuery(queryWrapper, &query);

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
			m_cur_patient_id = *iter;
		}
		iter++;
		GIL::DICOM::DicomDataset base;
		base.tags[GKDCM_QueryRetrieveLevel] = "STUDY";	//"0008|0052"
		base.tags[GKDCM_StudyInstanceUID] = item;		//"0020|000d"
		ObtenerEstudio(this, SCP_IDENTIFIER, base, false);
	}
}
void DcmtkDLDicomDemoFrameWnd::FillInQuery(const GIL::DICOM::DicomDataset& base, DcmDataset* query)//, const GNC::GCS::Ptr<DicomServer>& server)
{			
	//wxCSConv conv = GetConv(server->GetDefaultCharset());
	OFCondition cond;

	typedef GIL::DICOM::DicomDataset TJerarquia;
	//tags
	for (auto it = base.tags.begin(); it != base.tags.end(); ++it) {
		DcmElement* e = CrearElementoConValor((*it).first.c_str());

		if (e != NULL) {
			const std::string& val = (*it).second;
			if (val.size() > 0) {
				e->putString((*it).second.c_str());//by jia wxString( (*it).second.c_str(), wxConvUTF8).mb_str(conv) );
			}					
			cond = query->insert(e, true, false);

			if (cond.bad()) {
				//LOG_ERROR("DcmtkDLDicomDemoFrameWnd", "No se pudo insertar el elemento: (" << e->getTag().toString().c_str() << "): " << cond.text());
			}
		}
	}
	//sequences
	for (TJerarquia::DatasetList::const_iterator it2 = base.secuencias.begin(); it2 != base.secuencias.end(); ++it2) 
	{
		const TJerarquia& seq = (*it2);
		DcmElement* es = CrearElementoConValor(seq.tagName.c_str());
		if (es != NULL) {

			for (TJerarquia::DatasetList::const_iterator it3 = seq.items.begin(); it3 != seq.items.end(); ++it3) {
				const TJerarquia& item = (*it3);
				DcmItem* di = new DcmItem();

				for (auto it4 = item.tags.begin(); it4 != item.tags.end(); ++it4) {
					DcmElement* ei = CrearElementoConValor((*it4).first.c_str());
					if (ei != NULL) {
						const std::string& val = (*it4).second;
						if (val.size() > 0) {
							ei->putString((*it4).second.c_str());//by jia wxString( (*it4).second.c_str(), wxConvUTF8).mb_str(conv) );
						}					
						cond = di->insert(ei, true, false);

						if (cond.bad()) {
							//LOG_ERROR("DcmtkDLDicomDemoFrameWnd", "No se pudo insertar el elemento: (" << ei->getTag().toString().c_str() << "): " << cond.text());
						}
					}

				}

				query->insertSequenceItem(es->getTag(), di);
			}					
		}
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

void DcmtkDLDicomDemoFrameWnd::GetConnection(void* connectionKey)
{
	GIL::DICOM::DCMTK::Network::Instance(connectionKey);
}

void DcmtkDLDicomDemoFrameWnd::ReleaseConnection(void* connectionKey)
{
	GIL::DICOM::DCMTK::Network::FreeInstance(connectionKey);
}

bool DcmtkDLDicomDemoFrameWnd::ObtenerEstudio(void* connectionKey, const std::string& serverId, const GIL::DICOM::DicomDataset& base, /*IModeloDicom* pModelo, GNC::IProxyNotificadorProgreso* pNotificador,*/ bool link)
{
	bool success = true;
	std::ostringstream errorMsg;
	std::string errorTitle;

	std::string aet_title	= AET_TITLE;//"DEEPWISESCP";
	std::string host_addr	= HOST_ADDR;//"192.168.1.253";
	//std::string port		= "22222";
	std::string aet_local	= LOCAL_AE_TITLE;
	int psdu_length			= 16384;

	DcmDataset query;
	DcmElement* e = NULL;

	e = newDicomElement(DCM_SpecificCharacterSet);
	e->putString("");//server->GetDefaultCharset().c_str());
	query.insert(e);

	e = newDicomElement(DCM_PatientID);
	if (query.insert(e).bad()) {
		delete e;
	}

	e = newDicomElement(DCM_PatientName);
	if (query.insert(e).bad()) {
		delete e;
	}

	e = newDicomElement(DCM_StudyDate);
	if (query.insert(e).bad()) {
		delete e;
	}

	e = newDicomElement(DCM_StudyTime);
	if (query.insert(e).bad()) {
		delete e;
	}

	e = newDicomElement(DCM_StudyID);
	if (query.insert(e).bad()) {
		delete e;
	}

	FillInQuery(base, &query);//, server);

	std::string localAET = LOCAL_AE_TITLE;//GNC::Entorno::Instance()->GetDicomLocalAET();
		
	if (server->GetRetrieveSeries() || server->GetRetrieveMethod() == DicomServer::WADO) {
		//buscar + descargar series...
		//we have to know the series uids and obtain series by series
		e = newDicomElement(DCM_QueryRetrieveLevel);
		e->putString("SERIES");//wxString( "SERIES", wxConvUTF8).mb_str(conv));
		query.insert(e, true);

		e = newDicomElement(DCM_SeriesInstanceUID);
		if (query.insert(e).bad()) {
			delete e;
		}

		e = newDicomElement(DCM_Modality);
		if (query.insert(e).bad()) {
			delete e;
		}

		e = newDicomElement(DCM_SliceThickness);
		if (query.insert(e).bad()) {
			delete e;
		}

		e = newDicomElement(DCM_BodyPartExamined);
		if (query.insert(e).bad()) {
			delete e;
		}

		std::list<std::string> listOfUIDS;
		std::list<std::string> listOfModalities;
		{
			NetClient<FindAssociation> f(connectionKey, "C-GET/FIND");//, pNotificador);
			
			std::string localAET = LOCAL_AE_TITLE ;//GNC::Entorno::Instance()->GetDicomLocalAET();

			//if (server->useTLS) {
			//	f.SetTLS(server->GetCertificate(), server->GetPrivateKey(), server->GetverifyCredentials());
			//}
			//if (server->GetPACSUser() != "") {
			//	f.SetUserPass(server->GetPACSUser(), server->GetPACSPass());
			//}
			
			f.QueryServer(&query, server, localAET, CT_None);
			
			if (f.Stopped()){
				return false;
			}
			DcmStack* stack = f.GetResultStack();

			unsigned int numResults = 0;

			numResults = stack->card();


			for (unsigned int i = 0; i < numResults; i++) {

				if (stack->elem(i)->ident() == EVR_dataset) {
					DcmDataset* dset = dynamic_cast<DcmDataset*>(stack->elem(i));
					if (dset) {
						OFString OFSSeriesInstanceUID;
						OFString OFSeriesModality;
						if ( dset->findAndGetOFString(DCM_SeriesInstanceUID, OFSSeriesInstanceUID).good() && dset->findAndGetOFString(DCM_Modality, OFSeriesModality).good() )
						{
							std::string seriesModality(OFSeriesModality.c_str());
							std::string seriesInstanceUID(OFSSeriesInstanceUID.c_str());

							// 过滤层厚
							OFString OFSliceThickness;
							if ( dset->findAndGetOFString(DCM_SliceThickness, OFSliceThickness).good())
							{
								std::string sliceThickness(OFSliceThickness.c_str());

								if (sliceThickness.size() > 0 && m_filter_thickness != 0) {
									int thickness = atoi(sliceThickness.c_str());
									if (thickness != m_filter_thickness) {
										continue;
									}
								}								
							}
							// 过滤部位
							OFString OFBodyPartExamined;
							if ( dset->findAndGetOFString(DCM_BodyPartExamined, OFBodyPartExamined).good())
							{
								std::string bodyPartExamined(OFBodyPartExamined.c_str());

								if (bodyPartExamined.size() > 0 && m_bodyPartExamined.size() > 0) {
									if (bodyPartExamined != m_bodyPartExamined) {
										continue;
									}
								}								
							}

							listOfUIDS.push_back(seriesInstanceUID);
							listOfModalities.push_back(seriesModality);
						}
					}

				}
			}
		}

		for (std::list<std::string>::iterator itUIDS = listOfUIDS.begin(), itModalities = listOfModalities.begin();
			itUIDS != listOfUIDS.end(); ++itUIDS, ++itModalities) 
		{
			//it's mandatory to release connection here to start one connection for each series	
			ReleaseConnection(connectionKey);
			GetConnection(connectionKey);

			GIL::DICOM::DicomDataset baseAux = base;
			//baseAux.tags["0008|0052"] = "SERIES";
			baseAux.tags["0020|000e"] = (*itUIDS);
			baseAux.tags["0008|0060"] = (*itModalities);

			std::wstring ws_thickness = m_pThicknessEdit->GetText().GetData();
			std::string thickness	= toString(ws_thickness);
			baseAux.tags[GKDCM_SliceThickness] = thickness;


			ObtenerSerie(connectionKey, serverId, baseAux, link);
		}

		query.clear();	
	} 
	else if (retrieve_method == MOVE) { //->GetRetrieveMethod() == DicomServer::MOVE) {
		e = newDicomElement(DCM_QueryRetrieveLevel);
		e->putString("STUDY");//wxString( "STUDY", wxConvUTF8).mb_str(conv));
		query.insert(e, true);

		//if (server) {
		//	LOG_INFO("C-MOVE", "Obteniendo estudio del PACS " << serverId << ": " << server->AET << "@" << server->HostName << ":" << server->Port << " PDU=" << server->PDU << ", TLS=" << server->useTLS << ",  User = " << server->pacsUser << "Method=C-MOVE");
		//}

		//NetClient<MoveAssociation> a(connectionKey, "C-MOVE", pNotificador);

		//a.SetRole(Association::RT_Requestor);
		//a.SetModelo(pModelo);

		//if (server->useTLS) {
		//	a.SetTLS(server->GetCertificate(), server->GetPrivateKey(), server->GetverifyCredentials());
		//}
		//if (server->GetPACSUser() != "") {
		//	a.SetUserPass(server->GetPACSUser(), server->GetPACSPass());
		//}
		//a.QueryServer(&query, server, pModelo, localAET, CT_MoveSerie);

	}
	else {

		//if (server) {
		//	LOG_INFO("C-GET", "Obteniendo estudio del PACS " << serverId << ": " << server->AET << "@" << server->HostName << ":" << server->Port << " PDU=" << server->PDU << ", TLS=" << server->useTLS << ",  User = " << server->pacsUser << ", Method=C-GET");
		//}

		//we have to know the series uids and obtain series by series
		e = newDicomElement(DCM_QueryRetrieveLevel);
		e->putString("SERIES");//wxString( "SERIES", wxConvUTF8).mb_str(conv));
		query.insert(e, true);

		e = newDicomElement(DCM_SeriesInstanceUID);
		if (query.insert(e).bad()) {
			delete e;
		}

		e = newDicomElement(DCM_Modality);
		if (query.insert(e).bad()) {
			delete e;
		}

		NetClient<FindAssociation> f(connectionKey, "C-GET/FIND");//, pNotificador);

		std::string localAET = LOCAL_AE_TITLE ;//GNC::Entorno::Instance()->GetDicomLocalAET();

		//if (server->useTLS) {
		//	f.SetTLS(server->GetCertificate(), server->GetPrivateKey(), server->GetverifyCredentials());
		//}
		//if (!server->GetPACSUser().empty()) {
		//	f.SetUserPass(server->GetPACSUser(), server->GetPACSPass());
		//}

		f.QueryServer(&query, server, localAET, CT_None);
				
		if (f.Stopped()){
			return false;
		}
		DcmStack* stack = f.GetResultStack();

		unsigned int numResults = 0;

		numResults = stack->card();

		for (unsigned int i = 0; i < numResults; i++) {

			if (stack->elem(i)->ident() == EVR_dataset) {
				DcmDataset* dset = dynamic_cast<DcmDataset*>(stack->elem(i));
				if (dset) {
					OFString OFSStudyInstanceUID;
					OFString OFSSeriesInstanceUID;
					OFString OFSeriesModality;
					if ( dset->findAndGetOFString(DCM_SeriesInstanceUID, OFSSeriesInstanceUID).good() && dset->findAndGetOFString(DCM_Modality, OFSeriesModality).good() )
					{
						GIL::DICOM::DicomDataset baseAux = base;
						baseAux.tags["0008|0052"] = "SERIES";

						if (dset->findAndGetOFString(DCM_StudyInstanceUID, OFSStudyInstanceUID).good()) {
							baseAux.tags["0020|000d"] = OFSStudyInstanceUID.c_str();
						}

						baseAux.tags["0020|000e"] = OFSSeriesInstanceUID.c_str();
						baseAux.tags["0008|0060"] = OFSeriesModality.c_str();


						std::wstring ws_thickness = m_pThicknessEdit->GetText().GetData();
						std::string thickness	= toString(ws_thickness);
						baseAux.tags[GKDCM_SliceThickness] = thickness;

						ObtenerSerie(connectionKey, serverId, baseAux, link);
					}
				}

			}
		}

		query.clear();

	}

	return true;

}

bool DcmtkDLDicomDemoFrameWnd::ObtenerSerie(void* connectionKey, const std::string& serverId, const GIL::DICOM::DicomDataset& base, /*IModeloDicom* pModelo, GNC::IProxyNotificadorProgreso* pNotificador,*/ bool link)
{
	//if (pModelo == NULL) {
	//	return false;
	//}
	//DicomServerList* listaServidores = DicomServerList::Instance();
	//GNC::GCS::Ptr<DicomServer> server = listaServidores->GetServer(serverId);

	//if (!server.IsValid()) {
	//	LOG_ERROR("C-MOVE/C-GET", "Invalid server");
	//	return false;
	//}

	//wxCSConv conv = GetConv(server->GetDefaultCharset());

	std::string modality;
	if (base.tags.find("0008|0060") != base.tags.end()) {
		modality = base.tags.find("0008|0060")->second;
	}

	static std::string root_path = "G:\\temp\\";

	//std::string patient_id;
	//if (base.tags.find(GKDCM_PatientID) != base.tags.end()) {
	//	patient_id = base.tags.find(GKDCM_PatientID)->second;
	//}

	std::string study_id;
	if (base.tags.find(GKDCM_StudyInstanceUID) != base.tags.end()) {
		study_id = base.tags.find(GKDCM_StudyInstanceUID)->second;
	}

	std::string series_id;
	if (base.tags.find(GKDCM_SeriesInstanceUID) != base.tags.end()) {
		series_id = base.tags.find(GKDCM_SeriesInstanceUID)->second;
	}
	// 创建患者编号的文件夹
	std::string patient_path = root_path + m_cur_patient_id + "\\";
	TryCreateDir(patient_path);

	// 创建studyid的文件夹
	std::string study_path = patient_path + study_id + "\\";
	TryCreateDir(study_path);

	// 创建seriesid的文件夹
	std::string series_path = study_path + series_id + "\\" ;//DW::Util::TempDirOnlyPath::GetTempDirOnlyPath();
	TryCreateDir(series_path);
	
	DcmElement* e = NULL;
	DcmDataset query;

	unsigned int numResults = 0;

	if (retrieve_method == GET && modality.empty()) {//->GetRetrieveMethod() == DicomServer::GET && modality.empty()) { // We have to find series modality
		FillInQuery(base, &query);//, server);

		e = newDicomElement(DCM_SpecificCharacterSet);
		e->putString("");//server->GetDefaultCharset().c_str());
		query.insert(e);

		e = newDicomElement(DCM_QueryRetrieveLevel);
		e->putString("SERIES");//wxString( "SERIES", wxConvUTF8).mb_str(conv));
		query.insert(e, true);

		e = newDicomElement(DCM_StudyInstanceUID);
		if (query.insert(e).bad()) {
			delete e;
		}

		e = newDicomElement(DCM_SeriesInstanceUID);
		if (query.insert(e).bad()) {
			delete e;
		}

		e = newDicomElement(DCM_Modality);
		if (query.insert(e).bad()) {
			delete e;
		}

		NetClient<FindAssociation> f(connectionKey, "C-GET/FIND");//, pNotificador);

		std::string localAET = LOCAL_AE_TITLE ;//GNC::Entorno::Instance()->GetDicomLocalAET();

		//if (server->useTLS) {
		//	f.SetTLS(server->GetCertificate(), server->GetPrivateKey(), server->GetverifyCredentials());
		//}
		//if (server->GetPACSUser() != "") {
		//	f.SetUserPass(server->GetPACSUser(), server->GetPACSPass());
		//}

		if (!f.QueryServer(&query, server, localAET, CT_None)) 
		{
			return false;
		}

		if (f.Stopped()){
			return false;
		}
		DcmStack* stack = f.GetResultStack();

		OFString OFSeriesModality;

		numResults = stack->card();
		if (numResults >1) {
			//LOG_WARN("C-GET", "Obteniendo serie del PACS " << serverId << ": " << server->AET << "@" << server->HostName << ":" << server->Port << " PDU=" << server->PDU << ", TLS=" << server->useTLS << ",  User = " << server->pacsUser << ", there have a Series with more than one modality, we are going to process only first modality");
		}

		for (unsigned int i = 0; i < numResults; i++) {

			if (stack->elem(i)->ident() == EVR_dataset) {
				DcmDataset* dset = dynamic_cast<DcmDataset*>(stack->elem(i));
				if (dset) {

					if ( dset->findAndGetOFString(DCM_Modality, OFSeriesModality).good() && OFSeriesModality.size() > 0 )
					{
						modality = OFSeriesModality.c_str();
						break;
					}
				}

			}
		}
		query.clear();
	}// end query modality

	//association to make finds...
	NetClient<FindAssociation> f(connectionKey, "C-GET/FIND");//, pNotificador);

	std::string localAET = LOCAL_AE_TITLE;//GNC::Entorno::Instance()->GetDicomLocalAET();

	//if (server->useTLS) {
	//	f.SetTLS(server->GetCertificate(), server->GetPrivateKey(), server->GetverifyCredentials());
	//}
	//if (server->GetPACSUser() != "") {
	//	f.SetUserPass(server->GetPACSUser(), server->GetPACSPass());
	//}

	if (retrieve_method == WADO)//->GetRetrieveMethod() == DicomServer::WADO) 
	{
		//we need to know StudyInstanceUID, SeriesInstanceUID and SopInstanceUID of all elements
		std::string seriesInstanceUID, studyInstanceUID;
		if (!base.getTag(GKDCM_SeriesInstanceUID, seriesInstanceUID)) {
			//LOG_ERROR("DcmtkDLDicomDemoFrameWnd", "to obtain a series you must specify seriesInstanceUID");
			return false;
		}
		if (!base.getTag(GKDCM_StudyInstanceUID, studyInstanceUID)) {
			FillInQuery(base, &query);//, server);

			e = newDicomElement(DCM_SpecificCharacterSet);
			e->putString("");//server->GetDefaultCharset().c_str());
			query.insert(e);

			e = newDicomElement(DCM_QueryRetrieveLevel);
			e->putString("SERIES");//wxString( "SERIES", wxConvUTF8).mb_str(conv));
			query.insert(e, true);

			e = newDicomElement(DCM_StudyInstanceUID);
			if (query.insert(e).bad()) {
				delete e;
			}

			e = newDicomElement(DCM_SeriesInstanceUID);
			if (query.insert(e).bad()) {
				delete e;
			}

			if (!f.QueryServer(&query, server, localAET, CT_None)) 
			{
				return false;
			}

			if (f.Stopped()){
				return false;
			}
			DcmStack* stack = f.GetResultStack();

			OFString tempOf;

			for (unsigned int i = 0; i < stack->card(); i++) {

				if (stack->elem(i)->ident() == EVR_dataset) {
					DcmDataset* dset = dynamic_cast<DcmDataset*>(stack->elem(i));
					if (dset) {

						if ( dset->findAndGetOFString(DCM_StudyInstanceUID, tempOf).good() && tempOf.size() > 0 )
						{
							studyInstanceUID = tempOf.c_str();
							break;
						}
					}

				}
			}
			query.clear();
		}
		if (studyInstanceUID.empty()) {
			//LOG_ERROR("DcmtkDLDicomDemoFrameWnd", "StudyInstanceUID not found");
			return false;
		}

		//now we have to get all sop instance uids...
		f.DeleteResultStack();
		e = newDicomElement(DCM_SpecificCharacterSet);
		e->putString("");//server->GetDefaultCharset().c_str());
		query.insert(e);

		e = newDicomElement(DCM_QueryRetrieveLevel);
		e->putString("IMAGE");//wxString( "IMAGE", wxConvUTF8).mb_str(conv));
		query.insert(e, true);

		e = newDicomElement(DCM_SeriesInstanceUID);
		e->putString(seriesInstanceUID.c_str());//wxString( seriesInstanceUID.c_str(), wxConvUTF8).mb_str(conv));
		if (query.insert(e).bad()) {
			delete e;
		}

		e = newDicomElement(DCM_SOPInstanceUID);
		if (query.insert(e).bad()) {
			delete e;
		}

		e = newDicomElement(DCM_InstanceNumber);
		if (query.insert(e).bad()) {
			delete e;
		}

		if (!f.QueryServer(&query, server, localAET, CT_None)) 
		{
			return false;
		}

		if (f.Stopped()){
			return false;
		}
		DcmStack* stack = f.GetResultStack();

		OFString tempOf;
		std::list<std::pair<long, std::string> > instances;

		for (unsigned int i = 0; i < stack->card(); i++) {
			if (stack->elem(i)->ident() == EVR_dataset) {
				DcmDataset* dset = dynamic_cast<DcmDataset*>(stack->elem(i));
				if (dset) {
					if ( dset->findAndGetOFString(DCM_SOPInstanceUID, tempOf).good() && tempOf.size() > 0 )
					{
						const std::string sopInstanceUID = tempOf.c_str();
						long instanceNumber;
						std::pair<long, std::string> item;
						item.second = sopInstanceUID;
						if ( dset->findAndGetLongInt(DCM_InstanceNumber, instanceNumber).good() ) {
							item.first = instanceNumber;
						} else {
							item.first = LONG_MAX;
						}
						instances.push_back(item);
					}
				}
			}
		}
		query.clear();

		//sort by instance number...
		//instances.sort(compare_instances);
		std::list<std::string> sopInstanceUIDs;
		std::list<long> instanceNumbers;
		for (std::list<std::pair<long, std::string> >::const_iterator it = instances.begin(); it != instances.end(); ++it) {
			sopInstanceUIDs.push_back((*it).second);
			instanceNumbers.push_back((*it).first);
		}

		//now we have studyInstanceUID, seriesInstanceUID and sopInstancesUIDS.... download it!
		//return DownloadWADOImages(serverId, studyInstanceUID, seriesInstanceUID, sopInstanceUIDs, instanceNumbers, pModelo, pNotificador, link);
		return false;
	}//end wado
	else 
	{//get and move
		FillInQuery(base, &query);//, server);

		e = newDicomElement(DCM_SpecificCharacterSet);
		e->putString("");//server->GetDefaultCharset().c_str());
		query.insert(e);			

		e = newDicomElement(DCM_QueryRetrieveLevel);
		e->putString("SERIES");//wxString( "SERIES", wxConvUTF8).mb_str(conv));
		query.insert(e, true);

		e = newDicomElement(DCM_SeriesInstanceUID);
		if (query.insert(e).bad()) {
			delete e;
		}

		e = newDicomElement(DCM_Modality);
		if (query.insert(e).bad()) {
			delete e;
		}

		e = newDicomElement(DCM_SeriesNumber);
		if (query.insert(e).bad()) {
			delete e;
		}

		std::string localAET = LOCAL_AE_TITLE ;//GNC::Entorno::Instance()->GetDicomLocalAET();

		if (retrieve_method == MOVE) {//->GetRetrieveMethod() == DicomServer::MOVE) {

			//if (server) {
			//	LOG_INFO("C-MOVE", "Downloading serie from PACS " << serverId << ": " << server->AET << "@" << server->HostName << ":" << server->Port << " PDU=" << server->PDU << ", TLS=" << server->useTLS << ",  User = " << server->pacsUser << ", Method=C-MOVE");
			//}

			//if (server) {
			//	LOG_INFO("C-MOVE", "Downloading study from PACS " << serverId << ": " << server->AET << "@" << server->HostName << ":" << server->Port << " PDU=" << server->PDU << ", TLS=" << server->useTLS << ",  User = " << server->pacsUser << ", Method=C-MOVE");
			//}

			//NetClient<MoveAssociation> a(connectionKey, "C-MOVE", pNotificador);
			//a.SetRole(Association::RT_Requestor);
			//a.SetModelo(pModelo);


			//if (server->useTLS) {
			//	a.SetTLS(server->GetCertificate(), server->GetPrivateKey(), server->GetverifyCredentials());
			//}
			//if (server->GetPACSUser() != "") {
			//	a.SetUserPass(server->GetPACSUser(), server->GetPACSPass());
			//}
			//if (!a.QueryServer(&query, server, pModelo, localAET, CT_MoveSerie)) {
				return false;
			//}
		}
		else {
			//if (server) {
			//	LOG_INFO("C-GET", "Downloading serie from PACS " << serverId << ": " << server->AET << "@" << server->HostName << ":" << server->Port << " PDU=" << server->PDU << ", TLS=" << server->useTLS << ",  User = " << server->pacsUser << ", Method=C-GET");
			//}

			NetClient<GetAssociation> a(connectionKey, "C-GET");//, pNotificador);
			a.SetWellKnownNumResults(numResults);
			a.SetStorageSOPClasses(modality);//GIL::DICOM::Conformance::GetModalities().GetSupportedSOPClassUIDs(modality));
			//a.SetModelo(pModelo);
			a.SetPath(series_path);

			//if (server->useTLS) {
			//	a.SetTLS(server->GetCertificate(), server->GetPrivateKey(), server->GetverifyCredentials());
			//}
			//if (server->GetPACSUser() != "") {
			//	a.SetUserPass(server->GetPACSUser(), server->GetPACSPass());
			//}
			if (!a.QueryServer(&query, server, localAET, CT_MoveSerie)) {
				return false;
			}
		}
		query.clear();
	}
	return true;

}

bool DcmtkDLDicomDemoFrameWnd::ObtenerImagen(void* connectionKey, const std::string& serverId, const GIL::DICOM::DicomDataset& base/*, IModeloDicom* pModelo,GNC::IProxyNotificadorProgreso* pNotificador*/)
{
	//if (pModelo == NULL) {
	//	return false;
	//}
	//DicomServerList* listaServidores = DicomServerList::Instance();
	//GNC::GCS::Ptr<DicomServer> server = listaServidores->GetServer(serverId);

	//if (!server.IsValid()) {
	//	LOG_ERROR("C-MOVE/C-GET", "Invalid server");
	//	return false;
	//}

	//wxCSConv conv = GetConv(server->GetDefaultCharset());

	DcmDataset query;
	DcmElement* e = NULL;

	FillInQuery(base, &query);//, server);

	e = newDicomElement(DCM_SpecificCharacterSet);
	e->putString("");//server->GetDefaultCharset().c_str());
	query.insert(e);

	e = newDicomElement(DCM_QueryRetrieveLevel);
	e->putString("IMAGE");//wxString( "IMAGE", wxConvUTF8).mb_str(conv));
	query.insert(e, true);

	e = newDicomElement(DCM_InstanceNumber);
	if (query.insert(e).bad()) {
		delete e;
	}

	std::string localAET = LOCAL_AE_TITLE ;//GNC::Entorno::Instance()->GetDicomLocalAET();

	if (retrieve_method == MOVE) { //->GetRetrieveMethod() == DicomServer::MOVE) {

		//if (server) {
		//	LOG_INFO("C-MOVE", "Downloading imagen from PACS " << serverId << ": " << server->AET << "@" << server->HostName << ":" << server->Port << " PDU=" << server->PDU << ", TLS=" << server->useTLS << ",  User = " << server->pacsUser << ", Method=C-MOVE");
		//}

		//NetClient<MoveAssociation> a(connectionKey, "C-MOVE", pNotificador);
		//a.SetModelo(pModelo);

		//if (server->useTLS) {
		//	a.SetTLS(server->GetCertificate(), server->GetPrivateKey(), server->GetverifyCredentials());
		//}
		//if (server->GetPACSUser() != "") {
		//	a.SetUserPass(server->GetPACSUser(), server->GetPACSPass());
		//}
		//if (!a.QueryServer(&query, server, pModelo, localAET, CT_MoveImagen)) {
			return false;
		//}

	}
	else if (retrieve_method == GET) { //->GetRetrieveMethod() == DicomServer::GET) {

		//if (server) {
		//	LOG_INFO("C-GET", "Downloading image from PACS " << serverId << ": " << server->AET << "@" << server->HostName << ":" << server->Port << " PDU=" << server->PDU << ", TLS=" << server->useTLS << ",  User = " << server->pacsUser << ", Method=C-GET");
		//}

		NetClient<GetAssociation> a(connectionKey, "C-GET");//, pNotificador);
		//a.SetModelo(pModelo);

		//if (server->useTLS) {
		//	a.SetTLS(server->GetCertificate(), server->GetPrivateKey(), server->GetverifyCredentials());
		//}
		//if (server->GetPACSUser() != "") {
		//	a.SetUserPass(server->GetPACSUser(), server->GetPACSPass());
		//}
		//if (!a.QueryServer(&query, server, pModelo, localAET, CT_MoveImagen))  {
		if (!a.QueryServer(&query, server, localAET, CT_MoveImagen))  {
			return false;
		}
	} else if (retrieve_method == WADO) { //->GetRetrieveMethod() == DicomServer::WADO) {
		//we have to get seriesInstance and studyInstanceUID...
		std::string seriesInstanceUID, studyInstanceUID;
		if (!base.getTag(GKDCM_SeriesInstanceUID, seriesInstanceUID) || !base.getTag(GKDCM_StudyInstanceUID, studyInstanceUID)) {
			FillInQuery(base, &query);//, server);

			e = newDicomElement(DCM_SpecificCharacterSet);
			e->putString("");//server->GetDefaultCharset().c_str());
			query.insert(e);

			e = newDicomElement(DCM_QueryRetrieveLevel);
			e->putString("IMAGE");//wxString( "IMAGE", wxConvUTF8).mb_str(conv));
			query.insert(e, true);

			e = newDicomElement(DCM_StudyInstanceUID);
			if (query.insert(e).bad()) {
				delete e;
			}
			e = newDicomElement(DCM_SeriesInstanceUID);
			if (query.insert(e).bad()) {
				delete e;
			}

			//association to make finds...
			NetClient<FindAssociation> f(connectionKey, "WADO/FIND");//, pNotificador);

			std::string localAET = LOCAL_AE_TITLE;//GNC::Entorno::Instance()->GetDicomLocalAET();

			//if (server->useTLS) {
			//	f.SetTLS(server->GetCertificate(), server->GetPrivateKey(), server->GetverifyCredentials());
			//}
			//if (server->GetPACSUser() != "") {
			//	f.SetUserPass(server->GetPACSUser(), server->GetPACSPass());
			//}


			//if (!f.QueryServer(&query, server, pModelo, localAET, CT_None)) 
			if (!f.QueryServer(&query, server, localAET, CT_None)) 
			{
				return false;
			}

			if (f.Stopped()){
				return false;
			}
			DcmStack* stack = f.GetResultStack();

			OFString tempOf;

			for (unsigned int i = 0; i < stack->card(); i++) {

				if (stack->elem(i)->ident() == EVR_dataset) {
					DcmDataset* dset = dynamic_cast<DcmDataset*>(stack->elem(i));
					if (dset) {

						if ( dset->findAndGetOFString(DCM_StudyInstanceUID, tempOf).good() && tempOf.size() > 0 )
						{
							studyInstanceUID = tempOf.c_str();
						}
						if ( dset->findAndGetOFString(DCM_SeriesInstanceUID, tempOf).good() && tempOf.size() > 0 )
						{
							seriesInstanceUID = tempOf.c_str();
						}
					}
				}
			}
			query.clear();
		}
		if (studyInstanceUID.empty() || seriesInstanceUID.empty()) {
			//LOG_ERROR("DcmtkDLDicomDemoFrameWnd", "StudyInstanceUID or SeriesInstanceUID not found");
			return false;
		}
		std::list<std::string> sopInstanceUIDs;
		sopInstanceUIDs.push_back(base.getTag(GKDCM_SOPInstanceUID));
		std::list<long> instanceNumbers;
		instanceNumbers.push_back(0);
		//now we have studyInstanceUID, seriesInstanceUID and sopInstancesUIDS.... download it!
		//return DownloadWADOImages(serverId, studyInstanceUID, seriesInstanceUID, sopInstanceUIDs, instanceNumbers, pModelo, pNotificador, false);
		return false;
	}
	return true;
}

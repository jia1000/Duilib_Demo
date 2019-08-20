#include "DcmtkDLDicomDemoFrameWnd.h"

#include "DcmtkDLDicomSetDemoFrameWnd.h"

#include "api/dicom/dcmdictionary.h"
#include "api/dicom/dicomdataset.h"

#include "main/controllers/dcmtk/dicomfindassociation.h"
#include "main/controllers/dcmtk/dicomnetwork.h"



DcmtkDLDicomDemoFrameWnd::DcmtkDLDicomDemoFrameWnd(void)
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
		} else if (_tcscmp(pszCtrlName, _T("dicom_set")) == 0) {
			DcmtkDLDicomSetDemoFrameWnd* duiFrame = new DcmtkDLDicomSetDemoFrameWnd();
			duiFrame->Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, 0L);
			duiFrame->CenterWindow();
			duiFrame->ShowWindow();
		} else if (_tcscmp(pszCtrlName, _T("btn_filter")) == 0) {
			DoSearchTest();
		} 
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

void DcmtkDLDicomDemoFrameWnd::DoSearchTest()
{
	//GNC::GCS::StoredQuery pStoredQuery;

	GIL::DICOM::DicomDataset queryWrapper;
	queryWrapper.tags[GKDCM_QueryRetrieveLevel] = "STUDY";
	queryWrapper.tags[GKDCM_PatientName] = "123";

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

	std::string aet_title	= "DEEPWISESCP";
	std::string host_addr	= "192.168.1.253";
	std::string port		= "22222";
	std::string aet_local	= "DEEPWISE_001";
	int psdu_length			= 16384;

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
			, atoi(port.c_str())
			, aet_local
			);

		std::list< GIL::DICOM::DicomDataset> resultsWrapper;
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

		//LOG_INFO(ambitolog, "Disconnected");
		fs.Drop();
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
				//LOG_ERROR("PACSCONTROLLER", "No se pudo insertar el elemento: (" << e->getTag().toString().c_str() << "): " << cond.text());
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
							//LOG_ERROR("PACSCONTROLLER", "No se pudo insertar el elemento: (" << ei->getTag().toString().c_str() << "): " << cond.text());
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
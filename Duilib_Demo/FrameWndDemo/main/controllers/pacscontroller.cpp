/*
*  
*  $Id: pacscontroller.cpp $
*  DeepWise Dicom Viewer
*
*  Copyright 2017-2030 DeepWise All rights reserved.
*  http://www.deepwise.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*
*/

#include "pacscontroller.h"

#include <string>

#include <dcmtk/dcmdata/dcdeftag.h>

#include "main/controllers/dcmtk/dicomfindassociation.h"
#include "main/controllers/dcmtk/dicomgetassociation.h"
#include "main/controllers/dcmtk/dicomnetwork.h"
#include "main/controllers/dcmtk/dicomnetclient.h"

#include "api/dicom/dcmdictionary.h"

#include "utility_tool/common_utils.h"
#include "utility_tool/string_converse.h"
#include "controller/configcontroller.h"

#define LOCAL_AE_TITLE	"DEEPWISE_002"

namespace GIL {
	namespace DICOM {
		PACSController* PACSController::m_pInstance = NULL;
		
		PACSController::PACSController()
			: retrieve_method(GET)
		{
			InitDicomServer();
		}

		PACSController::~PACSController()
		{
			
		}
		PACSController* PACSController::Instance()
		{
			if (m_pInstance == NULL) {
				m_pInstance = new PACSController();
			}
			return m_pInstance;
		}
		void PACSController::FreeInstance()
		{
			if (m_pInstance != NULL) {
				delete m_pInstance;
				m_pInstance = NULL;
			}			
		}
		void PACSController::InitDicomServer()
		{
			std::string aet_nubmer = ConfigController::Instance()->GetAETNumberStr();
			std::string host_addr = ConfigController::Instance()->GetAETHostStr();
			std::string aet_title = ConfigController::Instance()->GetAETTitleStr();
			std::string aet_port = ConfigController::Instance()->GetAETPortStr();
			int port = atoi(aet_port.c_str());
			std::string aet_pdu = ConfigController::Instance()->GetAETPduStr();
			int pdu_length = atoi(aet_pdu.c_str());
			m_localAET = ConfigController::Instance()->GetAETLocalNameStr();
			server = new DicomServer(aet_nubmer, aet_title, host_addr, port, pdu_length);
		}

		bool PACSController::ObtainDicomDataSet(void* connectionKey, const GIL::DICOM::DicomDataset& base, 
			std::list< GNC::GCS::Ptr<GIL::DICOM::DicomDataset> >& resultsWrapper)
		{
			InitDicomServer();
			
			DcmDataset query;

			FillInQuery(base, &query);

			//std::string localAET = LOCAL_AE_TITLE;	
			
			NetClient<FindAssociation> f(connectionKey, "C-FIND");

			f.QueryServer(&query, server, resultsWrapper, m_localAET);

			if (f.Stopped()){
				return false;
			}

			if (resultsWrapper.size() <= 0) {
				return false;
			}

			query.clear();	
			return true;
		}

		bool PACSController::DownloadDicomFilesBySeries(
			void* connectionKey,
			const GIL::DICOM::DicomDataset& base, 
			const std::string series_path
			)
		{
			std::string modality;
			if (base.tags.find(GKDCM_Modality) != base.tags.end()) {
				modality = base.tags.find(GKDCM_Modality)->second;
			}

			DcmDataset query;
			FillInQuery(base, &query);

			NetClient<GetAssociation> a(connectionKey, "C-GET");
			a.SetStorageSOPClasses(modality);
			a.SetPath(series_path);

			if (!a.QueryServer(&query, server, m_localAET, CT_MoveSerie)) {
				return false;
			}
			query.clear();
			return true;
		}
				
		void PACSController::FillInQuery(const GIL::DICOM::DicomDataset& base, DcmDataset* query)//, const GNC::GCS::Ptr<DicomServer>& server)
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
						//LOG_ERROR("PACSController", "No se pudo insertar el elemento: (" << e->getTag().toString().c_str() << "): " << cond.text());
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

		DcmElement* PACSController::CrearElementoConValor(const char* s)
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
		void PACSController::GetConnection(void* connectionKey)
		{
			GIL::DICOM::DCMTK::Network::Instance(connectionKey);
		}

		void PACSController::ReleaseConnection(void* connectionKey)
		{
			GIL::DICOM::DCMTK::Network::FreeInstance(connectionKey);
		}

		void PACSController::InitFindQueryWrapper(GIL::DICOM::DicomDataset&  queryWrapper)
		{
			queryWrapper.tags[GKDCM_SpecificCharacterSet] = "";	//"0008|0005"
			queryWrapper.tags[GKDCM_StudyDate] = "";			//"0008|0020"
			queryWrapper.tags[GKDCM_StudyTime] = "";			//"0008|0030"
			queryWrapper.tags[GKDCM_QueryRetrieveLevel] = "";	//"0008|0052"
			queryWrapper.tags[GKDCM_Modality] = "";				//"0008|0060"
			queryWrapper.tags[GKDCM_ModalitiesInStudy] = "";	//"0008|0061"  CT MG DX  etc.

			queryWrapper.tags[GKDCM_PatientName] = "";			//"0010|0010"
			queryWrapper.tags[GKDCM_PatientID] = "";			//"0010|0020"

			queryWrapper.tags[GKDCM_BodyPartExamined] = "";		//"0018|0015" CHEST  BREAST
			queryWrapper.tags[GKDCM_SliceThickness] = "";		//"0018|0050"  CT MG DX  etc.

			queryWrapper.tags[GKDCM_StudyInstanceUID] = "";		//"0020|000d"
			queryWrapper.tags[GKDCM_SeriesInstanceUID] = "";	//"0020|000e"
			queryWrapper.tags[GKDCM_StudyID] = "";				//"0020|0010"
			queryWrapper.tags[GKDCM_SeriesNumber] = "";			//"0020|0011"
			queryWrapper.tags[GKDCM_InstanceNumber] = "";		//"0020|0013"  Image用
			queryWrapper.tags[GKDCM_NumberOfStudyRelatedInstances] = ""; // "0020|1208"  影像数量

		}

		void PACSController::SetWrapper(GIL::DICOM::DicomDataset&  queryWrapper, char* tag, std::string value)
		{
			if(tag)
			{
				queryWrapper.tags[tag] = value;
			}
		}
	};
};

/*
 *  
 *  $Id: idicomconformance.cpp $
 *  DeepWise Dicom Viewer
 *
 *  Copyright 2017-2030 DeepWise All rights reserved.
 *  http://www.deepwise.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#include <list>
#include <map>
#include <cstring>

#include "idicomconformance.h"
#include <api/globals.h>
#include <api/internationalization/internationalization.h>
//#include <api/ientorno.h>
#include <api/controllers/icontroladorlog.h>
//#include <main/controllers/configurationcontroller.h>

#include "tinyxml2/tinyxml2.h"
using namespace tinyxml2;

//#include <wx/xml/xml.h>
//#include <wx/file.h>
//#include <wx/sstream.h>
//#include <wx/msgdlg.h>
//#include <wx/tokenzr.h>
//#include <wx/filename.h>
//#include <wx/msw/stdpaths.h>
//#include <wx/stdpaths.h>

namespace GIL {
	namespace DICOM {
		static std::string emptyString;
		static TransferSyntaxList emptyTSList;
	}
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

void GIL::DICOM::TransferSyntaxMap::AddTransferSyntax(const std::string& tsRef, const std::string& tsUID)
{
	TransferSyntaxMapImpl& impl = *this;
	impl[tsRef] = tsUID;
}

const std::string& GIL::DICOM::TransferSyntaxMap::GetTransferSyntaxUID(const std::string& tsRef) const {
	const_iterator it = find(tsRef);
	if (it != end()) {
		return (*it).second;
	}
	else{
		return emptyString;
	}				
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

void GIL::DICOM::SOPClassMap::AddSOPClass(const std::string& scRef, const std::string& scUID) {
	SOPClassMapImpl::iterator it = find(scRef);

	if (it == end()) {
		SOPClassMapImpl& impl = *this;
		impl[scRef] = GIL::DICOM::SOPClassTuple(scUID);
	}
}

void GIL::DICOM::SOPClassMap::AddTransferSyntaxForSOP(const std::string& scRef, const std::string& tsRef) {
	SOPClassMapImpl::iterator it = find(scRef);

	if (it != end()) {
		const std::string& tsUID = GIL::DICOM::Conformance::GetTransferSyntaxes().GetTransferSyntaxUID(tsRef);
		if (tsUID.size() != 0) {
			SOPClassTuple& sop = (*it).second;
			sop.tsRefs.push_back(tsRef);
			sop.tsUIDs.push_back(tsUID);
		}
		else {
			LOG_ERROR("GIL/DICOM/Conformance", "Unable to add supported transfer syntax " << tsRef << " to SOP Class " << scRef << " . Transfer Syntax does not exists in model: Add it first.");			
		}
	}
	else {
		LOG_ERROR(_Std("GIL/DICOM/Conformance"), _Std("Unable to add supported transfer syntax ") << tsRef << _Std(" to SOP Class ") << scRef <<_Std(" . SOP Class does not exists in model: Add it first."));
	}
}

const std::string& GIL::DICOM::SOPClassMap::GetSOPClassUID(const std::string& scRef) const {
	
	SOPClassMapImpl::const_iterator it = find(scRef);
	if (it != end()) {
		return (*it).second.scUID;
	}
	return emptyString;
}

const GIL::DICOM::TransferSyntaxList& GIL::DICOM::SOPClassMap::GetSupportedTransferSyntaxRefs(const std::string& scRef) const {

	SOPClassMapImpl::const_iterator it = find(scRef);
	if (it != end()) {
		return (*it).second.tsRefs;
	}
	else {
		return emptyTSList;
	}
}

const GIL::DICOM::TransferSyntaxList& GIL::DICOM::SOPClassMap::GetSupportedTransferSyntaxUIDs(const std::string& scRef) const {

	SOPClassMapImpl::const_iterator it = find(scRef);
	if (it != end()) {
		return (*it).second.tsUIDs;
	}
	else {
		return emptyTSList;
	}
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

void GIL::DICOM::ModalityMap::AddModality(const std::string& mRef, const std::string& descr) {
	ModalityMapImpl::iterator it = find(mRef);

	if (it == end()) {
		ModalityMapImpl& impl = *this;
		impl[mRef] = GIL::DICOM::ModalityTuple(descr);
	}
}

void GIL::DICOM::ModalityMap::AddSOPClassForModality(const std::string& mRef, const std::string& scRef) {
	ModalityMapImpl::iterator it = find(mRef);

	if (it != end()) {
		std::string scUID = GIL::DICOM::Conformance::GetStorageSOPClasses().GetSOPClassUID(scRef);		
		if (scUID.size() == 0) {
			scUID = GIL::DICOM::Conformance::GetScuSOPClasses().GetSOPClassUID(scRef);
			if (scUID.size() == 0) {
				scUID = GIL::DICOM::Conformance::GetScpSOPClasses().GetSOPClassUID(scRef);				
			}
		}
		if (scUID.size() == 0) {
			//LOG_ERROR(_Std("GIL/DICOM/Conformance"), _Std("Unable to add supported SOP Class ") << scRef << _Std(" to Modality ") << mRef <<_Std(" . SOP Class does not exists in model: Add it first."));
		}
		else {
			ModalityTuple& modality = (*it).second;
			modality.scRefs.push_back(scRef);
			modality.scUIDs.push_back(scUID);
		}
	}
	else {
		//LOG_ERROR(_Std("GIL/DICOM/Conformance"), _Std("Unable to add supported SOP Class ") << scRef << _Std(" to Modality ") << mRef <<_Std(" . Modality does not exists in model: Add it first."));
	}
}

const std::string& GIL::DICOM::ModalityMap::GetDescription(const std::string& mRef) const {

	ModalityMapImpl::const_iterator it = find(mRef);
	if (it != end()) {
		return (*it).second.descr;
	}
	return emptyString;
}

/** Gets the SOP Classes names (refs) list supported by this Modality **/
const GIL::DICOM::SOPClassList& GIL::DICOM::ModalityMap::GetSupportedSOPClassRefs(const std::string& mRef) const {

	ModalityMapImpl::const_iterator it = find(mRef);
	if (it != end()) {
		return (*it).second.scRefs;
	}
	else {
		return emptyTSList;
	}

}
			
/** Gets the SOP Classes oid list supported by this Modality **/
const GIL::DICOM::SOPClassList& GIL::DICOM::ModalityMap::GetSupportedSOPClassUIDs(const std::string& mRef) const {

	ModalityMapImpl::const_iterator it = find(mRef);
	if (it != end()) {
		return (*it).second.scUIDs;
	}
	else {
		return emptyTSList;
	}
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------


GIL::DICOM::TransferSyntaxMap GIL::DICOM::Conformance::m_TransferSyntaxes;
GIL::DICOM::SOPClassMap GIL::DICOM::Conformance::m_StorageSOPClasses;
GIL::DICOM::SOPClassMap GIL::DICOM::Conformance::m_ScuSOPClasses;
GIL::DICOM::SOPClassMap GIL::DICOM::Conformance::m_ScpSOPClasses;
GIL::DICOM::ModalityMap GIL::DICOM::Conformance::m_Modalities;
bool GIL::DICOM::Conformance::m_Loaded = false;

#define CONFORMANCE_FILE "conformance.xml"
#define CONFORMANCE_ROOT_TAG "dicom-conformance"

//wxString GetResourceDir()
//{
	//wxString pluginsDir;
	//wxString resourcesDir;
	//wxString langDir;

	//wxFileName executable_path = wxStandardPaths::Get().GetExecutablePath();
//#if defined(_WINDOWS)
//	pluginsDir = executable_path.GetPath() + wxFileName::GetPathSeparator() + wxT("Plugins");
//	langDir = executable_path.GetPath() + wxFileName::GetPathSeparator() + wxT("lang");
//	resourcesDir = executable_path.GetPath();
//#elif defined(LINUX)
//#if defined(CUSTOM_PACKAGE)
//	pluginsDir = executable_path.GetPath() + wxFileName::GetPathSeparator() + wxT("Plugins");
//	resourcesDir = executable_path.GetPath();
//	langDir = executable_path.GetPath() + wxFileName::GetPathSeparator() + wxT("lang");
//#else
//	pluginsDir = wxStandardPaths::Get().GetPluginsDir() + wxFileName::GetPathSeparator() + wxT("Plugins");
//	resourcesDir = wxStandardPaths::Get().GetResourcesDir();
//	langDir = wxStandardPaths::Get().GetResourcesDir() + wxFileName::GetPathSeparator() + wxT("lang");
//#endif
//
//#else
//	pluginsDir = wxStandardPaths::Get().GetPluginsDir();
//	langDir = wxStandardPaths::Get().GetResourcesDir() + wxFileName::GetPathSeparator() + wxT("lang");
//	resourcesDir = wxStandardPaths::Get().GetResourcesDir();
//#endif
	//return resourcesDir;
//}

void GIL::DICOM::Conformance::Load() {
	
	m_Loaded = true;
	
	//wxString resouce_dir = "F:\\dev_study\\duilib_study\\Duilib_Demo\\Debug";//GetResourceDir();
	//wxFileName cpath = FROMPATH(GNC::GCS::IEntorno::Instance()->GetGinkgoResourcesDir()) + wxFileName::GetPathSeparator() + CONFORMANCE_FILE;
	//wxFileName cpath = FROMPATH(resouce_dir) + wxFileName::GetPathSeparator() + CONFORMANCE_FILE;

	std::string cpath = ".\\";
	cpath += CONFORMANCE_FILE;

	//wxXmlDocument xmldoc;
	//xmldoc.Load(cpath.GetFullPath());
	//if (!xmldoc.IsOk()) {
	//	LOG_ERROR(_Std("GIL/DICOM/Conformance"), _Std("Error loading DICOM Conformance file" ));
	//	m_Loaded = false;
	//	return;
	//}

	XMLDocument xmldoc;
	XMLError errXml = xmldoc.LoadFile(cpath.c_str());
	if (XML_SUCCESS != errXml) {
		//LOG_ERROR(_Std("GIL/DICOM/Conformance"), _Std("Error loading DICOM Conformance file" ));
		LOG_ERROR("GIL/DICOM/Conformance", "Error loading DICOM Conformance file" << cpath);
		m_Loaded = false;
		return;
	}

	//wxXmlNode* pRoot = xmldoc.GetRoot();
	//if (pRoot->GetName().CmpNoCase(CONFORMANCE_ROOT_TAG) != 0) {
	//	LOG_ERROR(_Std("GIL/DICOM/Conformance"), _Std("Invalid DICOM Conformance file" ));
	//	m_Loaded = false;
	//	return;
	//}

	XMLElement* pRoot = xmldoc.RootElement();
	if (!pRoot) {
		return ;
	}

	XMLElement* sub_xml = pRoot->FirstChildElement("transfer-syntaxes");
	if (sub_xml) {
		// Loading Transfer Syntaxes
		const char * ccc = sub_xml->Value();
		XMLElement* ele_transfer = sub_xml->FirstChildElement();
		for ( ; ele_transfer != NULL; ele_transfer = ele_transfer->NextSiblingElement()) {
			const char* ref_id = ele_transfer->Attribute("ref-id");
			const char* uid = ele_transfer->Attribute("uid");	
			if (ref_id && uid) {
				m_TransferSyntaxes.AddTransferSyntax(ref_id, uid);
			}
		}
	}

	sub_xml = pRoot->FirstChildElement("storage-sop-classes");
	if (sub_xml) {
		// Loading Storage SOP Classes
		XMLElement* ele_1 = sub_xml->FirstChildElement();
		for ( ; ele_1 != NULL; ele_1 = ele_1->NextSiblingElement()) {
			const char* ref_id	= ele_1->Attribute("ref-id");
			const char* uid		= ele_1->Attribute("uid");	
			if (ref_id && uid) {
				m_StorageSOPClasses.AddSOPClass(ref_id, uid);
				XMLElement* ele_2 = ele_1->FirstChildElement();
				for ( ; ele_2 != NULL; ele_2 = ele_2->NextSiblingElement()) {
					const char* ref	= ele_2->Attribute("ref");
					if (ref) {
						m_StorageSOPClasses.AddTransferSyntaxForSOP(ref_id, ref);
					}
				}
			}
		}
	}

	sub_xml = pRoot->FirstChildElement("scu-sop-classes");
	if (sub_xml) {
		// Loading SCU SOP Classes
		XMLElement* ele_1 = sub_xml->FirstChildElement();
		for ( ; ele_1 != NULL; ele_1 = ele_1->NextSiblingElement()) {
			if (0 == strcmp("sop-class", ele_1->Value())) {
				const char* ref_id	= ele_1->Attribute("ref-id");
				const char* uid		= ele_1->Attribute("uid");	
				if (ref_id && uid) {
					m_ScuSOPClasses.AddSOPClass(ref_id, uid);
					XMLElement* ele_2 = ele_1->FirstChildElement();
					for ( ; ele_2 != NULL; ele_2 = ele_2->NextSiblingElement()) {
						const char* ref	= ele_2->Attribute("ref");
						if (ref) {
							m_ScuSOPClasses.AddTransferSyntaxForSOP(ref_id, ref);
						}
					}
				}
			} else if(0 == strcmp("include-storage-sop-classes", ele_1->Value())){
				for (SOPClassMap::iterator it = m_StorageSOPClasses.begin(); it != m_StorageSOPClasses.end(); ++it) {
					const std::string& storageSOPRef = (*it).first;
					SOPClassTuple& storageSOPTuple = (*it).second;
					m_ScuSOPClasses[storageSOPRef] = storageSOPTuple;
				}
			}
		}
	}

	sub_xml = pRoot->FirstChildElement("scp-sop-classes");
	if (sub_xml) {
		// Loading SCP SOP Classes
		XMLElement* ele_1 = sub_xml->FirstChildElement();
		for ( ; ele_1 != NULL; ele_1 = ele_1->NextSiblingElement()) {
			if (0 == strcmp("sop-class", ele_1->Value())) {
				const char* ref_id	= ele_1->Attribute("ref-id");
				const char* uid		= ele_1->Attribute("uid");	
				if (ref_id && uid) {
					m_ScpSOPClasses.AddSOPClass(ref_id, uid);
					XMLElement* ele_2 = ele_1->FirstChildElement();
					for ( ; ele_2 != NULL; ele_2 = ele_2->NextSiblingElement()) {
						const char* ref	= ele_2->Attribute("ref");
						if (ref) {
							m_ScpSOPClasses.AddTransferSyntaxForSOP(ref_id, ref);
						}
					}
				}
			} else if(0 == strcmp("include-storage-sop-classes", ele_1->Value())){
				for (SOPClassMap::iterator it = m_StorageSOPClasses.begin(); it != m_StorageSOPClasses.end(); ++it) {
					const std::string& storageSOPRef = (*it).first;
					SOPClassTuple& storageSOPTuple = (*it).second;
					m_ScpSOPClasses[storageSOPRef] = storageSOPTuple;
				}
			}
		}
	}

	sub_xml = pRoot->FirstChildElement("modalities-map");
	if (sub_xml) {
		// Loading Modalities
		XMLElement* ele_1 = sub_xml->FirstChildElement();
		for ( ; ele_1 != NULL; ele_1 = ele_1->NextSiblingElement()) {
			const char* ref_id	= ele_1->Attribute("ref-id");
			const char* descr		= ele_1->Attribute("descr");	
			if (ref_id && descr) {
				m_Modalities.AddModality(ref_id, descr);
				XMLElement* ele_2 = ele_1->FirstChildElement();
				for ( ; ele_2 != NULL; ele_2 = ele_2->NextSiblingElement()) {
					const char* ref	= ele_2->Attribute("ref");
					if (ref) {
						m_Modalities.AddSOPClassForModality(ref_id, ref);
					}
				}
			}
		}
	}
	return;
}

const GIL::DICOM::TransferSyntaxMap& GIL::DICOM::Conformance::GetTransferSyntaxes() {
	if (!m_Loaded) {
		Load();
	}
	return m_TransferSyntaxes;
}

const GIL::DICOM::SOPClassMap& GIL::DICOM::Conformance::GetStorageSOPClasses() {
	if (!m_Loaded) {
		Load();
	}
	return m_StorageSOPClasses;
}

const GIL::DICOM::SOPClassMap& GIL::DICOM::Conformance::GetScuSOPClasses() {
	if (!m_Loaded) {
		Load();
	}
	return m_ScuSOPClasses;
}


const GIL::DICOM::SOPClassMap& GIL::DICOM::Conformance::GetScpSOPClasses() {
	if (!m_Loaded) {
		Load();
	}
	return m_ScpSOPClasses;
}

const GIL::DICOM::ModalityMap& GIL::DICOM::Conformance::GetModalities() {
	if (!m_Loaded) {
		Load();
	}
	return m_Modalities;
}


//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

GIL::DICOM::ArrayHelper::ArrayHelper()
{
	array = NULL;
	size = 0;
}

GIL::DICOM::ArrayHelper::ArrayHelper(const std::list<std::string>& list)
{	
	array = NULL;
	size = 0;
	copyFrom(list);
}

GIL::DICOM::ArrayHelper::ArrayHelper(const ArrayHelper& o)
{
	array = NULL;
	size = 0;
	*this = o;
}

GIL::DICOM::ArrayHelper& GIL::DICOM::ArrayHelper::operator=(const GIL::DICOM::ArrayHelper& o)
{
	this->free();
	this->size = o.size;
	this->array = new const char* [this->size];
	for (unsigned int i = 0; i < this->size; i++) {
		unsigned int len = strlen(o.array[i]);
		
		char* cpstr = new char[len + 1];
		unsigned int j = 0;
		for (; j < len; j++) {
			cpstr[j] = o.array[i][j];
		}
		cpstr[j] = 0;
		this->array[i] = cpstr;
	}

	return *this;

}

GIL::DICOM::ArrayHelper& GIL::DICOM::ArrayHelper::operator=(const std::list<std::string>& list)
{
	copyFrom(list);
	return *this;
}

void GIL::DICOM::ArrayHelper::copyFrom(const std::list<std::string>& list)
{
	free();
	this->size = list.size();
	this->array = new const char* [this->size];
	unsigned int i = 0;
	for (std::list<std::string>::const_iterator it = list.begin(); it != list.end(); ++it) {
		const std::string& str = (*it);
		char* cpstr = new char[str.size() + 1];
		unsigned int j = 0;
		for (std::string::const_iterator its = str.begin(); its != str.end(); ++its) {
			cpstr[j++] = (*its);
		}
		cpstr[j] = 0;
		this->array[i++] = cpstr;
	}
}
void GIL::DICOM::ArrayHelper::free() {
	if (this->size != 0) {
		for (unsigned int i = 0; i < this->size; i++) {
			delete[] this->array[i];
		}
		this->size = 0;
	}
	if (this->array != NULL) {
		delete[] this->array;
		this->array = NULL;
	}					
}

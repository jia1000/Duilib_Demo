/*
*  
*  $Id: ConfigController.cpp $
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
#include <StdAfx.h>

#include "configcontroller.h"

#include "utility_tool/inifile.h"


ConfigController* ConfigController::m_pInstance = NULL;
IniFile* ConfigController::m_ini_file = NULL;

ConfigController::ConfigController()
{

}

ConfigController::~ConfigController()
{

}
ConfigController* ConfigController::Instance()
{
	if (m_pInstance == NULL) {
		m_pInstance = new ConfigController();
		m_ini_file = new IniFile();
	}
	return m_pInstance;
}
void ConfigController::FreeInstance()
{
	if (m_pInstance != NULL) {
		delete m_pInstance;
		m_pInstance = NULL;

		delete m_ini_file;
		m_ini_file = NULL;
	}			
}

void ConfigController::ReloadFile(const std::string& file_name)
{
	if (m_ini_file) {
		m_ini_file->Load(file_name);
	}
}

void ConfigController::SaveFile()
{
	if (m_ini_file) {
		m_ini_file->Save();
	}
}

void  ConfigController::SetAETNumber(const std::string& number)
{
	if (m_ini_file) {
		m_ini_file->SetStringValue(SECTION_PACS_SERVICE, "Number", number);
	}
}
void  ConfigController::SetAETTitle(const std::string& aet_title)
{
	if (m_ini_file) {
		m_ini_file->SetStringValue(SECTION_PACS_SERVICE, "AetTitle", aet_title);
	}
}
void  ConfigController::SetAETHost(const std::string& ip_addr)
{
	if (m_ini_file) {
		m_ini_file->SetStringValue(SECTION_PACS_SERVICE, "Host", ip_addr);
	}
}
void  ConfigController::SetAETPort(const std::string& port)
{
	if (m_ini_file) {
		m_ini_file->SetStringValue(SECTION_PACS_SERVICE, "Port", port);
	}
}

void ConfigController::SetFilterDate(ST_Filter_Condition condition)
{
	if (m_ini_file) {
		//m_ini_file->SetStringValue(SECTION_FILTER_DATE, KEY_FILTER_NAME,		condition.name);
		//m_ini_file->SetStringValue(SECTION_FILTER_DATE, KEY_FILTER_DCMTK_TAG,	condition.tag);
		m_ini_file->SetBoolValue  (SECTION_FILTER_DATE,	KEY_FILTER_CHECKED,		condition.is_checked);
		m_ini_file->SetStringValue(SECTION_FILTER_DATE, KEY_FILTER_CONDITION,	condition.condition);
	}
}
void ConfigController::SetFilterBodyPart(ST_Filter_Condition condition)
{
	if (m_ini_file) {
		//m_ini_file->SetStringValue(SECTION_FILTER_BODY_PART, KEY_FILTER_NAME,		condition.name);
		//m_ini_file->SetStringValue(SECTION_FILTER_BODY_PART, KEY_FILTER_DCMTK_TAG,	condition.tag);
		m_ini_file->SetBoolValue  (SECTION_FILTER_BODY_PART, KEY_FILTER_CHECKED,	condition.is_checked);
		m_ini_file->SetStringValue(SECTION_FILTER_BODY_PART, KEY_FILTER_CONDITION,	condition.condition);
	}
}
void ConfigController::SetFilterThickness(ST_Filter_Condition condition)
{
	if (m_ini_file) {
		//m_ini_file->SetStringValue(SECTION_FILTER_THICKNESS, KEY_FILTER_NAME,		condition.name);
		//m_ini_file->SetStringValue(SECTION_FILTER_THICKNESS, KEY_FILTER_DCMTK_TAG,	condition.tag);
		m_ini_file->SetBoolValue  (SECTION_FILTER_THICKNESS, KEY_FILTER_CHECKED,	condition.is_checked);
		m_ini_file->SetStringValue(SECTION_FILTER_THICKNESS, KEY_FILTER_CONDITION,	condition.condition);
	}
}
void ConfigController::SetFilterModality(ST_Filter_Condition condition)
{
	if (m_ini_file) {
		//m_ini_file->SetStringValue(SECTION_FILTER_MODALITY, KEY_FILTER_NAME,		condition.name);
		//m_ini_file->SetStringValue(SECTION_FILTER_MODALITY, KEY_FILTER_DCMTK_TAG,	condition.tag);
		m_ini_file->SetBoolValue  (SECTION_FILTER_MODALITY,	KEY_FILTER_CHECKED,		condition.is_checked);
		m_ini_file->SetStringValue(SECTION_FILTER_MODALITY, KEY_FILTER_CONDITION,	condition.condition);
	}
}

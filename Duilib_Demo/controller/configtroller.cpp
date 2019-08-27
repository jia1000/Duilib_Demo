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
#include "utility_tool/string_converse.h"

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
		m_ini_file->Load("G:\\temp1\\test.ini");
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

void  ConfigController::SetAETNumber(const std::wstring& number)
{
	if (m_ini_file) {
		std::string s = toString(number);
		m_ini_file->SetStringValue(SECTION_PACS_SERVICE, "Number", s);
	}
}
void  ConfigController::SetAETTitle(const std::wstring& aet_title)
{
	if (m_ini_file) {
		std::string s = toString(aet_title);
		m_ini_file->SetStringValue(SECTION_PACS_SERVICE, "AetTitle", s);
	}
}
void  ConfigController::SetAETHost(const std::wstring& ip_addr)
{
	if (m_ini_file) {
		std::string s = toString(ip_addr);
		m_ini_file->SetStringValue(SECTION_PACS_SERVICE, "Host", s);
	}
}
void  ConfigController::SetAETPort(const std::wstring& port)
{
	if (m_ini_file) {
		std::string s = toString(port);
		m_ini_file->SetStringValue(SECTION_PACS_SERVICE, "Port", s);
	}
}
void  ConfigController::SetAETPduLength(const std::wstring& pdu)
{
	if (m_ini_file) {
		std::string s = toString(pdu);
		m_ini_file->SetStringValue(SECTION_PACS_SERVICE, "Pdu", s);
	}
}
std::wstring ConfigController::GetAETNumber()
{
	if (m_ini_file) {
		std::string number;
		m_ini_file->GetStringValue(SECTION_PACS_SERVICE, "Number", &number);
		return toWString(number);
	}
}
std::wstring ConfigController::GetAETTitle()
{
	if (m_ini_file) {
		std::string aet_title;
		m_ini_file->GetStringValue(SECTION_PACS_SERVICE, "AetTitle", &aet_title);
		return toWString(aet_title);
	}
}
std::wstring ConfigController::GetAETHost()
{
	if (m_ini_file) {
		std::string ip_addr;
		m_ini_file->GetStringValue(SECTION_PACS_SERVICE, "Host", &ip_addr);
		return toWString(ip_addr);
	}
}
std::wstring ConfigController::GetAETPort()
{
	if (m_ini_file) {
		std::string port;
		m_ini_file->GetStringValue(SECTION_PACS_SERVICE, "Port", &port);
		return toWString(port);
	}
}
std::wstring ConfigController::GetAETPdu()
{
	if (m_ini_file) {
		std::string pdu;
		m_ini_file->GetStringValue(SECTION_PACS_SERVICE, "Pdu", &pdu);
		return toWString(pdu);
	}
}

void ConfigController::SetFilterDate(ST_Filter_Condition condition)
{
	if (m_ini_file) {
		//m_ini_file->SetStringValue(SECTION_FILTER_DATE, KEY_FILTER_NAME,		condition.name);
		//m_ini_file->SetStringValue(SECTION_FILTER_DATE, KEY_FILTER_DCMTK_TAG,	condition.tag);
		m_ini_file->SetBoolValue  (SECTION_FILTER_DATE,	KEY_FILTER_CHECKED,		condition.is_checked);
		m_ini_file->SetStringValue(SECTION_FILTER_DATE, KEY_FILTER_CONDITION,	condition.condition_text);
	}
}
void ConfigController::SetFilterBodyPart(ST_Filter_Condition condition)
{
	if (m_ini_file) {
		//m_ini_file->SetStringValue(SECTION_FILTER_BODY_PART, KEY_FILTER_NAME,		condition.name);
		//m_ini_file->SetStringValue(SECTION_FILTER_BODY_PART, KEY_FILTER_DCMTK_TAG,	condition.tag);
		m_ini_file->SetBoolValue  (SECTION_FILTER_BODY_PART, KEY_FILTER_CHECKED,	condition.is_checked);
		m_ini_file->SetStringValue(SECTION_FILTER_BODY_PART, KEY_FILTER_CONDITION,	condition.condition_text);
	}
}
void ConfigController::SetFilterThickness(ST_Filter_Condition condition)
{
	if (m_ini_file) {
		//m_ini_file->SetStringValue(SECTION_FILTER_THICKNESS, KEY_FILTER_NAME,		condition.name);
		//m_ini_file->SetStringValue(SECTION_FILTER_THICKNESS, KEY_FILTER_DCMTK_TAG,	condition.tag);
		m_ini_file->SetBoolValue  (SECTION_FILTER_THICKNESS, KEY_FILTER_CHECKED,	condition.is_checked);
		m_ini_file->SetStringValue(SECTION_FILTER_THICKNESS, KEY_FILTER_CONDITION,	condition.condition_text);
	}
}
void ConfigController::SetFilterModality(ST_Filter_Condition condition)
{
	if (m_ini_file) {
		//m_ini_file->SetStringValue(SECTION_FILTER_MODALITY, KEY_FILTER_NAME,		condition.name);
		//m_ini_file->SetStringValue(SECTION_FILTER_MODALITY, KEY_FILTER_DCMTK_TAG,	condition.tag);
		m_ini_file->SetBoolValue  (SECTION_FILTER_MODALITY,	KEY_FILTER_CHECKED,		condition.is_checked);
		m_ini_file->SetStringValue(SECTION_FILTER_MODALITY, KEY_FILTER_CONDITION,	condition.condition_text);
	}
}

void ConfigController::GetFilterDate(ST_Filter_Condition& condition)
{
	if (m_ini_file) {
		m_ini_file->GetIntValue  (SECTION_FILTER_DATE,	KEY_FILTER_CHECKED,		&condition.is_checked);
		m_ini_file->GetStringValue(SECTION_FILTER_DATE, KEY_FILTER_CONDITION,	&condition.condition_text);
	}
}
void ConfigController::GetFilterBodyPart(ST_Filter_Condition& condition)
{
	if (m_ini_file) {
		m_ini_file->GetIntValue  (SECTION_FILTER_BODY_PART, KEY_FILTER_CHECKED,	&condition.is_checked);
		m_ini_file->GetStringValue(SECTION_FILTER_BODY_PART, KEY_FILTER_CONDITION,	&condition.condition_text);
	}
}
void ConfigController::GetFilterThickness(ST_Filter_Condition& condition)
{
	if (m_ini_file) {
		m_ini_file->GetIntValue  (SECTION_FILTER_THICKNESS, KEY_FILTER_CHECKED,	&condition.is_checked);
		m_ini_file->GetStringValue(SECTION_FILTER_THICKNESS, KEY_FILTER_CONDITION,	&condition.condition_text);
	}
}
void ConfigController::GetFilterModality(ST_Filter_Condition& condition)
{
	if (m_ini_file) {
		m_ini_file->GetIntValue  (SECTION_FILTER_MODALITY,	KEY_FILTER_CHECKED,		&condition.is_checked);
		m_ini_file->GetStringValue(SECTION_FILTER_MODALITY, KEY_FILTER_CONDITION,	&condition.condition_text);
	}
}
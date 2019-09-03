/*
 *  
 *  $Id: internationalization.cpp $
 *  DeepWise Dicom Viewer
 *
 *  Copyright 2017-2030 DeepWise All rights reserved.
 *  http://www.deepwise.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#include <string>

#include <api/internationalization/internationalization.h>

//#include <wx/intl.h>

std::string EXTAPI _Std(const char* s) {
	//wxString wxStr = wxString::FromUTF8(s);
	//wxStr = wxGetTranslation(wxStr);
	//return std::string(wxStr.ToUTF8());
	return std::string(s);
}
//wxString EXTAPI _StdWx(const char* s) {
//    wxString wxStr = wxString::FromUTF8(s);
//    return wxGetTranslation(wxStr);
//}



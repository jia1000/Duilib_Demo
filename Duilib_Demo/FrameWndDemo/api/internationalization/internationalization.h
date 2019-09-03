/*
 *  
 *  $Id: internationalization.h $
 *  DeepWise Dicom Viewer
 *
 *  Copyright 2017-2030 DeepWise All rights reserved.
 *  http://www.deepwise.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#pragma once

#include <string>

#include <api/api.h>

/** Translates the string to current language **/
std::string EXTAPI _Std(const char* s);
//wxString EXTAPI _StdWx(const char* s);

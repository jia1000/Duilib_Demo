#pragma once

#include <locale>
#include <codecvt>
#include <string>

//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////



std::string toString(const wchar_t* _src);
std::string toString(const std::wstring& _src) ;

std::wstring toWString(const char* _src);
std::wstring toWString(const std::string& _src);

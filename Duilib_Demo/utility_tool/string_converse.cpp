#include "string_converse.h"



	//The first and second param is used for return value when convert string and wstring faild.
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> utfStringConverter(std::string(""), std::wstring(L""));

	//name .936 is for gbk name
	//auto gbkCvt = new std::codecvt_byname<wchar_t, char, std::mbstate_t>(".936");
	//std::wstring_convert<std::codecvt_byname<wchar_t, char, std::mbstate_t>> gbkStringConverter(gbkCvt);

	//auto gbkCvt = new std::codecvt<wchar_t, char, std::mbstate_t>("CHS");
	//std::wstring_convert<std::codecvt<wchar_t, char, std::mbstate_t>> gbkStringConverter(gbkCvt);

	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	//std::string string = converter.to_bytes(L"这是一个宽字符串");



std::string toString(const wchar_t* _src) 
{
	//auto ret = utfStringConverter.from_bytes(_src);
	//if (ret.empty()) {
	//	return gbkStringConverter.from_bytes(_src);
	//} else {
	//	return ret;
	//}

	std::string ret = converter.to_bytes(_src);

	return ret;
}

std::string toString(const std::wstring& _src) 
{
	return toString(_src.c_str());
}

std::wstring toWString(const char* _src) {
	//auto ret = utfStringConverter.to_bytes(_src);
	//if (ret.empty()) {
	//	return gbkStringConverter.to_bytes(_src);
	//} else {
	//	return ret;
	//}

	std::wstring ret ;
	ret = converter.from_bytes(_src);

	return ret;
}

std::wstring toWString(const std::string& _src) {
	return toWString(_src.c_str());
}
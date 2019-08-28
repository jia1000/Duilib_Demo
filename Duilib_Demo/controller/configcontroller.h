#pragma once

typedef struct ST_Filter_Condition {
	//std::string name;
	//std::string tag;
	int is_checked;
	std::string condition_text;
};

#define SECTION_PACS_SERVICE		"PACSService"
#define SECTION_FILTER_DATE			"FilterDate"
#define SECTION_FILTER_BODY_PART	"FilterBodyPart"
#define SECTION_FILTER_THICKNESS	"FilterThickness"
#define SECTION_FILTER_MODALITY		"FilterModality"

//#define KEY_FILTER_INDEX			"index"
#define KEY_FILTER_NAME				"name"
#define KEY_FILTER_DCMTK_TAG		"tag"
#define KEY_FILTER_CHECKED			"ischecked"
#define KEY_FILTER_CONDITION		"condition"

using namespace std;
class IniFile;

class ConfigController{
public:
	static ConfigController* Instance();
	static void FreeInstance();

	void ReloadFile(const std::string& file_name);
	std::wstring GetConfigFile();
	void SaveFile();

	void SetAETNumber(const std::wstring& number);
	void SetAETTitle(const std::wstring& aet_title);
	void SetAETHost(const std::wstring& ip_addr);
	void SetAETPort(const std::wstring& port);
	void SetAETPduLength(const std::wstring& pdu);

	std::wstring GetAETNumber();
	std::wstring GetAETTitle();
	std::wstring GetAETHost();
	std::wstring GetAETPort();	
	std::wstring GetAETPdu();

	std::string GetAETNumberStr();
	std::string GetAETTitleStr();
	std::string GetAETHostStr();
	std::string GetAETPortStr();	
	std::string GetAETPduStr();

	void SetFilterDate(ST_Filter_Condition condition);
	void SetFilterBodyPart(ST_Filter_Condition condition);
	void SetFilterThickness(ST_Filter_Condition condition);
	void SetFilterModality(ST_Filter_Condition condition);
	
	void GetFilterDate(ST_Filter_Condition& condition);
	void GetFilterBodyPart(ST_Filter_Condition& condition);
	void GetFilterThickness(ST_Filter_Condition& condition);
	void GetFilterModality(ST_Filter_Condition& condition);

	// Õ®”√µƒ∂¡–¥≈‰÷√
	void GetValue(const string &section, const string &key, string *value, const string &defaultValue);
	void SetValue(const string &section, const string &key, const string &value);

protected:
	ConfigController();
	~ConfigController();

	static ConfigController*     m_pInstance;

	static IniFile* m_ini_file;
};

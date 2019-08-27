#pragma once

typedef struct ST_Filter_Condition {
	std::string name;
	std::string tag;
	bool is_checked;
	std::string condition;
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

class IniFile;

class ConfigController{
public:
	static ConfigController* Instance();
	static void FreeInstance();

	void ReloadFile(const std::string& file_name);
	void SaveFile();

	void SetAETNumber(const std::string& number);
	void SetAETTitle(const std::string& aet_title);
	void SetAETHost(const std::string& ip_addr);
	void SetAETPort(const std::string& port);

	std::string GetConfigFile();
	std::string GetAETNumber();
	std::string GetAETTitle();
	std::string GetAETHost();
	std::string GetAETPort();	

	void SetFilterDate(ST_Filter_Condition condition);
	void SetFilterBodyPart(ST_Filter_Condition condition);
	void SetFilterThickness(ST_Filter_Condition condition);
	void SetFilterModality(ST_Filter_Condition condition);
	
	

	// Õ®”√µƒ∂¡–¥≈‰÷√
	void GetValue(const string &section, const string &key, string *value, const string &defaultValue);
	void SetValue(const string &section, const string &key, const string &value);

protected:
	ConfigController();
	~ConfigController();

	static ConfigController*     m_pInstance;

	static IniFile* m_ini_file;
};

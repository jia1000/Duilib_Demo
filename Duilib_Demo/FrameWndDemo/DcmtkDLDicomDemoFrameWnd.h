#pragma once
#include <UIlib.h>
using namespace DuiLib;

#include "api/dicom/dicomdataset.h"

#include <dcmtk/dcmdata/dcdict.h>
#include <dcmtk/dcmdata/dcdicent.h>
#include <dcmtk/dcmdata/dcdatset.h>

#include "main/controllers/dcmtk/dicomservers.h"

#include "main/controllers/pacscontroller.h"

//////////////////////////////////////////////////////////////////////////
// 该类使用xml文件创建窗口
//////////////////////////////////////////////////////////////////////////

typedef struct SeriesInfo
{
	std::string sereis_id;
};
typedef struct StudyInfo
{
	std::string study_id;
	std::vector<SeriesInfo> series_ids;
};
typedef struct PatientInfo
{
	std::string patiend_id;
	std::vector<StudyInfo> studies_ids;
};

class DcmtkDLDicomDemoFrameWnd : public WindowImplBase
{
public:
	DcmtkDLDicomDemoFrameWnd(void);
	~DcmtkDLDicomDemoFrameWnd(void);

	virtual LPCTSTR GetWindowClassName() const ;
	virtual void OnFinalMessage(HWND hWnd);
	virtual void InitWindow();
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual void    Notify(TNotifyUI& msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void OnSelChanged(CControlUI* pSender);
	void OnOpenPatientIDListFile();
	void OnOpenDownloadPath();

	void DoSearchTest();
	void DoSearchTest2();
	void DoDownloadTest();	

	DcmElement* CrearElementoConValor(const char* s);
	std::vector<std::string> testSplit(std::string srcStr, const std::string& delim);

private:
	std::vector<std::string> m_study_ids;
	std::vector<std::string> m_patient_ids; // 不是edit控件是行显示的patientid
	
	CEditUI* m_pPatientCsvPathEdit;
	CEditUI* m_pPatientIdEdit;
	CEditUI* m_pBodyPartEdit;
	CEditUI* m_pThicknessEdit;
	CEditUI* m_pMOdalityiesInStudyEdit;
	CEditUI* m_pDownloadPathEdit;

	CLabelUI* m_pFindResultLabel;

	std::map<std::string, std::vector<StudyInfo>> m_patient_info; // first patientid, second study
};


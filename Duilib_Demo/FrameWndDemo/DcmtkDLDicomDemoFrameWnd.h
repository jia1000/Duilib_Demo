#pragma once
#include <UIlib.h>
using namespace DuiLib;

#include "api/dicom/dicomdataset.h"

#include <dcmtk/dcmdata/dcdict.h>
#include <dcmtk/dcmdata/dcdicent.h>
#include <dcmtk/dcmdata/dcdatset.h>

#include "main/controllers/dcmtk/dicomservers.h"


//////////////////////////////////////////////////////////////////////////
// 该类使用xml文件创建窗口
//////////////////////////////////////////////////////////////////////////

typedef enum TRetrieveMethod {
	MOVE = 0,
	GET,
	WADO
}TRetrieveMethod;

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

	void DoSearchTest();
	void DoDownloadTest();

	virtual void  GetConnection(void* connectionKey);
	virtual void  ReleaseConnection(void* connectionKey);

	virtual bool ObtenerEstudio(
		void* connectionKey,
		const std::string& serverId,
		const GIL::DICOM::DicomDataset& base,
		//IModeloDicom* pModelo,
		//GNC::IProxyNotificadorProgreso* pNotificador,
		bool link
		);

	virtual bool ObtenerSerie(
		void* connectionKey,
		const std::string& serverId,
		const GIL::DICOM::DicomDataset& base,
		//IModeloDicom* pModelo,
		//GNC::IProxyNotificadorProgreso* pNotificador,
		bool link
		);

	virtual bool ObtenerImagen(
		void* connectionKey,
		const std::string& serverId,
		const GIL::DICOM::DicomDataset& base//,
		//IModeloDicom* pModelo,
		//GNC::IProxyNotificadorProgreso* pNotificador
		);

	void FillInQuery(const GIL::DICOM::DicomDataset& base, DcmDataset* query);//, const GNC::GCS::Ptr<DicomServer>& server);
	DcmElement* CrearElementoConValor(const char* s);
	std::vector<std::string> testSplit(std::string srcStr, const std::string& delim);

private:
	GNC::GCS::Ptr<DicomServer> server;
	TRetrieveMethod retrieve_method; 
	std::vector<std::string> m_study_ids;

	CEditUI* m_pPatientIdEdit;
	CLabelUI* m_pFindResultLabel;
};


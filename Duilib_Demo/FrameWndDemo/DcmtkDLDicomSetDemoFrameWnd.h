#pragma once
#include <UIlib.h>
using namespace DuiLib;

//////////////////////////////////////////////////////////////////////////
// 该类使用xml文件创建窗口
//////////////////////////////////////////////////////////////////////////

class DcmtkDLDicomSetDemoFrameWnd : public WindowImplBase
{
public:
	DcmtkDLDicomSetDemoFrameWnd(void);
	~DcmtkDLDicomSetDemoFrameWnd(void);

	virtual LPCTSTR GetWindowClassName() const ;
	virtual void OnFinalMessage(HWND hWnd);
	virtual void InitWindow();
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual void    Notify(TNotifyUI& msg);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void DoConnectTest();
	void DoSaveConfigTest();

private:
	CEditUI* m_pNumberEdit;
	CEditUI* m_pAetEdit;
	CEditUI* m_pHostEdit;
	CEditUI* m_pDicomPortEdit;
	CEditUI* m_pDicomPdutEdit;
	//CSpinnerUI* m_pPduSpinner;
	CComboUI* m_pSerchMethodCmb;
	CComboUI* m_pSerchModeCmb;
	CLabelUI* m_pLabelStatus;

};


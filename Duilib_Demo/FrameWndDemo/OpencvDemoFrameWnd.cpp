#include "OpencvDemoFrameWnd.h"

//C++输入输出库头文件
#include "opencv2/core/core.hpp"    
#include "opencv2/highgui/highgui.hpp"  
#include "opencv2/imgproc/imgproc.hpp" 
#include "opencv2/opencv.hpp"
#include <iostream> 


COpencvDemoFrameWnd::COpencvDemoFrameWnd(void)
{
}


COpencvDemoFrameWnd::~COpencvDemoFrameWnd(void)
{
}

LPCTSTR COpencvDemoFrameWnd::GetWindowClassName() const 
{ 
	return _T("OpencvDemoFrameWnd"); 
}

void COpencvDemoFrameWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	//delete this; //退出时，会有异常
}

CDuiString COpencvDemoFrameWnd::GetSkinFile()
{
	return _T("frame_wnd_demo\\opencv_demo_frame.xml");
}

CDuiString COpencvDemoFrameWnd::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");

}

CControlUI* COpencvDemoFrameWnd::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void COpencvDemoFrameWnd::InitWindow()
{
	
}

LRESULT COpencvDemoFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	__super::OnSize(uMsg, wParam, lParam, bHandled);
	return 0;
}


void    COpencvDemoFrameWnd::Notify(TNotifyUI& msg) 
{
	if (msg.sType == _T("click")) {
		LPCTSTR pszCtrlName = msg.pSender->GetName().GetData();
		if (_tcscmp(pszCtrlName, _T("btn_sysmin")) == 0) {
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		} else if (_tcscmp(pszCtrlName, _T("btn_sysmax")) == 0) {
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
			// 最大化和还原2个button之间的切换
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_sysmax")));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_restore")));
			if( pControl ) pControl->SetVisible(true);
		} else if (_tcscmp(pszCtrlName, _T("btn_restore")) == 0) {
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
			// 最大化和还原2个button之间的切换
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_sysmax")));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("btn_restore")));
			if( pControl ) pControl->SetVisible(false);
		} else if (_tcscmp(pszCtrlName, _T("btn_sysclose")) == 0) {
			Close(IDOK);
		} else if (_tcscmp(pszCtrlName, _T("btn_test")) == 0) {
			TestOpencv();
		}
	}
}

LRESULT COpencvDemoFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg, wParam, lParam);
}

void COpencvDemoFrameWnd::TestOpencv()
{
	cv::Mat src = cv::imread(".\\21.png");

	// 缩放图像
	double scale = 0.5;
	cv::Mat dst;
	cv::resize(src, dst, cv::Size(), scale, scale);
	cv::imwrite(".\\haha.png", dst);
}
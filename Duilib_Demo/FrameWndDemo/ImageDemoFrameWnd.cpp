#include "ImageDemoFrameWnd.h"
#include <windows.h>



CImageDemoFrameWnd::CImageDemoFrameWnd(void)
	: m_pImageControl(NULL)
	, m_move_step(10)
	, file_index(0)
{
	m_vFiles.push_back(_T("image\\bk1.png"));
	m_vFiles.push_back(_T("image\\bk2.png"));

	m_file_name = m_vFiles[file_index];
}


CImageDemoFrameWnd::~CImageDemoFrameWnd(void)
{
}

LPCTSTR CImageDemoFrameWnd::GetWindowClassName() const 
{ 
	return _T("VtkDemoFrameWnd"); 
}

void CImageDemoFrameWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	//delete this; //退出时，会有异常
}

CDuiString CImageDemoFrameWnd::GetSkinFile()
{
	return _T("frame_wnd_demo\\image_frame_demo.xml");
}

CDuiString CImageDemoFrameWnd::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");

}

CControlUI* CImageDemoFrameWnd::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CImageDemoFrameWnd::InitWindow()
{
	CButtonUI* pControl = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_image_show")));
	if (pControl) {
		CDuiString name = m_file_name.c_str();
		pControl->SetBkImage(name);	
		m_pImageControl = pControl;
	}
	CEditUI* pEditControl = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_move_step")));
	if (pEditControl) {
		CDuiString text;
		text.Format(_T("%d"), m_move_step);
		pEditControl->SetText(text);
	}

}

LRESULT CImageDemoFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	__super::OnSize(uMsg, wParam, lParam, bHandled);
	return 0;
}


void    CImageDemoFrameWnd::Notify(TNotifyUI& msg) 
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
		} else if (_tcscmp(pszCtrlName, _T("btn_image_show")) == 0) {
			CButtonUI* pControl = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn_image_show")));
			if (pControl) {
				RECT rc = pControl->GetPos();
				m_image_rect_info.x_center = (rc.right - rc.left) / 2 ;
				m_image_rect_info.y_center = (rc.bottom - rc.top) / 2;
				m_image_rect_info.height = 300;
				m_image_rect_info.widht = 240;
				
				if (m_vFiles.size() > 0) {
					if (file_index == m_vFiles.size()) {
						file_index = 0;
					} else if (file_index == 0) {
						file_index = m_vFiles.size() -1;
					}
					m_file_name = m_vFiles[file_index];					
					UpdateImageControl();
					file_index++;
				}
			}
		} else if (_tcscmp(pszCtrlName, _T("btn_image_magnify")) == 0) {
			if (m_pImageControl) {;
				ModifyImageWidth(m_move_step);
				UpdateImageControl();
			}
		} else if (_tcscmp(pszCtrlName, _T("btn_image_reduce")) == 0) {
			if (m_pImageControl) {
				ModifyImageWidth(-1 * m_move_step);
				UpdateImageControl();
			}
		} else if (_tcscmp(pszCtrlName, _T("btn_image_up")) == 0) {
			if (m_pImageControl) {
				ModifyImageCenter(0, -1 * m_move_step);
				UpdateImageControl();
			}
		} else if (_tcscmp(pszCtrlName, _T("btn_image_down")) == 0) {
			if (m_pImageControl) {
				ModifyImageCenter(0, m_move_step);
				UpdateImageControl();
			}
		} else if (_tcscmp(pszCtrlName, _T("btn_image_left")) == 0) {
			if (m_pImageControl) {
				ModifyImageCenter(-1 * m_move_step, 0);
				UpdateImageControl();
			}
		} else if (_tcscmp(pszCtrlName, _T("btn_image_right")) == 0) {
			if (m_pImageControl) {
				ModifyImageCenter(m_move_step, 0);
				UpdateImageControl();
			}
		}
	} else if (msg.sType == DUI_MSGTYPE_TEXTCHANGED) {
		CEditUI* pControl = static_cast<CEditUI*>(m_pm.FindControl(_T("edit_move_step")));
		if (pControl) {
			CDuiString text = pControl->GetText();
			std::wstring ws = text;
			int step = wcstol(ws.c_str(), NULL, 10);
			m_move_step = step;
		}
	} 
}

LRESULT CImageDemoFrameWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg, wParam, lParam);
}

void CImageDemoFrameWnd::UpdateImageControl()
{
	RECT rc_new = GetImageRect();
	CDuiString image_path;
	image_path.Format(_T("file='%s' dest='%d,%d,%d,%d' "), m_file_name.c_str(),
		rc_new.left, rc_new.top, rc_new.right, rc_new.bottom);

	if (m_pImageControl) {
		m_pImageControl->SetBkImage(image_path);
	}
}

RECT CImageDemoFrameWnd::GetImageRect()
{
	RECT rc;
	rc.left = m_image_rect_info.x_center - m_image_rect_info.widht / 2;
	rc.right = m_image_rect_info.x_center + m_image_rect_info.widht / 2;
	rc.top = m_image_rect_info.y_center - m_image_rect_info.height / 2;
	rc.bottom = m_image_rect_info.y_center + m_image_rect_info.height / 2;

	return rc;
}

void CImageDemoFrameWnd::ModifyImageWidth(int delta)
{
	m_image_rect_info.widht += delta;
	m_image_rect_info.height += delta;
}

void CImageDemoFrameWnd::ModifyImageCenter(int x_delta, int y_delta)
{
	m_image_rect_info.x_center += x_delta;
	m_image_rect_info.y_center += y_delta;
}

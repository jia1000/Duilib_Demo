#include "pacs_download_right_menu.h"

#include "FrameWndDemo\DcmtkDLDicomDemoFrameWnd.h"

#include <windows.h>

PacsDownloadRightMenu::PacsDownloadRightMenu(WindowImplBase* parent, TNotifyUI& msg) 
	: m_pPacsSearchWnd(parent)
	, m_curRClickMsg(msg) 
{

}

PacsDownloadRightMenu::~PacsDownloadRightMenu() { }

LRESULT PacsDownloadRightMenu::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    return __super::HandleMessage(uMsg, wParam, lParam);
}

void PacsDownloadRightMenu::Notify(TNotifyUI& msg)
{
    if (_tcsicmp(msg.sType, _T("itemclick")) == 0)
    {
        if( msg.pSender->GetName() == _T("right_menu_download") ) {
			DcmtkDLDicomDemoFrameWnd* pwnd = (DcmtkDLDicomDemoFrameWnd*) m_pPacsSearchWnd;
			if (pwnd) {
				pwnd->OnDownloadSelected();
			}
        } else if( msg.pSender->GetName() == _T("right_menu_property") ) {
        }
    }
}



bool PacsDownloadRightMenu::ShowCustomMenu(const long x, const long y)
{
    CDuiPoint point;
    ::GetCursorPos(&point);

    SIZE szWnd = this->m_pm.GetInitSize();
    POINT pt_wnd = {x, y};
    ::ClientToScreen(m_pPacsSearchWnd->GetHWND(), &pt_wnd);

    std::wstring xml_file_mame = _T("rightmenu/pacs_search_right_menu.xml");
    Init(NULL, xml_file_mame.c_str(), pt_wnd, &m_pm);

    return false;
}

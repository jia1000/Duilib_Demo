#ifndef PACS_SEARCH_RIGHT_MENU_H
#define PACS_SEARCH_RIGHT_MENU_H

#include <UIlib.h>
using namespace DuiLib;

//#include "dui/StdAfx.h"
//#include "dui/Core/UIDefine.h"
//#include "main/pacs_serch/pacs_wnd.h"
using namespace DuiLib;

class PacsDownloadRightMenu : public CMenuWnd
{
public:
    PacsDownloadRightMenu(WindowImplBase* parent, TNotifyUI& msg);
	~PacsDownloadRightMenu();

public:
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    bool ShowCustomMenu(const long x, const long y);
	void Notify(TNotifyUI& msg);    
    
private:
    WindowImplBase* m_pPacsSearchWnd;
    TNotifyUI m_curRClickMsg;
};

#endif // PACS_SEARCH_RIGHT_MENU_H
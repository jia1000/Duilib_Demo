#ifndef _DEEPWISE_EXTENDED_CONTROL_LIST_PRO_H_
#define _DEEPWISE_EXTENDED_CONTROL_LIST_PRO_H_
#pragma once

#include <UIlib.h>
using namespace DuiLib;

class ListPro :public CListUI
{
public:
    ListPro();

	void DoEvent(DuiLib::TEventUI& event);
	//void SelectNextVisibleItem();
	//void SelectLastVisibleItem();


};

#endif //_DEEPWISE_EXTENDED_CONTROL_LIST_PRO_H_
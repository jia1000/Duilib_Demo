#include "list_pro_ui.h"

#include <string>



ListPro::ListPro()
{
}


void ListPro::DoEvent(DuiLib::TEventUI& event)
{
	switch( event.Type ) {
	case DuiLib::UIEVENT_KEYDOWN:
		switch( event.chKey ) {
		case VK_UP:
			{
				//SelectLastVisibleItem();
				if (m_aSelItems.GetSize() > 0) {					
					int index = GetMinSelItemIndex() - 1;
					UnSelectAllItems();
					index > 0 ? SelectItem(index, true) : SelectItem(0, true);					
				}
			}			
			return;
		case VK_DOWN:			
			{
				//SelectNextVisibleItem();
				if (m_aSelItems.GetSize() > 0) {					
					int index = GetMaxSelItemIndex() + 1;
					UnSelectAllItems();
					index + 1 > m_pList->GetCount() ? SelectItem(GetCount() - 1, true) : SelectItem(index, true);					
				}
			}
			return;
		//case VK_LEFT:
		//	{
		//		ItemExpansion(false);
		//	}
		//	return;
		//case VK_RIGHT:
		//	{
		//		ItemExpansion(true);
		//	}
		//	return;

		}
		break;
	}
	__super::DoEvent(event);
}


//void ListPro::SelectNextVisibleItem()
//{
//	int index = m_iCurSel+1;
//	if (index < 0) {
//		SelectItem(0);
//	} else if (index >= m_list_body->m_show_list_data.size()) {
//		int list_count = m_list_body->m_show_list_data.size() - 1;
//		if (m_iCurSel == list_count) {
//			return;
//		}
//		SelectItem(m_list_body->m_show_list_data.size() -1);
//	} else {
//		SelectItem(index);
//		EnsureVisible(index);
//	}
//}

//void ListPro::SelectLastVisibleItem()
//{
//	int index = m_iCurSel - 1;
//	if (index < 0) {
//		if (m_iCurSel == 0) {
//			return;
//		}
//		SelectItem(0);
//	} else if (index >= m_list_body->m_show_list_data.size()) {
//		SelectItem(m_list_body->m_show_list_data.size() -1);
//	} else {
//		SelectItem(index);
//		EnsureVisible(index);
//	}
//}
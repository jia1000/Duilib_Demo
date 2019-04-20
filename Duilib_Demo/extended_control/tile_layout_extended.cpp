#include "tile_layout_extended.h"

#include "vtk/vtk_window.h"

TileLayoutExtended::TileLayoutExtended()
:m_activation_vtk_windows(NULL)
,m_nRows(1)
,m_nSpecialLayout(0)
{
}

TileLayoutExtended::~TileLayoutExtended()
{
    int i = 0;
    i++;
    RemoveAllVtkWindow();
}

LPCTSTR TileLayoutExtended::GetClass() const
{
    return _T("TileLayoutExtended");
}

int TileLayoutExtended::GetVtkCount() const
{
    return m_vtk_windows.size();
}

DuiVtkWindow* TileLayoutExtended::GetActivationVtkWindow()
{
    return m_activation_vtk_windows;
}

std::list<DuiVtkWindow*> TileLayoutExtended::GetVtkWindowList()
{
    return m_vtk_windows;
}

void TileLayoutExtended::SetActivationVtkWindow(DuiVtkWindow* vtk_window)
{
    m_activation_vtk_windows = vtk_window;
}

bool TileLayoutExtended::AddVtkWindow(DuiVtkWindow* vtk_window)
{
    std::list<DuiVtkWindow*>::iterator it = m_vtk_windows.begin();
    while (it != m_vtk_windows.end()) {
        if (*it == vtk_window){
            return false;
        }
        ++it;
    }
    m_vtk_windows.push_back(vtk_window);
    vtk_window->SetLayoutManager(this);
    if (m_vtk_windows.size() > m_nRows*m_nColumns) {
        if (m_nRows == m_nColumns) {
            m_nColumns++;
        } else {
            m_nRows > m_nColumns ? m_nColumns++ : m_nRows++;
        }
    }
    NeedUpdate();
    SetActivationVtkWindow(vtk_window);
    return true;
}

void TileLayoutExtended::RemoveAllVtkWindow()
{
    std::list<DuiVtkWindow*>::iterator it = m_vtk_windows.begin();
    for (;it!= m_vtk_windows.end();++it) {
        (*it)->SetLayoutManager(NULL);
        (*it)->Close(0);
    }
    m_vtk_windows.clear();
}

bool TileLayoutExtended::RemoveVtkWinodw(int index)
{
    std::list<DuiVtkWindow*>::iterator it = m_vtk_windows.begin();
    for (int i = 0;it!= m_vtk_windows.end(); i++,it++) {
        if (i == index) {
            (*it)->Close(0);
            return true;
        }
    }
    return false;
}

bool TileLayoutExtended::RemoveVtkWinodw(DuiVtkWindow* vtk_window)
{
    std::list<DuiVtkWindow*>::iterator it = m_vtk_windows.begin();
    while (it != m_vtk_windows.end()) {
        if (*it == vtk_window){
            m_vtk_windows.erase(it);
            NeedUpdate();
            return true;
        }
        it++;
    }
    return false;
}


void TileLayoutExtended::AddRow()
{
    m_nRows++;
    NeedUpdate();
}
void TileLayoutExtended::AddColumn()
{
    m_nColumns++;
    NeedUpdate();
}

void TileLayoutExtended::DelRow()
{
    m_nRows--;
    if (m_nRows <= 0) {
        m_nRows = 1;
    }
    NeedUpdate();
}

void TileLayoutExtended::DelColumn()
{
    m_nColumns--;
    if (m_nColumns <= 0) {
        m_nColumns = 1;
    }
    NeedUpdate();
}

int TileLayoutExtended::GetRow() const
{
    return m_nRows;
}

void TileLayoutExtended::SetRow(int nRows)
{
    if( nRows <= 0 ) return;
    m_nRows = nRows;
    NeedUpdate();
}

int TileLayoutExtended::GetSepcialLayout() const
{
    return m_nSpecialLayout;
}
void TileLayoutExtended::SetSepcialLayout(int nSpecialLayout)
{
    if( nSpecialLayout < 0 ) return;
    m_nSpecialLayout = nSpecialLayout;
    NeedUpdate();
}

void TileLayoutExtended::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
    if( _tcsicmp(pstrName, _T("row")) == 0 ) {
        SetRow(_ttoi(pstrValue));
    } else if (_tcsicmp(pstrName, _T("special_layout")) == 0 ) {
        SetSepcialLayout(_ttoi(pstrValue));
    } else {
        __super::SetAttribute(pstrName, pstrValue);
    }
}

void TileLayoutExtended::SetSpecialLayoutPos(RECT rc, bool bNeedInvalidate)
{

}

void TileLayoutExtended::SetPos(RECT rc, bool bNeedInvalidate)
{
    CControlUI::SetPos(rc, bNeedInvalidate);
    rc = m_rcItem;
    if (m_nSpecialLayout != 0) {
        SetSpecialLayoutPos(rc,bNeedInvalidate);
        return;
    }
    // Adjust for inset
    rc.left += m_rcInset.left;
    rc.top += m_rcInset.top;
    rc.right -= m_rcInset.right;
    rc.bottom -= m_rcInset.bottom;

    if( m_vtk_windows.size() == 0) {
        ProcessScrollBar(rc, 0, 0);
        return;
    }

    if( m_pVerticalScrollBar && m_pVerticalScrollBar->IsVisible() ) m_pVerticalScrollBar->SetVisible(false);
    if( m_pHorizontalScrollBar && m_pHorizontalScrollBar->IsVisible() ) m_pHorizontalScrollBar->SetVisible(false);

    // Position the elements
    if( m_nColumns == 0 ) m_nColumns = 1;
    if (m_nRows == 0 ) m_nRows = 1;

    int cxWidth = (rc.right - rc.left) / m_nColumns;
    
    int cyHeight = (rc.bottom - rc.top) / m_nRows;
    std::list<DuiVtkWindow*>::iterator it = m_vtk_windows.begin();
    int row = 0;
    int col = 0;
    for (int i =0;it != m_vtk_windows.end();++it,i++) {
        if (i >= m_nRows*m_nColumns) {
            ::SetWindowPos((*it)->GetHWND(),HWND_TOP,
                0,0,0,0,SWP_ASYNCWINDOWPOS);
            break;
        } else {
            if (1){//!(*it)->IsFloatWnd()) {
                RECT rcPos = {m_rcItem.left + cxWidth * col
                    ,m_rcItem.top + cyHeight*row
                    ,m_rcItem.left + cxWidth * (col+1)
                    ,m_rcItem.top +cyHeight*(row+1)};
                ::SetWindowPos((*it)->GetHWND(),HWND_TOP,
                    rcPos.left,rcPos.
                    top,rcPos.right-rcPos.left,
                    rcPos.bottom-rcPos.top,SWP_ASYNCWINDOWPOS);
            }
            col++;
            if (col >= m_nColumns) {
                row++;
                col = 0;
            }
        }
    }
}
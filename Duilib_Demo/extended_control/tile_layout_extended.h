#ifndef _DEEPWISE_EXTENDED_TILE_LAYOUT_EXTENDED_H_
#define _DEEPWISE_EXTENDED_TILE_LAYOUT_EXTENDED_H_
#pragma once

#include "UIlib.h"

#include <list>

class DuiVtkWindow;

class TileLayoutExtended :public DuiLib::CTileLayoutUI
{
    DECLARE_DUICONTROL(TileLayoutExtended)
public:
    TileLayoutExtended();
    ~TileLayoutExtended();

    LPCTSTR GetClass() const;

    int GetVtkCount() const;

    DuiVtkWindow* GetActivationVtkWindow();
    void SetActivationVtkWindow(DuiVtkWindow* vtk_window);
    std::list<DuiVtkWindow*> GetVtkWindowList();

    bool AddVtkWindow(DuiVtkWindow* vtk_window);
    void RemoveAllVtkWindow();
    bool RemoveVtkWinodw(int index);
    bool RemoveVtkWinodw(DuiVtkWindow* vtk_window);

    void AddRow();
    void AddColumn();
    void DelRow();
    void DelColumn();

    int GetRow() const;
    void SetRow(int nRows);

    int GetSepcialLayout() const;
    void SetSepcialLayout(int nSpecialLayout);

    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

    void SetPos(RECT rc, bool bNeedInvalidate = true);
    void SetSpecialLayoutPos(RECT rc, bool bNeedInvalidate = true);
protected:
    std::list<DuiVtkWindow*> m_vtk_windows;
    DuiVtkWindow* m_activation_vtk_windows;
    int m_nSpecialLayout;
    int m_nRows;
};

#endif //_DEEPWISE_EXTENDED_TILE_LAYOUT_EXTENDED_H_
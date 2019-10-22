#include "stdafx.h"
#include "vtkExampleBase.h"


vtkExampleBase::vtkExampleBase(HWND parent, CButtonUI* vtkShowBtn)
	: m_parentWnd(parent)
	, m_vtkShowBtn(vtkShowBtn)
{
}


vtkExampleBase::~vtkExampleBase(void)
{
}

void vtkExampleBase::InitVtkExampleDemo()
{
    ResizeAndPosition();
}

void vtkExampleBase::ShowVtkExampleDemo()
{    
}

void vtkExampleBase::ResizeAndPosition()
{
	if (m_parentWnd) {
		m_renderWindow->SetParentId(m_parentWnd);

		RECT rect;
		if (m_vtkShowBtn) {
			rect = m_vtkShowBtn->GetClientPos();
			m_vtkShowBtn->SetText(_T("Please wait..."));
		}

		m_renderWindow->SetSize(rect.right - rect.left, rect.bottom - rect.top);
		m_renderWindow->SetPosition(rect.left, rect.top);
	}
}
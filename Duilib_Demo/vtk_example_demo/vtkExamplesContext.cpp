#include "stdafx.h"
#include "vtk_example_demo/vtkExamplesContext.h"

#include "vtk_example_demo/vtkExampleStrategy.h"

vtkExamplesContext::vtkExamplesContext(vtkExampleStrategyBase* strategy)
	: m_strategy(strategy)
{
}


vtkExamplesContext::~vtkExamplesContext(void)
{
	if (m_strategy) {
		delete m_strategy;
		m_strategy = NULL;
	}
}

void vtkExamplesContext::DoAction()
{
	if (m_strategy) {
		m_strategy->AlgrithmInterface();
	}
}
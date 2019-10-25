#pragma once
#ifndef _VTK_EXAMPLES_CONTEXT_H_
#define _VTK_EXAMPLES_CONTEXT_H_

class vtkExampleStrategyBase;
class vtkExamplesContext
{
public:
	vtkExamplesContext(vtkExampleStrategyBase* strategy);

	virtual ~vtkExamplesContext(void);

	void DoAction();

private:
	vtkExampleStrategyBase* m_strategy;
};

#endif

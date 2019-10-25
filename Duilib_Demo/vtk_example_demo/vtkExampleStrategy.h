#pragma once
#ifndef _VTK_EXAMPLE_STRATEGY_BASE_H_
#define _VTK_EXAMPLE_STRATEGY_BASE_H_

#include "global_include.h"

class vtkExampleStrategyBase
{
public:	
	virtual ~vtkExampleStrategyBase(void);

	virtual void AlgrithmInterface() = 0;

protected:
	vtkExampleStrategyBase(void);

private:
};

//////////////////////////////////////////////////////////////////////////
class vtkShrinkAndMaginifyStrategy : public vtkExampleStrategyBase
{
public:
    vtkShrinkAndMaginifyStrategy();
    ~vtkShrinkAndMaginifyStrategy();
    virtual void AlgrithmInterface();
protected:
private:
};
//////////////////////////////////////////////////////////////////////////
class vtkMathematicsStrategy : public vtkExampleStrategyBase
{
public:
	vtkMathematicsStrategy();
	~vtkMathematicsStrategy();
	virtual void AlgrithmInterface();
protected:
private:
};
//////////////////////////////////////////////////////////////////////////

#endif
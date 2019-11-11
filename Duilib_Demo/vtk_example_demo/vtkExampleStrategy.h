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
class vtkMathLogicStrategy : public vtkExampleStrategyBase
{
public:
	vtkMathLogicStrategy();
	~vtkMathLogicStrategy();
	virtual void AlgrithmInterface();
protected:
private:
};
//////////////////////////////////////////////////////////////////////////
class vtkImageThreholdStrategy : public vtkExampleStrategyBase
{
public:
	vtkImageThreholdStrategy();
	~vtkImageThreholdStrategy();
	virtual void AlgrithmInterface();
protected:
private:
};
//////////////////////////////////////////////////////////////////////////
class vtkImageGradientStrategy : public vtkExampleStrategyBase
{
public:
	vtkImageGradientStrategy();
	~vtkImageGradientStrategy();
	virtual void AlgrithmInterface();
protected:
private:
};
//////////////////////////////////////////////////////////////////////////
// 图像平滑---均值滤波
class vtkImageMeanFilterStrategy : public vtkExampleStrategyBase
{
public:
	vtkImageMeanFilterStrategy();
	~vtkImageMeanFilterStrategy();
	virtual void AlgrithmInterface();
protected:
private:
};
//////////////////////////////////////////////////////////////////////////
// 图像频域---FFT和RFFT
class vtkImageFftAndRfftStrategy : public vtkExampleStrategyBase
{
public:
	vtkImageFftAndRfftStrategy();
	~vtkImageFftAndRfftStrategy();
	virtual void AlgrithmInterface();
protected:
private:
};
//////////////////////////////////////////////////////////////////////////
// 多个 Mask
class vtkImageMultiMaskStrategy : public vtkExampleStrategyBase
{
public:
	vtkImageMultiMaskStrategy();
	~vtkImageMultiMaskStrategy();
	virtual void AlgrithmInterface();
protected:
private:
};
//////////////////////////////////////////////////////////////////////////

#endif
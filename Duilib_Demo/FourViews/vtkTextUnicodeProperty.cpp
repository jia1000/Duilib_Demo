/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkTextUnicodeProperty.cpp

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

/**********************************************************************

  文件名: vtkTextUnicodeProperty.cpp
  Copyright (c) 张晓东, 罗火灵. All rights reserved.
  更多信息请访问: 
    http://www.vtkchina.org (VTK中国)
	http://blog.csdn.net/www_doling_net (东灵工作室) 

**********************************************************************/

#include "vtkTextUnicodeProperty.h"
#include "vtkObjectFactory.h"

vtkStandardNewMacro(vtkTextUnicodeProperty);

vtkTextUnicodeProperty::vtkTextUnicodeProperty(void)
{
	// 注意：这里应该将默认字体置为空，字体大小也不需要初始化；否则外部
	// 无法修改中文显示字体以及字体大小，只能在此位置修改。
	m_FontFileName="C:/Windows/Fonts/SIMYOU.TTF";
	this->FontSize = 10;
}

vtkTextUnicodeProperty::~vtkTextUnicodeProperty(void)
{
}

void vtkTextUnicodeProperty::ShallowCopy(vtkTextUnicodeProperty *tprop)
{
	if (!tprop)
	{
		return;
	}
	tprop->SetFontFileName("C:/Windows/Fonts/SIMYOU.TTF");
	Superclass::ShallowCopy( tprop);
	SetFontFileName(tprop->GetFontFileName());
}

void vtkTextUnicodeProperty::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os,indent);
	os << indent << "FontFileName: " << m_FontFileName.c_str() << "\n";
}

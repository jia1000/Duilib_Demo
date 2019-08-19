/*
 *  
 *  $Id: dicomechoassociation.h $
 *  DeepWise Dicom Viewer
 *
 *  Copyright 2017-2030 DeepWise All rights reserved.
 *  http://www.deepwise.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *
 */
#pragma once
#include "dicomassociation.h"
//#include <api/dicom/imodelodicom.h>

class EchoAssociation : public Association {
public:
	
	/**
	 Constructors
	 */
	EchoAssociation(const std::string& ambitolog);
	
	virtual ~EchoAssociation();
	
	virtual void OnAddPresentationContext(T_ASC_Parameters* params);
	
};

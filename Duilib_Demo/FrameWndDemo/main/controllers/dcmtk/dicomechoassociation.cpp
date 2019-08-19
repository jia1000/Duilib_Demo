/*
 *
 *  $Id: dicomechoassociation.cpp $
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
//#include <api/controllers/icontroladorlog.h>
//#include <api/dicom/idicomconformance.h>
//#include <main/controllers/controladorlog.h>
#include "dicomechoassociation.h"


#ifdef verify
#define MACRO_QUE_ESTORBA verify
#undef verify
#endif


#ifdef MACRO_QUE_ESTORBA
#define verify MACRO_QUE_ESTORBA
#endif

EchoAssociation::EchoAssociation(const std::string& ambitolog) : Association(ambitolog) {
	m_abstractSyntax = "";//GIL::DICOM::Conformance::GetScpSOPClasses().GetSOPClassUID("VerificationSOPClass");
}

EchoAssociation::~EchoAssociation() {
}

void EchoAssociation::OnAddPresentationContext(T_ASC_Parameters* params) {
	//GIL::DICOM::ArrayHelper transfersyntaxes;
	//transfersyntaxes.copyFrom(GIL::DICOM::Conformance::GetScuSOPClasses().GetSupportedTransferSyntaxUIDs("VerificationSOPClass"));

	const char* transfersyntaxes[4] = {"1.2.840.10008.1.2"	    
		, "1.2.840.10008.1.2.1"	
		, "1.2.840.10008.1.2.2"	
		, "1.2.840.10008.1.2.1.99"
	};
	
	//CONDITION cond = ASC_addPresentationContext(params, 1, m_abstractSyntax.c_str(), transfersyntaxes.array, transfersyntaxes.size);
	CONDITION cond = ASC_addPresentationContext(params, 1, m_abstractSyntax.c_str(), transfersyntaxes, 4);

	if (cond.bad()) {
		//LOG_ERROR(ambitolog, "Unable to add default presentation context");
	}
}

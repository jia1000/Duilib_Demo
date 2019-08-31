/*
*  
*  $Id: dicommoveassociation.h $
*  DeepWise Dicom Viewer
*
*  Copyright 2017-2030 DeepWise All rights reserved.
*  http://www.deepwise.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*  Code adapted from Aeskulap
*
*/
#pragma once
#include <ctime>

#include "dicomfindassociation.h"
//#include "istorecallback.h"


class MoveAssociation : public FindAssociation {
public:

	MoveAssociation(const std::string& ambitolog);//, IModeloDicom* pModelo=NULL);
	virtual ~MoveAssociation();

	//void SetModelo(IModeloDicom* pModelo);

	void Create(const std::string& title, const std::string& peer, int port, const std::string& ouraet, /*int ourPort,*/ const char *abstractSyntax = UID_MOVEStudyRootQueryRetrieveInformationModel);

	CONDITION SendObject(DcmDataset *dataset);
	static void subOpCallback(void* pCaller ,T_ASC_Network *aNet, T_ASC_Association **subAssoc);
	OFCondition	subOpSCP(T_ASC_Association **subAssoc);
	CONDITION storeSCP(T_ASC_Association *assoc, T_DIMSE_Message *msg, T_ASC_PresentationContextID presID) ;
	void SetPath(std::string path);
	void SetNumeroImagene(int index);
	static void storeSCPCallback(void *callbackData, T_DIMSE_StoreProgress *progress, T_DIMSE_C_StoreRQ *req, char *imageFileName, DcmDataset **imageDataSet, T_DIMSE_C_StoreRSP *rsp, DcmDataset **statusDetail);
	static CONDITION echoSCP(T_ASC_Association *assoc, T_DIMSE_Message *msg, T_ASC_PresentationContextID presID);
	OFCondition	acceptSubAssoc(T_ASC_Network *aNet, T_ASC_Association **assoc);
protected:

	void OnAddPresentationContext(T_ASC_Parameters *params);

private:

	CONDITION moveSCU(DcmDataset *pdset);

	int m_maxReceivePDULength;
	//IModeloDicom* m_pModelo;
	std::string m_errorMessage;
	std::string m_pathParam;
	int m_numeroImagenes;
};

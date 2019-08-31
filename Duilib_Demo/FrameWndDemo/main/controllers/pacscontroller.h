/*
 *  
 *  $Id: pacscontroller.h $
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

#include "api/dicom/dicomdataset.h"

#include <dcmtk/dcmdata/dcdict.h>
#include <dcmtk/dcmdata/dcdicent.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include "dcmtk/dicomservers.h"

//typedef enum TRetrieveMethod {
//	MOVE = 0,
//	GET,
//	WADO
//}TRetrieveMethod;

namespace GIL {
	namespace DICOM {
		class PACSController{
		public:
			static PACSController* Instance();
			static void FreeInstance();
			
			virtual void  GetConnection(void* connectionKey);
			virtual void  ReleaseConnection(void* connectionKey);
			
			virtual void InitDicomServer();

			virtual bool ObtainDicomDataSet(
				void* connectionKey,
				const GIL::DICOM::DicomDataset& base,
				std::list< GNC::GCS::Ptr<GIL::DICOM::DicomDataset> >& resultsWrapper);

			virtual bool DownloadDicomFilesBySeries(
				void* connectionKey,
				const GIL::DICOM::DicomDataset& base, 
				const std::string series_path
				);
						
			void FillInQuery(const GIL::DICOM::DicomDataset& base, DcmDataset* query);//, const GNC::GCS::Ptr<DicomServer>& server);
			DcmElement* CrearElementoConValor(const char* s);

			void InitFindQueryWrapper(GIL::DICOM::DicomDataset&  queryWrapper);
			void SetWrapper(GIL::DICOM::DicomDataset&  queryWrapper, char* tag, std::string value);

		protected:
			PACSController();
			~PACSController();

			static PACSController*     m_pInstance;

			GNC::GCS::Ptr<DicomServer> server;

			//TRetrieveMethod retrieve_method;
			std::string m_localAET;
			std::string m_retrieve_way;
		};
	};
};

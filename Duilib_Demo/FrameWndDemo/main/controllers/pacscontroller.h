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

typedef enum TRetrieveMethod {
	MOVE = 0,
	GET,
	WADO
}TRetrieveMethod;

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
			
			virtual bool ObtenerSerie(
				void* connectionKey,
				const std::string& serverId,
				const GIL::DICOM::DicomDataset& base,
				//IModeloDicom* pModelo,
				//GNC::IProxyNotificadorProgreso* pNotificador,
				bool link
				);

			virtual bool ObtenerImagen(
				void* connectionKey,
				const std::string& serverId,
				const GIL::DICOM::DicomDataset& base//,
				//IModeloDicom* pModelo,
				//GNC::IProxyNotificadorProgreso* pNotificador
				);

			void FillInQuery(const GIL::DICOM::DicomDataset& base, DcmDataset* query);//, const GNC::GCS::Ptr<DicomServer>& server);
			DcmElement* CrearElementoConValor(const char* s);

			//void SetCurPatientId(std::string patient_id);
			void SetThickness(std::string thickness);
			void SetBodyPartExamined(std::string bodyPartExamined);
			void SetDicomSavedPath(std::string path);

			void InitFindQueryWrapper(GIL::DICOM::DicomDataset&  queryWrapper);
			//void InitStudyFindQueryWrapper(GIL::DICOM::DicomDataset&  queryWrapper);
			void InitSerieFindQueryWrapper(GIL::DICOM::DicomDataset&  queryWrapper);

			void SetWrapper(GIL::DICOM::DicomDataset&  queryWrapper, char* tag, std::string value);

		protected:
			PACSController();
			~PACSController();

			static PACSController*     m_pInstance;

			GNC::GCS::Ptr<DicomServer> server;

			std::string m_dicom_saved_path;
			std::string m_filter_thickness;
			std::string m_bodyPartExamined;
			TRetrieveMethod retrieve_method; 
			//std::string m_cur_patient_id;
		};
	};
};

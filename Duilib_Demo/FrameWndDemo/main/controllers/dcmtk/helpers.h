/*
 *  
 *  $Id: helpers.h $
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
#include <string>

class DcmDataset;

const std::string DumpDataset(DcmDataset* dataset);

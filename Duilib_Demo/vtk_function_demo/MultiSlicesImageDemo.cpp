#include "stdafx.h"
#include "MultiSlicesImageDemo.h"

// 创建一个序列的三维重建
MultiSlicesImageDemo::MultiSlicesImageDemo(HWND parent, RECT rc) : CWidgetsDemoBase(parent, rc)
{
}


MultiSlicesImageDemo::~MultiSlicesImageDemo(void)
{
}

void MultiSlicesImageDemo::StartWidgetsRender(vtkSmartPointer<vtkRenderer> renderer, 
                                              vtkSmartPointer<vtkRenderWindow> renderWindow, 
                                              vtkSmartPointer<vtkRenderWindowInteractor> interactor)
{
	//m_v16 = vtkSmartPointer<vtkMetaImageReader>::New();
	//m_v16->SetDataDimensions(64,64);
	//m_v16->SetImageRange(1, 93);
	//m_v16->SetDataByteOrderToLittleEndian();
	//m_v16->SetFilePrefix ("F:\\dev_study\\duilib_study\\Duilib_Demo\\Duilib_Demo\\data\\FullHead.mhd");
	//m_v16->SetDataSpacing (3.2, 3.2, 1.5);
	//m_v16->Update();

	m_v16 = vtkSmartPointer<vtkMetaImageReader>::New();
	m_v16->SetFileName("F:\\dev_study\\duilib_study\\Duilib_Demo\\Duilib_Demo\\data\\FullHead.mhd");
	m_v16->Update();

	int *data_extent;
	data_extent = m_v16->GetDataExtent();

	for (int Index = 0; Index < 6 ; Index++) {
		m_data_extent[Index] = data_extent[Index];
	}
	m_bwLut = vtkSmartPointer<vtkLookupTable>::New();
	m_bwLut->SetTableRange (0, 2000);
	m_bwLut->SetSaturationRange (0, 0);
	m_bwLut->SetHueRange (0, 0);
	m_bwLut->SetValueRange (0, 1);
	m_bwLut->Build(); 

	m_hueLut = vtkSmartPointer<vtkLookupTable>::New();
	m_hueLut->SetTableRange (0, 2000);
	m_hueLut->SetHueRange (0, 1);
	m_hueLut->SetSaturationRange (1, 1);
	m_hueLut->SetValueRange (1, 1);
	m_hueLut->Build();

	m_satLut = vtkSmartPointer<vtkLookupTable>::New();
	m_satLut->SetTableRange (0, 2000);
	m_satLut->SetHueRange (.6, .6);
	m_satLut->SetSaturationRange (0, 1);
	m_satLut->SetValueRange (1, 1);
	m_satLut->Build();

	SetSkinActor(m_v16);
	SetBoneActor(m_v16);
	SetOutlineActor(m_v16);
	// 这是3个轴面的slice显示
	SetSagittalActor(m_v16, m_bwLut);
	SetAxialActor(m_v16, m_hueLut);
	SetCoronalActor(m_v16, m_satLut);

	vtkSmartPointer<vtkCamera> aCamera =
		vtkSmartPointer<vtkCamera>::New();
	aCamera->SetViewUp (0, 0, -1);
	aCamera->SetPosition (0, 1, 0);
	aCamera->SetFocalPoint (0, 0, 0);
	aCamera->ComputeViewPlaneNormal();
	aCamera->Azimuth(30.0);
	aCamera->Elevation(30.0);

	renderer->AddActor(outline);
	renderer->AddActor(sagittal);
	renderer->AddActor(axial);
	renderer->AddActor(coronal);
	renderer->AddActor(skin);
	renderer->AddActor(bone);

	// Turn off bone for this example.
	bone->VisibilityOff();

	// Set skin to semi-transparent.
	skin->GetProperty()->SetOpacity(0.5);

	// An initial camera view is created.  The Dolly() method moves
	// the camera towards the FocalPoint, thereby enlarging the image.
	renderer->SetActiveCamera(aCamera);

	//////////////////////////////////////////////////////////////////////////
    //renderWindow->SetSize(600, 600);
	ResizeAndPosition(m_rc);

	renderer->ResetCamera ();
	aCamera->Dolly(1.5);

	renderer->ResetCameraClippingRange ();

    interactor->Initialize();
    renderWindow->Render();
    interactor->Start();
}

void MultiSlicesImageDemo::SetMyInteractorStyle(vtkSmartPointer<vtkRenderWindowInteractor> interactor)
{
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    interactor->SetInteractorStyle( style );
}

void MultiSlicesImageDemo::CreatePipeline(vtkSmartPointer<vtkRenderer> renderer)
{
    
}

void MultiSlicesImageDemo::AddMyActor(vtkSmartPointer<vtkRenderWindowInteractor> interactor)
{

}

void MultiSlicesImageDemo::SetSkinActor(vtkSmartPointer<vtkMetaImageReader> v16)
{
	vtkSmartPointer<vtkContourFilter> skinExtractor =
		vtkSmartPointer<vtkContourFilter>::New();
	skinExtractor->SetInputConnection( v16->GetOutputPort());
	skinExtractor->SetValue(0, 500);
	skinExtractor->Update();

	vtkSmartPointer<vtkPolyDataNormals> skinNormals =
		vtkSmartPointer<vtkPolyDataNormals>::New();
	skinNormals->SetInputConnection(skinExtractor->GetOutputPort());
	skinNormals->SetFeatureAngle(60.0);
	skinNormals->Update();

	vtkSmartPointer<vtkStripper> skinStripper =
		vtkSmartPointer<vtkStripper>::New();
	skinStripper->SetInputConnection(skinNormals->GetOutputPort());
	skinStripper->Update();

	vtkSmartPointer<vtkPolyDataMapper> skinMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	skinMapper->SetInputConnection(skinStripper->GetOutputPort());
	skinMapper->ScalarVisibilityOff();

	skin = vtkSmartPointer<vtkActor>::New();
	skin->SetMapper(skinMapper);
	skin->GetProperty()->SetDiffuseColor(1, .49, .25);
	skin->GetProperty()->SetSpecular(.3);
	skin->GetProperty()->SetSpecularPower(20);
}

void MultiSlicesImageDemo::SetBoneActor(vtkSmartPointer<vtkMetaImageReader> v16)
{
	vtkSmartPointer<vtkContourFilter> boneExtractor =
		vtkSmartPointer<vtkContourFilter>::New();
	boneExtractor->SetInputConnection(v16->GetOutputPort());
	boneExtractor->SetValue(0, 1150);

	vtkSmartPointer<vtkPolyDataNormals> boneNormals =
		vtkSmartPointer<vtkPolyDataNormals>::New();
	boneNormals->SetInputConnection(boneExtractor->GetOutputPort());
	boneNormals->SetFeatureAngle(60.0);

	vtkSmartPointer<vtkStripper> boneStripper =
		vtkSmartPointer<vtkStripper>::New();
	boneStripper->SetInputConnection(boneNormals->GetOutputPort());

	vtkSmartPointer<vtkPolyDataMapper> boneMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	boneMapper->SetInputConnection(boneStripper->GetOutputPort());
	boneMapper->ScalarVisibilityOff();

	bone = vtkSmartPointer<vtkActor>::New();
	bone->SetMapper(boneMapper);
	bone->GetProperty()->SetDiffuseColor(1, 1, .9412);
}

void MultiSlicesImageDemo::SetOutlineActor(vtkSmartPointer<vtkMetaImageReader> v16)
{
	vtkSmartPointer<vtkOutlineFilter> outlineData =
		vtkSmartPointer<vtkOutlineFilter>::New();
	outlineData->SetInputConnection(v16->GetOutputPort());
	outlineData->Update();

	vtkSmartPointer<vtkPolyDataMapper> mapOutline =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapOutline->SetInputConnection(outlineData->GetOutputPort());

	outline = vtkSmartPointer<vtkActor>::New();
	outline->SetMapper(mapOutline);
	outline->GetProperty()->SetColor(0,0,0);
}

void MultiSlicesImageDemo::SetSagittalActor(vtkSmartPointer<vtkMetaImageReader> v16, vtkSmartPointer<vtkLookupTable> lut)
{
	vtkSmartPointer<vtkImageMapToColors> sagittalColors = vtkSmartPointer<vtkImageMapToColors>::New();
	sagittalColors->SetInputConnection(v16->GetOutputPort());
	sagittalColors->SetLookupTable(lut);
	sagittalColors->Update();

	sagittal = vtkSmartPointer<vtkImageActor>::New();
	sagittal->GetMapper()->SetInputConnection(sagittalColors->GetOutputPort());
	//sagittal->SetDisplayExtent(32,32, 0,63, 0,92);
	int adv = m_data_extent[1] - m_data_extent[0] + 1;
	adv /= 2;
	sagittal->SetDisplayExtent(adv, adv, m_data_extent[2],m_data_extent[3], m_data_extent[4],m_data_extent[5]);

}

void MultiSlicesImageDemo::SetAxialActor(vtkSmartPointer<vtkMetaImageReader> v16, vtkSmartPointer<vtkLookupTable> lut)
{
	vtkSmartPointer<vtkImageMapToColors> axialColors = vtkSmartPointer<vtkImageMapToColors>::New();
	axialColors->SetInputConnection(v16->GetOutputPort());
	axialColors->SetLookupTable(lut);
	axialColors->Update();

	axial =	vtkSmartPointer<vtkImageActor>::New();
	axial->GetMapper()->SetInputConnection(axialColors->GetOutputPort());
	//axial->SetDisplayExtent(0,63, 0,63, 46,46);
	int adv = m_data_extent[5] - m_data_extent[4] + 1;
	adv /= 2;
	axial->SetDisplayExtent(m_data_extent[0],m_data_extent[1], m_data_extent[2],m_data_extent[3], adv, adv);

}

void MultiSlicesImageDemo::SetCoronalActor(vtkSmartPointer<vtkMetaImageReader> v16, vtkSmartPointer<vtkLookupTable> lut)
{
	vtkSmartPointer<vtkImageMapToColors> coronalColors =	vtkSmartPointer<vtkImageMapToColors>::New();
	coronalColors->SetInputConnection(v16->GetOutputPort());
	coronalColors->SetLookupTable(lut);
	coronalColors->Update();

	coronal = vtkSmartPointer<vtkImageActor>::New();
	coronal->GetMapper()->SetInputConnection(coronalColors->GetOutputPort());
	//coronal->SetDisplayExtent(0,63, 32,32, 0,92);
	int adv = m_data_extent[3] - m_data_extent[2] + 1;
	adv /= 2;
	coronal->SetDisplayExtent(m_data_extent[0],m_data_extent[1], adv, adv, m_data_extent[4],m_data_extent[5]);

}
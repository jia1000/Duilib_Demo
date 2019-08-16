#pragma once
#include "WidgetsDemoBase.h"

class MultiSlicesImageDemo :
    public CWidgetsDemoBase
{
public:
    MultiSlicesImageDemo(HWND parent, RECT rc);
    ~MultiSlicesImageDemo(void);

    void SetMyInteractorStyle(vtkSmartPointer<vtkRenderWindowInteractor> interactor);

    virtual void StartWidgetsRender(vtkSmartPointer<vtkRenderer> renderer, 
        vtkSmartPointer<vtkRenderWindow> renderWindow, 
        vtkSmartPointer<vtkRenderWindowInteractor> interactor);
    virtual void CreatePipeline(vtkSmartPointer<vtkRenderer> renderer);
    virtual void AddMyActor(vtkSmartPointer<vtkRenderWindowInteractor> interactor);

	void SetSkinActor(vtkSmartPointer<vtkVolume16Reader> v16);
	void SetBoneActor(vtkSmartPointer<vtkVolume16Reader> v16);
	void SetOutlineActor(vtkSmartPointer<vtkVolume16Reader> v16);

	void SetSagittalActor(vtkSmartPointer<vtkVolume16Reader> v16, vtkSmartPointer<vtkLookupTable> lut);
	void SetAxialActor(vtkSmartPointer<vtkVolume16Reader> v16, vtkSmartPointer<vtkLookupTable> lut);
	void SetCoronalActor(vtkSmartPointer<vtkVolume16Reader> v16, vtkSmartPointer<vtkLookupTable> lut);



private:
    vtkSmartPointer<vtkTextWidget> textWidget;

	vtkSmartPointer<vtkVolume16Reader> m_v16;
	vtkSmartPointer<vtkLookupTable> m_bwLut;
	vtkSmartPointer<vtkLookupTable> m_hueLut;
	vtkSmartPointer<vtkLookupTable> m_satLut;

	vtkSmartPointer<vtkActor> skin;
	vtkSmartPointer<vtkActor> bone;
	vtkSmartPointer<vtkActor> outline;

	vtkSmartPointer<vtkImageActor> sagittal;
	vtkSmartPointer<vtkImageActor> axial;
	vtkSmartPointer<vtkImageActor> coronal;
};


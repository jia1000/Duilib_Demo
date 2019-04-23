#include "stdafx.h"
#include "Rib3DView.h"

// 创建一个序列的三维重建
Rib3DView::Rib3DView(HWND parent, RECT rc) : CWidgetsDemoBase(parent, rc)
{
}


Rib3DView::~Rib3DView(void)
{
}

void Rib3DView::StartWidgetsRender(vtkSmartPointer<vtkRenderer> renderer, 
                                              vtkSmartPointer<vtkRenderWindow> renderWindow, 
                                              vtkSmartPointer<vtkRenderWindowInteractor> interactor)
{
    //renderWindow->SetSize(600, 600);
	ResizeAndPosition(m_rc);

    interactor->Initialize();
    renderWindow->Render();
    interactor->Start();
}

void Rib3DView::SetMyInteractorStyle(vtkSmartPointer<vtkRenderWindowInteractor> interactor)
{
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    interactor->SetInteractorStyle( style );
}

void Rib3DView::CreatePipeline(vtkSmartPointer<vtkRenderer> renderer)
{
    vtkSmartPointer<vtkDICOMImageReader> v16=vtkSmartPointer<vtkDICOMImageReader>::New();
    v16->SetDataByteOrderToLittleEndian();
    v16->SetDirectoryName("..//Bin//Skin//data//slices1");// 文件目录path
    //v16->SetDataSpacing(3.2,3.2,1.5);
    v16->Update();


    vtkSmartPointer<vtkContourFilter> skinExtractor=vtkSmartPointer<vtkContourFilter>::New();
    skinExtractor->SetInputConnection(v16->GetOutputPort());
    skinExtractor->SetValue(0,500);

    vtkSmartPointer<vtkPolyDataNormals> skinNormals=vtkSmartPointer<vtkPolyDataNormals>::New();
    skinNormals->SetInputConnection(skinExtractor->GetOutputPort());
    skinNormals->SetFeatureAngle(60.0);

    vtkSmartPointer<vtkPolyDataMapper> coneMapper=vtkSmartPointer<vtkPolyDataMapper>::New();
    coneMapper->SetInputConnection(skinNormals->GetOutputPort());

    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(coneMapper);

    renderer->AddActor(actor);
}

void Rib3DView::AddMyActor(vtkSmartPointer<vtkRenderWindowInteractor> interactor)
{

}
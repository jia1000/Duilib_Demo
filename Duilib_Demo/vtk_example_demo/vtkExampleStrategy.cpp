#include "stdafx.h"
#include "vtk_example_demo/vtkExampleStrategy.h"


vtkExampleStrategyBase::vtkExampleStrategyBase(void)
{
}


vtkExampleStrategyBase::~vtkExampleStrategyBase(void)
{
}

//////////////////////////////////////////////////////////////////////////
vtkShrinkAndMaginifyStrategy::vtkShrinkAndMaginifyStrategy()
{}

vtkShrinkAndMaginifyStrategy::~vtkShrinkAndMaginifyStrategy()
{}

void vtkShrinkAndMaginifyStrategy::AlgrithmInterface()
{
	//
	vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetFileName ("..\\Bin\\Skin\\data\\slices1\\2");
	reader->Update();

	// 降采样 设置
	vtkSmartPointer<vtkImageShrink3D> shrinkFilter = 
		vtkSmartPointer<vtkImageShrink3D>::New();
	shrinkFilter->SetInputConnection(reader->GetOutputPort());
	shrinkFilter->SetShrinkFactors(16,16,1);
	shrinkFilter->Update();

	// 升采样 设置
	vtkSmartPointer<vtkImageMagnify> magnifyFilter =
		vtkSmartPointer<vtkImageMagnify>::New();
	magnifyFilter->SetInputConnection(reader->GetOutputPort());
	magnifyFilter->SetMagnificationFactors(10,10,1);
	magnifyFilter->Update();

	int originalDims[3];
	reader->GetOutput()->GetDimensions(originalDims);

	double originalSpace[3];
	reader->GetOutput()->GetSpacing(originalSpace);

	// 降采样 图像大小
	int shrinkDims[3];
	shrinkFilter->GetOutput()->GetDimensions(shrinkDims);

	double shrinkSpace[3];
	shrinkFilter->GetOutput()->GetSpacing(shrinkSpace);

	// 升采样 图像大小
	int magnifyDims[3];
	magnifyFilter->GetOutput()->GetDimensions(magnifyDims);

	double magnifySpace[3];
	magnifyFilter->GetOutput()->GetSpacing(magnifySpace);

	std::cout<<"origin dimesions      : "<<originalDims[0] << " "<<originalDims[1]<<" "<<originalDims[2]<<std::endl;
	std::cout<<"origin space          : "<<originalSpace[0] << " "<<originalSpace[1]<<" "<<originalSpace[2]<<std::endl;
	std::cout<<"shrink dimesions      : "<<shrinkDims[0] << " "<<shrinkDims[1]<<" "<<shrinkDims[2]<<std::endl;
	std::cout<<"shrink space          : "<<shrinkSpace[0] << " "<<shrinkSpace[1]<<" "<<shrinkSpace[2]<<std::endl;
	std::cout<<"magnify dimensisons   :"<<magnifyDims[0] << " "<<magnifyDims[1]<<" "<<magnifyDims[2]<<std::endl;
	std::cout<<"magnify space         :"<<magnifySpace[0] << " "<<magnifySpace[1]<<" "<<magnifySpace[2]<<std::endl;

	vtkSmartPointer<vtkImageActor> originalActor =
		vtkSmartPointer<vtkImageActor>::New();
	originalActor->SetInput( reader->GetOutput());

	// 降采样 的actor
	vtkSmartPointer<vtkImageActor> shrinkActor =
		vtkSmartPointer<vtkImageActor>::New();
	shrinkActor->SetInput(shrinkFilter->GetOutput());

	// 升采样 的actor
	vtkSmartPointer<vtkImageActor> magnifyActor =
		vtkSmartPointer<vtkImageActor>::New();
	magnifyActor->SetInput(magnifyFilter->GetOutput());

	double originalViewport[4] = {0.0, 0.0, 0.33, 1.0};
	double shrinkViewport[4] = {0.33, 0.0, 0.66, 1.0};
	double magnifyViewport[4] = {0.66, 0.0, 1.0, 1.0};

	vtkSmartPointer<vtkRenderer> originalRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	originalRenderer->SetViewport(originalViewport);
	originalRenderer->AddActor(originalActor);
	originalRenderer->ResetCamera();
	originalRenderer->SetBackground(1.0, 1.0, 1.0);

	// 降采样 的 renderer
	vtkSmartPointer<vtkRenderer> shrinkRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	shrinkRenderer->SetViewport(shrinkViewport);
	shrinkRenderer->AddActor(shrinkActor);
	shrinkRenderer->ResetCamera();
	shrinkRenderer->SetBackground(1.0, 1.0, 1.0);

	// 升采样 的 renderer
	vtkSmartPointer<vtkRenderer> magnifyRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	magnifyRenderer->SetViewport(magnifyViewport);
	magnifyRenderer->AddActor(magnifyActor);
	magnifyRenderer->ResetCamera();
	magnifyRenderer->SetBackground(1.0, 1.0, 1.0);

	// renderwindow 及 添加renderer
	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(originalRenderer);
	renderWindow->AddRenderer(shrinkRenderer);
	renderWindow->AddRenderer(magnifyRenderer);
	renderWindow->SetSize(640, 320);
	renderWindow->Render();
	renderWindow->SetWindowName("ImageShrink3DExample");

	// vtk 的交互形式
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style =
		vtkSmartPointer<vtkInteractorStyleImage>::New();

	renderWindowInteractor->SetInteractorStyle(style);
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();
}
//////////////////////////////////////////////////////////////////////////
vtkMathematicsStrategy::vtkMathematicsStrategy()
{}

vtkMathematicsStrategy::~vtkMathematicsStrategy()
{}

void vtkMathematicsStrategy::AlgrithmInterface()
{
	vtkSmartPointer<vtkImageCanvasSource2D> imageSource = 
		vtkSmartPointer<vtkImageCanvasSource2D>::New();
	imageSource->SetNumberOfScalarComponents(3);
	imageSource->SetScalarTypeToUnsignedChar();
	imageSource->SetExtent(0, 4, 0, 4, 0, 0);
	imageSource->SetDrawColor(100.0, 0, 0);
	imageSource->FillBox(0, 4, 0, 4);
	imageSource->Update();

	vtkSmartPointer<vtkImageMathematics> imageMath = 
		vtkSmartPointer<vtkImageMathematics>::New();
	imageMath->SetOperationToMultiplyByK();		// 设置使用数学操作为：所有图像值，乘以常数K. 下面会设置为2.
	imageMath->SetConstantK(2.0);
	imageMath->SetInputConnection(imageSource->GetOutputPort());
	imageMath->Update();

	vtkSmartPointer<vtkImageActor> originalActor =
		vtkSmartPointer<vtkImageActor>::New();
	originalActor->SetInput(
		imageSource->GetOutput());

	vtkSmartPointer<vtkImageActor> mathActor =
		vtkSmartPointer<vtkImageActor>::New();
	mathActor->SetInput(imageMath->GetOutput());

	double leftViewport[4] = {0.0, 0.0, 0.5, 1.0};
	double rightViewport[4] = {0.5, 0.0, 1.0, 1.0};

	vtkSmartPointer<vtkRenderer> originalRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	originalRenderer->SetViewport(leftViewport);
	originalRenderer->AddActor(originalActor);
	originalRenderer->ResetCamera();
	originalRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> gradientMagnitudeRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	gradientMagnitudeRenderer->SetViewport(rightViewport);
	gradientMagnitudeRenderer->AddActor(mathActor);
	gradientMagnitudeRenderer->ResetCamera();
	gradientMagnitudeRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(originalRenderer);
	renderWindow->AddRenderer(gradientMagnitudeRenderer);
	renderWindow->SetSize(640, 480);
	renderWindow->Render();
	renderWindow->SetWindowName("ImageMathematicsExample");

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style =
		vtkSmartPointer<vtkInteractorStyleImage>::New();

	renderWindowInteractor->SetInteractorStyle(style);
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();
}
//////////////////////////////////////////////////////////////////////////
vtkMathLogicStrategy::vtkMathLogicStrategy()
{}

vtkMathLogicStrategy::~vtkMathLogicStrategy()
{}

void vtkMathLogicStrategy::AlgrithmInterface()
{
	vtkSmartPointer<vtkImageCanvasSource2D> imageSource1 =
		vtkSmartPointer<vtkImageCanvasSource2D>::New();
	imageSource1->SetScalarTypeToUnsignedChar();
	imageSource1->SetNumberOfScalarComponents(1);
	imageSource1->SetExtent(0, 100, 0, 100, 0, 0);
	imageSource1->SetDrawColor(0.0);
	imageSource1->FillBox(0,100,0,100);
	imageSource1->SetDrawColor(255);
	imageSource1->FillBox(20,60,20,60);
	imageSource1->Update();

	vtkSmartPointer<vtkImageCanvasSource2D> imageSource2 = 
		vtkSmartPointer<vtkImageCanvasSource2D>::New();
	imageSource2->SetNumberOfScalarComponents(1);
	imageSource2->SetScalarTypeToUnsignedChar();
	imageSource2->SetExtent(0, 100, 0, 100, 0, 0);
	imageSource2->SetDrawColor(0.0);
	imageSource2->FillBox(0,100,0,100);
	imageSource2->SetDrawColor(255.0);
	imageSource2->FillBox(40,80,40,80);
	imageSource2->Update();

	vtkSmartPointer<vtkImageLogic> imageLogic = 
		vtkSmartPointer<vtkImageLogic>::New();
	imageLogic->SetInput1(imageSource1->GetOutput());
	imageLogic->SetInput2(imageSource2->GetOutput());
	imageLogic->SetOperationToXor();		// 设置异或操作
	imageLogic->SetOutputTrueValue(50);		// 设置异或操作为true时 的像素值。即，2个图像的像素不同时，显示为50
	imageLogic->Update();

	vtkSmartPointer<vtkImageActor> originalActor1 =
		vtkSmartPointer<vtkImageActor>::New();
	originalActor1->SetInput(imageSource1->GetOutput());

	vtkSmartPointer<vtkImageActor> originalActor2 =
		vtkSmartPointer<vtkImageActor>::New();
	originalActor2->SetInput(imageSource2->GetOutput());

	vtkSmartPointer<vtkImageActor> logicActor =
		vtkSmartPointer<vtkImageActor>::New();
	logicActor->SetInput(imageLogic->GetOutput());

	double leftViewport[4] = {0.0, 0.0, 0.33, 1.0};
	double midViewport[4] = {0.33, 0.0, 0.66, 1.0};
	double rightViewport[4] = {0.66, 0.0, 1.0, 1.0};

	vtkSmartPointer<vtkRenderer> originalRenderer1 =
		vtkSmartPointer<vtkRenderer>::New();
	originalRenderer1->SetViewport(leftViewport);
	originalRenderer1->AddActor(originalActor1);
	originalRenderer1->ResetCamera();
	originalRenderer1->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> originalRenderer2 =
		vtkSmartPointer<vtkRenderer>::New();
	originalRenderer2->SetViewport(midViewport);
	originalRenderer2->AddActor(originalActor2);
	originalRenderer2->ResetCamera();
	originalRenderer2->SetBackground(1.0, 1.0, 1.0);//(0.8, 0.8, 0.8);

	vtkSmartPointer<vtkRenderer> logicRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	logicRenderer->SetViewport(rightViewport);
	logicRenderer->AddActor(logicActor);
	logicRenderer->ResetCamera();
	logicRenderer->SetBackground(1.0, 1.0, 1.0);//(0.6, 0.6, 0.6);

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(originalRenderer1);
	renderWindow->AddRenderer(originalRenderer2);
	renderWindow->AddRenderer(logicRenderer);
	renderWindow->SetSize(640, 320);
	renderWindow->Render();
	renderWindow->SetWindowName("ImageLogicExample");

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style =
		vtkSmartPointer<vtkInteractorStyleImage>::New();

	renderWindowInteractor->SetInteractorStyle(style);
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();
}
//////////////////////////////////////////////////////////////////////////
vtkImageThreholdStrategy::vtkImageThreholdStrategy()
{}

vtkImageThreholdStrategy::~vtkImageThreholdStrategy()
{}

void vtkImageThreholdStrategy::AlgrithmInterface()
{
	vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetFileName ("..\\Bin\\Skin\\data\\slices1\\2");
	reader->Update();

	vtkSmartPointer<vtkImageThreshold> thresholdFilter =
		vtkSmartPointer<vtkImageThreshold>::New();
	thresholdFilter->SetInputConnection(reader->GetOutputPort());
	thresholdFilter->ThresholdByUpper(100); // 设置大于门限值100的区域，为有效区域
	thresholdFilter->SetInValue(255);		// 在有效区域内的像素内，设置为255
	thresholdFilter->SetOutValue(0);		// 在有效区域内的像素外，设置为0

	vtkSmartPointer<vtkImageActor> originalActor =
		vtkSmartPointer<vtkImageActor>::New();
	originalActor->SetInput(reader->GetOutput());

	vtkSmartPointer<vtkImageActor> binaryActor =
		vtkSmartPointer<vtkImageActor>::New();
	binaryActor->SetInput(thresholdFilter->GetOutput());

	double originalViewport[4] = {0.0, 0.0, 0.5, 1.0};
	double binaryviewport[4] = {0.5, 0.0, 1.0, 1.0};

	vtkSmartPointer<vtkRenderer> originalRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	originalRenderer->SetViewport(originalViewport);
	originalRenderer->AddActor(originalActor);
	originalRenderer->ResetCamera();
	originalRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> binaryRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	binaryRenderer->SetViewport(binaryviewport);
	binaryRenderer->AddActor(binaryActor);
	binaryRenderer->ResetCamera();
	binaryRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(originalRenderer);
	renderWindow->AddRenderer(binaryRenderer);
	renderWindow->SetSize(640, 320);
	renderWindow->Render();
	renderWindow->SetWindowName("ImageBinaryExample");

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style =
		vtkSmartPointer<vtkInteractorStyleImage>::New();

	renderWindowInteractor->SetInteractorStyle(style);
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();
}
//////////////////////////////////////////////////////////////////////////
vtkImageGradientStrategy::vtkImageGradientStrategy()
{}

vtkImageGradientStrategy::~vtkImageGradientStrategy()
{}

void vtkImageGradientStrategy::AlgrithmInterface()
{
	// 本例子，采用梯度算子，计算边缘问题。
	// 还有索贝尔sobel、Canny算子、拉普拉斯算子等算法。可以参看红宝书
	vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetFileName ("..\\Bin\\Skin\\data\\slices1\\2");
	reader->Update();

	vtkSmartPointer<vtkImageGradient> gradientFilter =
		vtkSmartPointer<vtkImageGradient>::New();
	gradientFilter->SetInputConnection(reader->GetOutputPort());
	gradientFilter->SetDimensionality(2);

	vtkSmartPointer<vtkImageMagnitude> magnitudeFilter =
		vtkSmartPointer<vtkImageMagnitude>::New();
	magnitudeFilter->SetInputConnection(gradientFilter->GetOutputPort());
	magnitudeFilter->Update();

	double range[2];
	magnitudeFilter->GetOutput()->GetScalarRange(range);

	vtkSmartPointer<vtkImageShiftScale> ShiftScale =
		vtkSmartPointer<vtkImageShiftScale>::New();
	ShiftScale->SetOutputScalarTypeToUnsignedChar();
	ShiftScale->SetScale( 255 / range[1] );
	ShiftScale->SetInputConnection(magnitudeFilter->GetOutputPort());
	ShiftScale->Update();

	vtkSmartPointer<vtkImageActor> originalActor =
		vtkSmartPointer<vtkImageActor>::New();
	originalActor->SetInput(reader->GetOutput());

	vtkSmartPointer<vtkImageActor> gradActor =
		vtkSmartPointer<vtkImageActor>::New();
	gradActor->SetInput(ShiftScale->GetOutput());

	double originalViewport[4] = {0.0, 0.0, 0.5, 1.0};
	double gradviewport[4] = {0.5, 0.0, 1.0, 1.0};

	vtkSmartPointer<vtkRenderer> originalRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	originalRenderer->SetViewport(originalViewport);
	originalRenderer->AddActor(originalActor);
	originalRenderer->ResetCamera();
	originalRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> gradRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	gradRenderer->SetViewport(gradviewport);
	gradRenderer->AddActor(gradActor);
	gradRenderer->ResetCamera();
	gradRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(originalRenderer);
	renderWindow->AddRenderer(gradRenderer);
	renderWindow->SetSize( 640, 320 );
	renderWindow->Render();
	renderWindow->SetWindowName("ImageGradientExample");

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style =
		vtkSmartPointer<vtkInteractorStyleImage>::New();

	renderWindowInteractor->SetInteractorStyle(style);
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();
}
//////////////////////////////////////////////////////////////////////////

vtkImageMeanFilterStrategy::vtkImageMeanFilterStrategy()
{}

vtkImageMeanFilterStrategy::~vtkImageMeanFilterStrategy()
{}

void vtkImageMeanFilterStrategy::AlgrithmInterface()
{
	// 本示例为使用了均值滤波的图像平滑处理。
	// 图像平滑处理后，再进行梯度计算，可以减少噪声对梯度的影响。
	vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetFileName ("..\\Bin\\Skin\\data\\slices1\\2");
	reader->Update();

	// 下面的卷积处理，要求数值为浮点型，所以，需要将图像的值，进行转换
	vtkSmartPointer<vtkImageCast> originalCastFilter =
		vtkSmartPointer<vtkImageCast>::New();
	originalCastFilter->SetInputConnection(reader->GetOutputPort());
	originalCastFilter->SetOutputScalarTypeToFloat();  // 这里转换
	originalCastFilter->Update();

	// 设置卷积模板
	vtkSmartPointer<vtkImageConvolve> convolveFilter =
		vtkSmartPointer<vtkImageConvolve>::New();
	convolveFilter->SetInputConnection(originalCastFilter->GetOutputPort());

	double kernel[25] = {0.04,0.04,0.04,0.04,0.04,
		0.04,0.04,0.04,0.04,0.04,
		0.04,0.04,0.04,0.04,0.04,
		0.04,0.04,0.04,0.04,0.04,
		0.04,0.04,0.04,0.04,0.04 };
	convolveFilter->SetKernel5x5(kernel);
	convolveFilter->Update();

	// 卷积完成后，为了图像显示，需要将数值转换为unsigned char类型。
	vtkSmartPointer<vtkImageCast> convCastFilter = 
		vtkSmartPointer<vtkImageCast>::New();
	convCastFilter->SetInput(convolveFilter->GetOutput());
	convCastFilter->SetOutputScalarTypeToUnsignedChar();  // 这里转换
	convCastFilter->Update();

	vtkSmartPointer<vtkImageActor> originalActor =
		vtkSmartPointer<vtkImageActor>::New();
	originalActor->SetInput(reader->GetOutput());

	vtkSmartPointer<vtkImageActor> convolvedActor =
		vtkSmartPointer<vtkImageActor>::New();
	convolvedActor->SetInput(convCastFilter->GetOutput());

	double leftViewport[4] = {0.0, 0.0, 0.5, 1.0};
	double rightViewport[4] = {0.5, 0.0, 1.0, 1.0};

	vtkSmartPointer<vtkRenderer> originalRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	originalRenderer->SetViewport(leftViewport);
	originalRenderer->AddActor(originalActor);
	originalRenderer->SetBackground(1.0, 1.0, 1.0);
	originalRenderer->ResetCamera();

	vtkSmartPointer<vtkRenderer> convolvedRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	convolvedRenderer->SetViewport(rightViewport);
	convolvedRenderer->AddActor(convolvedActor);
	convolvedRenderer->SetBackground(1.0, 1.0, 1.0);
	convolvedRenderer->ResetCamera();

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();;
	renderWindow->AddRenderer(originalRenderer);
	renderWindow->AddRenderer(convolvedRenderer);
	renderWindow->SetSize(640, 320);
	renderWindow->Render();
	renderWindow->SetWindowName("MeanFilterExample");

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style =
		vtkSmartPointer<vtkInteractorStyleImage>::New();

	renderWindowInteractor->SetInteractorStyle(style);
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();
}
//////////////////////////////////////////////////////////////////////////
vtkImageFftAndRfftStrategy::vtkImageFftAndRfftStrategy()
{}

vtkImageFftAndRfftStrategy::~vtkImageFftAndRfftStrategy()
{}

void vtkImageFftAndRfftStrategy::AlgrithmInterface()
{
	vtkSmartPointer<vtkDICOMImageReader> reader = vtkSmartPointer<vtkDICOMImageReader>::New();
	reader->SetFileName ("..\\Bin\\Skin\\data\\slices1\\2");
	reader->Update();

	// 进行二维快速傅里叶变化
	vtkSmartPointer<vtkImageFFT> fftFilter = 
		vtkSmartPointer<vtkImageFFT>::New();
	fftFilter->SetInputConnection(reader->GetOutputPort());
	fftFilter->SetDimensionality(2);
	fftFilter->Update();

	vtkSmartPointer<vtkImageExtractComponents> fftExtractReal =
		vtkSmartPointer<vtkImageExtractComponents>::New();
	fftExtractReal->SetInputConnection(fftFilter->GetOutputPort());
	fftExtractReal->SetComponents(0);

	double range[2];
	fftExtractReal->GetOutput()->GetScalarRange(range);

	vtkSmartPointer<vtkImageShiftScale> ShiftScale =
		vtkSmartPointer<vtkImageShiftScale>::New();
	ShiftScale->SetOutputScalarTypeToUnsignedChar();
	ShiftScale->SetScale( 255.0 / (range[1]-range[0]) );
	ShiftScale->SetShift(-range[0]);
	ShiftScale->SetInputConnection(fftExtractReal->GetOutputPort());
	ShiftScale->Update();

	vtkSmartPointer<vtkImageRFFT> rfftFilter =
		vtkSmartPointer<vtkImageRFFT>::New();
	rfftFilter->SetInputConnection(fftFilter->GetOutputPort());
	rfftFilter->SetDimensionality(2);
	rfftFilter->Update();

	vtkSmartPointer<vtkImageExtractComponents> ifftExtractReal =
		vtkSmartPointer<vtkImageExtractComponents>::New();
	ifftExtractReal->SetInputConnection(rfftFilter->GetOutputPort());
	ifftExtractReal->SetComponents(0);

	vtkSmartPointer<vtkImageCast> rfftCastFilter =
		vtkSmartPointer<vtkImageCast>::New();
	rfftCastFilter->SetInputConnection(ifftExtractReal->GetOutputPort());
	rfftCastFilter->SetOutputScalarTypeToUnsignedChar();
	rfftCastFilter->Update();

	vtkSmartPointer<vtkImageActor> originalActor =
		vtkSmartPointer<vtkImageActor>::New();
	originalActor->SetInput(reader->GetOutput());

	vtkSmartPointer<vtkImageActor> fftActor =
		vtkSmartPointer<vtkImageActor>::New();
	fftActor->SetInput(ShiftScale->GetOutput());

	vtkSmartPointer<vtkImageActor> rfftActor =
		vtkSmartPointer<vtkImageActor>::New();
	rfftActor->SetInput(rfftCastFilter->GetOutput());

	double originalViewport[4] = {0.0, 0.0, 0.33, 1.0};
	double fftViewport[4] = {0.33, 0.0, 0.66, 1.0};
	double rfftViewport[4] = {0.66, 0.0, 1.0, 1.0};

	vtkSmartPointer<vtkRenderer> originalRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	originalRenderer->SetViewport(originalViewport);
	originalRenderer->AddActor(originalActor);
	originalRenderer->ResetCamera();
	originalRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> fftRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	fftRenderer->SetViewport(fftViewport);
	fftRenderer->AddActor(fftActor);
	fftRenderer->ResetCamera();
	fftRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderer> rfftRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	rfftRenderer->SetViewport(rfftViewport);
	rfftRenderer->AddActor(rfftActor);
	rfftRenderer->ResetCamera();
	rfftRenderer->SetBackground(1.0, 1.0, 1.0);

	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(originalRenderer);
	renderWindow->AddRenderer(fftRenderer);
	renderWindow->AddRenderer(rfftRenderer);
	renderWindow->SetSize(640, 320);
	renderWindow->Render();
	renderWindow->SetWindowName("FFTAndRFFTExample");

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<vtkInteractorStyleImage> style =
		vtkSmartPointer<vtkInteractorStyleImage>::New();

	renderWindowInteractor->SetInteractorStyle(style);
	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();
	renderWindowInteractor->Start();
}
//////////////////////////////////////////////////////////////////////////
class opacityVtkInteractorStyleImage: public vtkInteractorStyleImage
{
public:
	static opacityVtkInteractorStyleImage* New();
	vtkTypeMacro(opacityVtkInteractorStyleImage, vtkInteractorStyleImage);
	void SetRendererWindow(vtkSmartPointer<vtkRenderWindow> renderer_window)
	{
		m_rendererWindow = renderer_window;
	}
	void SetActors(vtkSmartPointer<vtkImageActor> actor1, vtkSmartPointer<vtkImageActor> actor2)
	{
		m_actor1 = actor1;
		m_actor2 = actor2;
		m_opacity1 = 1.0f;
		m_opacity2 = 1.0f;
	}
	virtual void OnKeyDown()
	{
		std::string key = this->GetInteractor()->GetKeySym();
		if (key.compare("Up") == 0) {
			m_opacity1 += 0.2;
			if (m_opacity1 > 1.0) {
				m_opacity1 = 0;
			}
			m_actor1->SetOpacity(m_opacity1);
			printf("m_opacity1 = %.2f\n", m_opacity1);
		}
		else if (key.compare("Down")==0)
		{
			m_opacity1 -= 0.2;
			if (m_opacity1 < 0) {
				m_opacity1 = 1.0;
			}
			m_actor1->SetOpacity(m_opacity1);
			printf("m_opacity1 = %.2f\n", m_opacity1);
		}
		else if (key.compare("Left") == 0) {
			m_opacity2 -= 0.2;
			if (m_opacity2 < 0) {
				m_opacity2 = 1.0;
			}
			m_actor2->SetOpacity(m_opacity2);
			printf("                     m_opacity2 = %.2f\n", m_opacity2);
		}
		else if (key.compare("Right")==0)
		{
			m_opacity2 += 0.2;
			if (m_opacity2 > 1.0) {
				m_opacity2 = 0;
			}
			m_actor2->SetOpacity(m_opacity2);
			printf("                     m_opacity2 = %.2f\n", m_opacity2);
		}

		m_rendererWindow->Render();

		vtkInteractorStyleImage::OnKeyDown();
	}

private:
	vtkSmartPointer<vtkImageActor> m_actor1;
	vtkSmartPointer<vtkImageActor> m_actor2;
	vtkSmartPointer<vtkRenderWindow> m_rendererWindow;
	double m_opacity1;
	double m_opacity2;
};
vtkStandardNewMacro(opacityVtkInteractorStyleImage);
//////////////////////////////////////////////////////////////////////////
vtkImageMultiMaskStrategy::vtkImageMultiMaskStrategy()
{}

vtkImageMultiMaskStrategy::~vtkImageMultiMaskStrategy()
{}

void vtkImageMultiMaskStrategy::AlgrithmInterface()
{
	// Create an image of a rectangle
	vtkSmartPointer<vtkImageCanvasSource2D> source = 
		vtkSmartPointer<vtkImageCanvasSource2D>::New();
	source->SetScalarTypeToUnsignedChar();
	source->SetNumberOfScalarComponents(3);
	source->SetExtent(0, 200, 0, 200, 0, 0);

	// Create a red image
	source->SetDrawColor(255,0,0);
	source->FillBox(0,200,0,200);

	source->Update();

	// Create a rectanglular mask
	vtkSmartPointer<vtkImageCanvasSource2D> maskSource1 = 
		vtkSmartPointer<vtkImageCanvasSource2D>::New();
	maskSource1->SetScalarTypeToUnsignedChar();
	maskSource1->SetNumberOfScalarComponents(1);
	maskSource1->SetExtent(0, 200, 0, 200, 0, 0);

	// Initialize the mask to black
	maskSource1->SetDrawColor(255,255,255);
	maskSource1->FillBox(0,200,0,200);

	// Create a square
	maskSource1->SetDrawColor(0,255,0); //anything non-zero means "make the output pixel equal the input pixel." If the mask is zero, the output pixel is set to MaskedValue
	maskSource1->FillBox(100,120,100,120);
	maskSource1->Update();

	vtkSmartPointer<vtkImageCanvasSource2D> maskSource2 = 
		vtkSmartPointer<vtkImageCanvasSource2D>::New();
	maskSource2->SetScalarTypeToUnsignedChar();
	maskSource2->SetNumberOfScalarComponents(1);
	maskSource2->SetExtent(0, 200, 0, 200, 0, 0);

	// Initialize the mask to black
	maskSource2->SetDrawColor(255,255,255);
	maskSource2->FillBox(0,200,0,200);

	// Create a square
	maskSource2->SetDrawColor(0,255,0); //anything non-zero means "make the output pixel equal the input pixel." If the mask is zero, the output pixel is set to MaskedValue
	maskSource2->FillBox(10,30,10,30);
	maskSource2->Update();

	vtkSmartPointer<vtkImageMask> maskFilter1 = 
		vtkSmartPointer<vtkImageMask>::New();
	maskFilter1->SetInputConnection(0, source->GetOutputPort());
	maskFilter1->SetInputConnection(1, maskSource1->GetOutputPort());
	maskFilter1->SetMaskedOutputValue(0,1,0);
	maskFilter1->SetNotMask(1);
	maskFilter1->Update();

	vtkSmartPointer<vtkImageMask> maskFilter2 = 
		vtkSmartPointer<vtkImageMask>::New();
	maskFilter2->SetInputConnection(0, source->GetOutputPort());
	maskFilter2->SetInputConnection(1, maskSource2->GetOutputPort());
	maskFilter2->SetMaskedOutputValue(0,1,0);
	maskFilter2->SetNotMask(1);
	maskFilter2->Update();

	vtkSmartPointer<vtkImageLogic> imageLogic = 
		vtkSmartPointer<vtkImageLogic>::New();
	imageLogic->SetInput1(maskSource1->GetOutput());
	imageLogic->SetInput2(maskSource2->GetOutput());
	imageLogic->SetOperationToXor();		// 设置或操作
	imageLogic->SetOutputTrueValue(50);	// 设置或操作为true时 的像素值。
	imageLogic->Update();

	// Create actors
	vtkSmartPointer<vtkImageActor> originalActor =
		vtkSmartPointer<vtkImageActor>::New();
	originalActor->GetMapper()->SetInputConnection(source->GetOutputPort());

	vtkSmartPointer<vtkImageActor> maskedActor1 =
		vtkSmartPointer<vtkImageActor>::New();
	maskedActor1->GetMapper()->SetInputConnection(maskFilter1->GetOutputPort());
	//maskedActor1->SetOpacity(0.4);

	vtkSmartPointer<vtkImageActor> maskedActor2 =
		vtkSmartPointer<vtkImageActor>::New();
	maskedActor2->GetMapper()->SetInputConnection(maskFilter2->GetOutputPort());
	//maskedActor2->SetOpacity(0.4);

	vtkSmartPointer<vtkImageActor> logicActor =	vtkSmartPointer<vtkImageActor>::New();
	logicActor->SetInput(imageLogic->GetOutput());

	// Define viewport ranges
	// (xmin, ymin, xmax, ymax)
	double originalViewport[4] = {0.0, 0.0, 0.2, 1.0};
	double maskedViewport1[4] = {0.2, 0.0, 0.4, 1.0};
	double maskedViewport2[4] = {0.4, 0.0, 0.6, 1.0};
	double maskedViewportLogic[4] = {0.6, 0.0, 0.8, 1.0};
	double maskedViewportBlend[4] = {0.8, 0.0, 1.0, 1.0};


	// Setup renderers
	vtkSmartPointer<vtkRenderer> originalRenderer =
		vtkSmartPointer<vtkRenderer>::New();
	originalRenderer->SetViewport(originalViewport);
	originalRenderer->AddActor(originalActor);
	originalRenderer->ResetCamera();
	originalRenderer->SetBackground(.4, .5, .6);

	vtkSmartPointer<vtkRenderer> maskedRenderer1 =
		vtkSmartPointer<vtkRenderer>::New();
	maskedRenderer1->SetViewport(maskedViewport1);
	maskedRenderer1->AddActor(maskedActor1);
	maskedRenderer1->ResetCamera();
	maskedRenderer1->SetBackground(.4, .5, .6);

	vtkSmartPointer<vtkRenderer> maskedRenderer2 =
		vtkSmartPointer<vtkRenderer>::New();
	maskedRenderer2->SetViewport(maskedViewport2);
	maskedRenderer2->AddActor(maskedActor2);
	maskedRenderer2->ResetCamera();
	maskedRenderer2->SetBackground(.4, .5, .6);

	vtkSmartPointer<vtkRenderer> maskedRendererLogic =
		vtkSmartPointer<vtkRenderer>::New();
	maskedRendererLogic->SetViewport(maskedViewportLogic);
	maskedRendererLogic->AddActor(logicActor);
	maskedRendererLogic->ResetCamera();
	maskedRendererLogic->SetBackground(.4, .5, .6);

	vtkSmartPointer<vtkRenderer> maskedRendererBlend =
		vtkSmartPointer<vtkRenderer>::New();
	maskedRendererBlend->SetViewport(maskedViewportBlend);
	maskedRendererBlend->AddActor(maskedActor1);
	maskedRendererBlend->AddActor(maskedActor2);
	maskedRendererBlend->ResetCamera();
	maskedRendererBlend->SetBackground(.4, .5, .6);
	
	vtkSmartPointer<vtkRenderWindow> renderWindow =
		vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->SetSize(1000, 250);
	renderWindow->AddRenderer(originalRenderer);
	renderWindow->AddRenderer(maskedRenderer1);
	renderWindow->AddRenderer(maskedRenderer2);
	renderWindow->AddRenderer(maskedRendererLogic);
	renderWindow->AddRenderer(maskedRendererBlend);

	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	vtkSmartPointer<opacityVtkInteractorStyleImage> style = vtkSmartPointer<opacityVtkInteractorStyleImage>::New();
	style->SetRendererWindow(renderWindow);
	style->SetActors(maskedActor1, maskedActor2);
	renderWindowInteractor->SetInteractorStyle(style);

	renderWindowInteractor->SetRenderWindow(renderWindow);
	renderWindowInteractor->Initialize();

	renderWindowInteractor->Start();
}
//////////////////////////////////////////////////////////////////////////

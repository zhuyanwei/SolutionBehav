#include "BackAver.h"

BackAver::BackAver()
{
}

BackAver::~BackAver()
{
}

int BackAver::funMain(string fileName)
{
	CvCapture* capture;
	if (fileName == "00")
	{
		capture = cvCreateCameraCapture(0);	//初始化从摄像机中获取视频
		//@J,judge if thera is a camera
		if (!capture)
		{
			printf("Couldn't Open the file.");
			return -1;
		}
	}
	else
	{
		char* myChar = const_cast<char*>(fileName.c_str());
		capture = cvCreateFileCapture(myChar);
		if (NULL == capture)
		{
			printf("没有找到该视频！\n");
			return -1;
		}

	}

	cvNamedWindow("raw");
	cvNamedWindow("mask");
	//namedWindow("process", 1);
	IplImage* rawImage = cvQueryFrame(capture);		//这个函数仅仅是函数cvGrabFrame和函数cvRetrieveFrame在一起调用的组合
	cvShowImage("raw", rawImage);					//显示第一帧图像
	//@J,wait for camera to init
	while (rawImage == NULL)
	{
		cvWaitKey(1000);
		rawImage = cvQueryFrame(capture);
	}
	AllocateImages(rawImage);						//以第一帧图像分配图像大小		
	//***************@J,set ROI
	Mat roi1 = Mat(rawImage);
	Mat dst1 = Mat::zeros(roi1.size(), roi1.type());
	Mat mask1 = Mat::zeros(roi1.size(), CV_8U);
	//circle
	//Point circleCenter(mask1.cols / 2, mask1.rows / 2);
	//int radius = min(mask1.cols, mask1.rows) / 2;
	//circle(mask1, circleCenter, radius, Scalar(255), -1);
	//multi rect
	vector<vector<Point>> contour;
	vector<Point> pts;
	pts.push_back(Point(30, 60));
	pts.push_back(Point(500, 60));
	pts.push_back(Point(500, 450));
	pts.push_back(Point(30, 450));
	contour.push_back(pts);
	drawContours(mask1, contour, 0, Scalar::all(255), -1);
	//copy to mask
	roi1.copyTo(dst1, mask1);
	rawImage = &IplImage(dst1);
	//********************
	for (int i = 0;; i++)
	{
		if (i <= NUM_AVER)
		{
			cout << i<<'\n';
			accumulateBackground(rawImage);		//前30帧用于累积计算背景图像	
			if (i == NUM_AVER)								//将前30帧转换成一个背景统计模型	
				createModelsfromStats();
		}
		else
		{
			backgroundDiff(rawImage);
			process();
		}
		
		cvShowImage("mask", Imask);
		if (mmask.empty() == false)
			imshow("process", mmask);

		if (cvWaitKey(CVWAIT) == 'q')						
			break;
		if (!(rawImage = cvQueryFrame(capture)))
			break;
		drawContours(Mat(rawImage), contour, 0, Scalar::all(255), 1);
		cvShowImage("raw", rawImage);	
		//**********roi
		roi1 = Mat(rawImage);
		roi1.copyTo(dst1, mask1);
		rawImage = &IplImage(dst1);
		//************
	}
	DeallocateImages();								//撤销内存

	return 1;
}

/*该函数为需要的所有临时图像分配内存，传入来自视频的首帧图像作为大小参考*/
void BackAver::AllocateImages(IplImage* I)
{
	CvSize sz = cvGetSize(I);
	/*Float 3-channel images*/
	IavgF = cvCreateImage(sz, IPL_DEPTH_32F, 3);
	IdiffF = cvCreateImage(sz, IPL_DEPTH_32F, 3);
	IprevF = cvCreateImage(sz, IPL_DEPTH_32F, 3);
	IhiF = cvCreateImage(sz, IPL_DEPTH_32F, 3);
	IlowF = cvCreateImage(sz, IPL_DEPTH_32F, 3);
	cvZero(IavgF);
	cvZero(IdiffF);
	cvZero(IprevF);
	cvZero(IhiF);
	cvZero(IlowF);

	/*Float 1-channel images*/
	Ilow1 = cvCreateImage(sz, IPL_DEPTH_32F, 1);
	Ilow2 = cvCreateImage(sz, IPL_DEPTH_32F, 1);
	Ilow3 = cvCreateImage(sz, IPL_DEPTH_32F, 1);
	Ihi1 = cvCreateImage(sz, IPL_DEPTH_32F, 1);
	Ihi2 = cvCreateImage(sz, IPL_DEPTH_32F, 1);
	Ihi3 = cvCreateImage(sz, IPL_DEPTH_32F, 1);

	Icount = 1e-5;

	/*Float 3-channel images*/
	Iscratch = cvCreateImage(sz, IPL_DEPTH_32F, 3);
	Iscratch2 = cvCreateImage(sz, IPL_DEPTH_32F, 3);
	cvZero(Iscratch);
	cvZero(Iscratch2);

	/*Float 1-channel images*/
	Igray1 = cvCreateImage(sz, IPL_DEPTH_32F, 1);
	Igray2 = cvCreateImage(sz, IPL_DEPTH_32F, 1);
	Igray3 = cvCreateImage(sz, IPL_DEPTH_32F, 1);

	/*Byte 1-channel image*/
	Imaskt = cvCreateImage(sz, IPL_DEPTH_8U, 1);
	Imask = cvCreateImage(sz, IPL_DEPTH_8U, 1);
	IProc = cvCreateImage(sz, IPL_DEPTH_8U, 1);
}

//@J,calcu both addsum and diffs,to $IavgF and $IdiffF
/*累积背景图像和前后帧图像差值的绝对值，当累积够一定数量后就将其转换成一个背景统计模型*/
void BackAver::accumulateBackground(IplImage* I)
{
	static int first = 1;							//局部静态变量,只初始化一次,意思就是第一次被赋值为1
	cvCvtScale(I, Iscratch, 1, 0);				//即cvConvertScale，dst(I)=src(I)*scale+(shift,shift,...)，此处Iscratch(I)=I(I)*1+0
	//将I指向的图像复制给Iscratch 不能用cvCopy,因为像素的位深度不同
	if (!first)										//刚开始，first=1，不进入
	{
		cvAcc(Iscratch, IavgF);					//累积原始的浮点图像到IIavgF
		cvAbsDiff(Iscratch, IprevF, Iscratch2);	//计算前后帧图像绝对差图像到Iscratch2
		cvAcc(Iscratch2, IdiffF);					//将前后帧差值图像累加到IdiffF中	
		Icount += 1.0;								//记录累加的次数用于背景统计时计算均值		
	}
	first = 0;										//first为局部静态变量,，以后调用该函数将不再初始化为1，意思就是除了第一次，以后调用该函数均进入if语句
	cvCopy(Iscratch, IprevF);						//IprevF用来保存前一帧图像
}

/*当累积足够多的帧图后,就将其转化成一个背景统计模型，该函数用于计算每个像素的均值和平均绝对差分*/
void BackAver::createModelsfromStats()
{
	cvConvertScale(IavgF, IavgF, (double)(1.0 / Icount));	//计算平均原始图像到 IavgF：IavgF = IavgF/Icount
	cvConvertScale(IdiffF, IdiffF, (double)(1.0 / Icount));	//计算绝对差分图像到 IdiffF：IdiffF = IdiffF/Icount
	cvAddS(IdiffF, cvScalar(1.0, 1.0, 1.0), IdiffF);		//使得到的绝对差分图像每个像素值均不为空，故每个像素加上1
	setHighThreshold(THR_ALL);									//根据统计的背景模型设定一个阀值上限和下限	
	setLowThreshold(THR_ALL);									//如果 IlowF<=Temp<IhiF 时认为其为背景,否则为视频中出现的运动目标物体	
	cout << "background done";
}

//@J,IhiF = IavgF + IdiffF * scale
/*设置高阈值*/
void BackAver::setHighThreshold(float scale)
{
	cvConvertScale(IdiffF, Iscratch, scale);		//将统计的绝对差分图像值放大scale倍赋给Iscratch：Iscratch = IdiffF*scale
	cvAdd(Iscratch, IavgF, IhiF);					//IhiF = Iscratch + IavgF
	cvSplit(IhiF, Ihi1, Ihi2, Ihi3, 0);			//将阀值上限分割为多通道	
}

//@J,IlowF = IavgF - IdiffF * scale
/*设置低阈值*/
void BackAver::setLowThreshold(float scale)
{
	cvConvertScale(IdiffF, Iscratch, scale);		//Iscratch = IdiffF*scale
	cvSub(IavgF, Iscratch, IlowF);				//IlowF = IavgF - Iscratch
	cvSplit(IlowF, Ilow1, Ilow2, Ilow3, 0);		//将阀值下限分割为多通道
}

void BackAver::backgroundDiff(IplImage* I)
{
	cvCvtScale(I, Iscratch, 1, 0);				//将I指向的图像复制给Iscratch 不能用cvCopy, 因为像素的位深度不同
	cvSplit(Iscratch, Igray1, Igray2, Igray3, 0);	//得到的当前帧分割成3个单通道图像	
	cvInRange(Igray1, Ilow1, Ihi1, Imask);		//检查这些单通道图像是否在平均背景像素高低阀值之间
	//         src     lower  upper dst				  如果src(I)在范围内(lower<=src<upper)，dst(I)被设置为0xff(每一位都是'1')否则置0
	cvInRange(Igray2, Ilow2, Ihi2, Imaskt);
	cvOr(Imask, Imaskt, Imask);					//计算两个数组每个元素的按位或值赋值给第三个参数
	cvInRange(Igray3, Ilow3, Ihi3, Imaskt);
	cvOr(Imask, Imaskt, Imask);					//最后Imask为分离出的前景二值图
	cvSubRS(Imask, cvScalar(255), Imask);			//计算数量和数组之间的差,将Imask反相处理
}

int BackAver::process()
{
	Mat tmp(Imask);
	tmp.copyTo(mmask);
	medianBlur(mmask, mmask, 3);
	morphologyEx(mmask, mmask, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1)), Point(-1, -1), 2, BORDER_REPLICATE);
	return 1;
}

/*解除分配的内存*/
void BackAver::DeallocateImages()
{
	cvReleaseImage(&IavgF);
	cvReleaseImage(&IdiffF);
	cvReleaseImage(&IprevF);
	cvReleaseImage(&IhiF);
	cvReleaseImage(&IlowF);
	cvReleaseImage(&Ilow1);
	cvReleaseImage(&Ilow2);
	cvReleaseImage(&Ilow3);
	cvReleaseImage(&Ihi1);
	cvReleaseImage(&Ihi2);
	cvReleaseImage(&Ihi3);
	cvReleaseImage(&Iscratch);
	cvReleaseImage(&Iscratch2);
	cvReleaseImage(&Igray1);
	cvReleaseImage(&Igray2);
	cvReleaseImage(&Igray3);
	cvReleaseImage(&Imaskt);
	cvReleaseImage(&Imask);
	cvReleaseImage(&IProc);
}


#include "BackAver5.h"

BackAver5::BackAver5()
{
}

BackAver5::~BackAver5()
{
}

// 创建一个函数来给需要的所有临时图像分配内存
//为了方便，我们传递一幅图像(来自视频)作为大小参考来分配临时图像
void BackAver5::AllocateImages(IplImage* I)
{
	CvSize sz = cvGetSize(I);
	IavgF = cvCreateImage(sz, IPL_DEPTH_32F, 3);
	IdiffF = cvCreateImage(sz, IPL_DEPTH_32F, 3);
	IprevF = cvCreateImage(sz, IPL_DEPTH_32F, 3);
	IhiF = cvCreateImage(sz, IPL_DEPTH_32F, 3);
	IlowF = cvCreateImage(sz, IPL_DEPTH_32F, 3);

	Ilow1 = cvCreateImage(sz, IPL_DEPTH_32F, 1);
	Ilow2 = cvCreateImage(sz, IPL_DEPTH_32F, 1);
	Ilow3 = cvCreateImage(sz, IPL_DEPTH_32F, 1);
	Ihi1 = cvCreateImage(sz, IPL_DEPTH_32F, 1);
	Ihi2 = cvCreateImage(sz, IPL_DEPTH_32F, 1);
	Ihi3 = cvCreateImage(sz, IPL_DEPTH_32F, 1);
	cvZero(IavgF);
	cvZero(IdiffF);
	cvZero(IprevF);
	cvZero(IhiF);
	cvZero(IlowF);
	Icount = 0.00001;

	Iscratch = cvCreateImage(sz, IPL_DEPTH_32F, 3);
	Iscratch2 = cvCreateImage(sz, IPL_DEPTH_32F, 3);

	Igray1 = cvCreateImage(sz, IPL_DEPTH_32F, 1);
	Igray2 = cvCreateImage(sz, IPL_DEPTH_32F, 1);
	Igray3 = cvCreateImage(sz, IPL_DEPTH_32F, 1);

	Imask = cvCreateImage(sz, IPL_DEPTH_8U, 1);
	Imaskt = cvCreateImage(sz, IPL_DEPTH_8U, 1);

	cvZero(Iscratch);
	cvZero(Iscratch2);
}

//学习累积背景图像和每一帧图像差值的绝对值
// Learn the background statistics for one more frame
// I is a color sample of the background, 3-channel, 8u
void BackAver5::accumulateBackground(IplImage *I)
{
	static int first = 1;
	cvCvtScale(I, Iscratch, 1, 0);
	if (!first)
	{
		cvAcc(Iscratch, IavgF);
		cvAbsDiff(Iscratch, IprevF, Iscratch2);
		cvAcc(Iscratch2, IdiffF);
		Icount += 1.0;
	}
	first = 0;
	cvCopy(Iscratch, IprevF);
}

//setHighThreshold和setLowThreshold都是基于每一帧图像平均绝对差设置阈值的有效函数
void BackAver5::setHighThreshold(float scale)
{
	cvConvertScale(IdiffF, Iscratch, scale);
	cvAdd(Iscratch, IavgF, IhiF);
	cvSplit(IhiF, Ihi1, Ihi2, Ihi3, 0);
}
void BackAver5::setLowThreshold(float scale)
{
	cvConvertScale(IdiffF, Iscratch, scale);
	cvSub(IavgF, Iscratch, IlowF);
	cvSplit(IlowF, Ilow1, Ilow2, Ilow3, 0);
}

//当积累了足够多的帧图像之后，就将其转化为一个背景的统计模型
//计算每一个像素的均值和方差观测
void BackAver5::createModelsfromStats()
{
	cvConvertScale(IavgF, IavgF, (double)(1.0 / Icount));
	cvConvertScale(IdiffF, IdiffF, (double)(1.0 / Icount));

	//Make sure diff is always something

	cvAddS(IdiffF, cvScalar(1.0, 1.0, 1.0), IdiffF);
	setHighThreshold(7.0);
	setLowThreshold(6.0);
}

//有了背景模型，同时给出了高，低阈值，就能用它将图像分割为前景和背景
// Create a binary: 0,255 mask where 255 means foregrond pixel
// I Input image, 3-channel, 8u
//Imask  
void BackAver5::backgroundDiff(IplImage* I)
{
	cvCvtScale(I, Iscratch, 1, 0);
	cvSplit(Iscratch, Igray1, Igray2, Igray3, 0);

	//Channel 1
	cvInRange(Igray1, Ilow1, Ihi1, Imask);

	//Channel 2
	cvInRange(Igray2, Ilow2, Ihi2, Imaskt);
	cvOr(Imask, Imaskt, Imask);

	//Channel 3
	cvInRange(Igray3, Ilow3, Ihi3, Imaskt);
	cvOr(Imask, Imaskt, Imask);

	//Finally, invert the result
	cvSubRS(Imask, cvScalar(255), Imask);
}

//完成背景建模后， 释放内存
void BackAver5::DeallocateImage()
{
	cvReleaseImage(&IavgF);
	cvReleaseImage(&IdiffF);
	cvReleaseImage(&IprevF);
	cvReleaseImage(&IhiF);
	cvReleaseImage(&IlowF);
	cvReleaseImage(&Ilow1);
	cvReleaseImage(&Ilow2);
	cvReleaseImage(&Ilow3);
	cvReleaseImage(&Iscratch);
	cvReleaseImage(&Iscratch2);
	cvReleaseImage(&Igray1);
	cvReleaseImage(&Igray2);
	cvReleaseImage(&Igray3);
	cvReleaseImage(&Imaskt);
}

//主函数
int BackAver5::funMain(string fileName)
{
	char* myChar = const_cast<char*>(fileName.c_str());
	CvCapture* capture = cvCreateFileCapture(myChar);
	if (!capture)
	{
		return -1;
	}
	cvNamedWindow("win1");
	cvNamedWindow("win2");

	IplImage* rawImage = cvQueryFrame(capture);
	cvShowImage("win1", rawImage);

	AllocateImages(rawImage);
	int i = 0;
	while (1)
	{
		if (i <= 30)
		{
			accumulateBackground(rawImage);
			if (i == 30)
			{
				createModelsfromStats();
			}
		}
		else
		{
			backgroundDiff(rawImage);
		}
		cvShowImage("win2", Imask);

		if (cvWaitKey(33) == 27)
		{
			break;
		}
		if (!(rawImage = cvQueryFrame(capture)))
		{
			break;
		}
		cvShowImage("win1", rawImage);
		//if (i == 56 || i == 63)
		//	cvWaitKey();
		i = i + 1;
		cout << i<<'\n';
	}
	cout << "end while";
	DeallocateImage();
	return 1;
}
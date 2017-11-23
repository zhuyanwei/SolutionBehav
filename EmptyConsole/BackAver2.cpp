#include "BackAver2.h"

IplImage *IavgF[NUM_CAMERAS], *IdiffF[NUM_CAMERAS], *IprevF[NUM_CAMERAS], *IhiF[NUM_CAMERAS], *IlowF[NUM_CAMERAS];
IplImage *Iscratch, *Iscratch2, *Igray1, *Igray2, *Igray3, *Imaskt;
IplImage *Ilow1[NUM_CAMERAS], *Ilow2[NUM_CAMERAS], *Ilow3[NUM_CAMERAS], *Ihi1[NUM_CAMERAS], *Ihi2[NUM_CAMERAS], *Ihi3[NUM_CAMERAS];

float Icount[NUM_CAMERAS];

BackAver2::BackAver2()
{
}

BackAver2::~BackAver2()
{
}

void BackAver2::AllocateImages(IplImage *I)  //I is just a sample for allocation purposes
{
	for (int i = 0; i<NUM_CAMERAS; i++){
		IavgF[i] = cvCreateImage(cvGetSize(I), IPL_DEPTH_32F, 3);
		IdiffF[i] = cvCreateImage(cvGetSize(I), IPL_DEPTH_32F, 3);
		IprevF[i] = cvCreateImage(cvGetSize(I), IPL_DEPTH_32F, 3);
		IhiF[i] = cvCreateImage(cvGetSize(I), IPL_DEPTH_32F, 3);
		IlowF[i] = cvCreateImage(cvGetSize(I), IPL_DEPTH_32F, 3);
		Ilow1[i] = cvCreateImage(cvGetSize(I), IPL_DEPTH_32F, 1);
		Ilow2[i] = cvCreateImage(cvGetSize(I), IPL_DEPTH_32F, 1);
		Ilow3[i] = cvCreateImage(cvGetSize(I), IPL_DEPTH_32F, 1);
		Ihi1[i] = cvCreateImage(cvGetSize(I), IPL_DEPTH_32F, 1);
		Ihi2[i] = cvCreateImage(cvGetSize(I), IPL_DEPTH_32F, 1);
		Ihi3[i] = cvCreateImage(cvGetSize(I), IPL_DEPTH_32F, 1);
		cvZero(IavgF[i]);
		cvZero(IdiffF[i]);
		cvZero(IprevF[i]);
		cvZero(IhiF[i]);
		cvZero(IlowF[i]);
		Icount[i] = 0.00001; //Protect against divide by zero
	}
	Iscratch = cvCreateImage(cvGetSize(I), IPL_DEPTH_32F, 3);
	Iscratch2 = cvCreateImage(cvGetSize(I), IPL_DEPTH_32F, 3);
	Igray1 = cvCreateImage(cvGetSize(I), IPL_DEPTH_32F, 1);
	Igray2 = cvCreateImage(cvGetSize(I), IPL_DEPTH_32F, 1);
	Igray3 = cvCreateImage(cvGetSize(I), IPL_DEPTH_32F, 1);
	Imaskt = cvCreateImage(cvGetSize(I), IPL_DEPTH_8U, 1);

	cvZero(Iscratch);
	cvZero(Iscratch2);
}

void BackAver2::DeallocateImages()
{
	for (int i = 0; i<NUM_CAMERAS; i++){
		cvReleaseImage(&IavgF[i]);
		cvReleaseImage(&IdiffF[i]);
		cvReleaseImage(&IprevF[i]);
		cvReleaseImage(&IhiF[i]);
		cvReleaseImage(&IlowF[i]);
		cvReleaseImage(&Ilow1[i]);
		cvReleaseImage(&Ilow2[i]);
		cvReleaseImage(&Ilow3[i]);
		cvReleaseImage(&Ihi1[i]);
		cvReleaseImage(&Ihi2[i]);
		cvReleaseImage(&Ihi3[i]);
	}
	cvReleaseImage(&Iscratch);
	cvReleaseImage(&Iscratch2);

	cvReleaseImage(&Igray1);
	cvReleaseImage(&Igray2);
	cvReleaseImage(&Igray3);

	cvReleaseImage(&Imaskt);
}

// Accumulate the background statistics for one more frame
// We accumulate the images, the image differences and the count of images for the 
//    the routine createModelsfromStats() to work on after we're done accumulating N frames.
// I        Background image, 3 channel, 8u
// number    Camera number
void BackAver2::accumulateBackground(IplImage *I, int number)
{
	static int first = 1;
	cvCvtScale(I, Iscratch, 1, 0); //To float;#define cvCvtScale cvConvertScale #define cvScale cvConvertScale
	if (!first){
		cvAcc(Iscratch, IavgF[number]);//将2幅图像相加：IavgF[number]=IavgF[number]+Iscratch，IavgF[]里面装的是时间序列图片的累加
		cvAbsDiff(Iscratch, IprevF[number], Iscratch2);//将2幅图像相减：Iscratch2=abs(Iscratch-IprevF[number]);
		cvAcc(Iscratch2, IdiffF[number]);//IdiffF[]里面装的是图像差的累积和
		Icount[number] += 1.0;//累积的图片帧数计数
	}
	first = 0;
	cvCopy(Iscratch, IprevF[number]);//执行完该函数后，将当前帧数据保存为前一帧数据
}

// Scale the average difference from the average image high acceptance threshold
void BackAver2::scaleHigh(float scale, int num)//设定背景建模时的高阈值函数
{
	cvConvertScale(IdiffF[num], Iscratch, scale); //Converts with rounding and saturation
	cvAdd(Iscratch, IavgF[num], IhiF[num]);//将平均累积图像与误差累积图像缩放scale倍然后再相加
	cvCvtPixToPlane(IhiF[num], Ihi1[num], Ihi2[num], Ihi3[num], 0);//#define cvCvtPixToPlane cvSplit,且cvSplit是将一个多通道矩阵转换为几个单通道矩阵
}

// Scale the average difference from the average image low acceptance threshold
void BackAver2::scaleLow(float scale, int num)//设定背景建模时的低阈值函数
{
	cvConvertScale(IdiffF[num], Iscratch, scale); //Converts with rounding and saturation
	cvSub(IavgF[num], Iscratch, IlowF[num]);//将平均累积图像与误差累积图像缩放scale倍然后再相减
	cvCvtPixToPlane(IlowF[num], Ilow1[num], Ilow2[num], Ilow3[num], 0);
}

//Once you've learned the background long enough, turn it into a background model
void BackAver2::createModelsfromStats()
{
	for (int i = 0; i<NUM_CAMERAS; i++)
	{
		cvConvertScale(IavgF[i], IavgF[i], (double)(1.0 / Icount[i]));//此处为求出累积求和图像的平均值
		cvConvertScale(IdiffF[i], IdiffF[i], (double)(1.0 / Icount[i]));//此处为求出累计误差图像的平均值
		cvAddS(IdiffF[i], cvScalar(1.0, 1.0, 1.0), IdiffF[i]);  //Make sure diff is always something，cvAddS是用于一个数值和一个标量相加
		scaleHigh(HIGH_SCALE_NUM, i);//HIGH_SCALE_NUM初始定义为7，其实就是一个倍数
		scaleLow(LOW_SCALE_NUM, i);//LOW_SCALE_NUM初始定义为6
	}
}

// Create a binary: 0,255 mask where 255 means forground pixel
// I        Input image, 3 channel, 8u
// Imask    mask image to be created, 1 channel 8u
// num        camera number.
//
void BackAver2::backgroundDiff(IplImage *I, IplImage *Imask, int num)  //Mask should be grayscale
{
	cvCvtScale(I, Iscratch, 1, 0); //To float;
	//Channel 1
	cvCvtPixToPlane(Iscratch, Igray1, Igray2, Igray3, 0);
	cvInRange(Igray1, Ilow1[num], Ihi1[num], Imask);//Igray1[]中相应的点在Ilow1[]和Ihi1[]之间时，Imask中相应的点为255(背景符合)
	//Channel 2
	cvInRange(Igray2, Ilow2[num], Ihi2[num], Imaskt);//也就是说对于每一幅图像的绝对值差小于绝对值差平均值的6倍或者大于绝对值差平均值的7倍被认为是前景图像
	cvOr(Imask, Imaskt, Imask);
	//Channel 3
	cvInRange(Igray3, Ilow3[num], Ihi3[num], Imaskt);//这里的固定阈值6和7太不合理了，还好工程后面可以根据实际情况手动调整！
	cvOr(Imask, Imaskt, Imask);
	//Finally, invert the results
	cvSubRS(Imask, cvScalar(255), Imask);//前景用255表示了，背景是用0表示
}

int BackAver2::script()
{
	return 1;
}

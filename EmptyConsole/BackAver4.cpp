#include "BackAver4.h"

BackAver4::BackAver4()
{
}

BackAver4::~BackAver4()
{
}

int BackAver4::funMain(string fileName)
{
	char* myChar = const_cast<char*>(fileName.c_str());
	CvCapture *capture = cvCreateFileCapture(myChar);
	IplImage *myImg = cvQueryFrame(capture);
	CvSize img_sz = cvGetSize(myImg);
	IplImage *pic;
	IplImage *frame = cvCreateImage(img_sz, 8, 1);
	IplImage *frame2 = cvCreateImage(img_sz, 8, 1);
	IplImage *framedn = cvCreateImage(img_sz, 8, 1);
	IplImage *Imask = cvCreateImage(img_sz, IPL_DEPTH_8U, 1);
	IplImage *Iscratch = cvCreateImage(img_sz, IPL_DEPTH_32F, 1);
	IplImage *IavgF = cvCreateImage(img_sz, IPL_DEPTH_32F, 1);
	IplImage *IpreF = cvCreateImage(img_sz, IPL_DEPTH_32F, 1);
	IplImage *Iscratch2 = cvCreateImage(img_sz, IPL_DEPTH_32F, 1);
	IplImage *IdiffF = cvCreateImage(img_sz, IPL_DEPTH_32F, 1);
	IplImage *IhiF = cvCreateImage(img_sz, IPL_DEPTH_32F, 1);
	IplImage *IlowF = cvCreateImage(img_sz, IPL_DEPTH_32F, 1);
	int fnum = 1, Icount = 100, ht = 5;
	cvZero(IavgF); cvZero(Iscratch); cvZero(IpreF); cvZero(Iscratch2); cvZero(IdiffF);

	while (fnum <= Icount)
	{
		pic = cvQueryFrame(capture);
		if (!pic) 
			break;
		if (pic->depth != 1)
			cvCvtColor(pic, frame, CV_BGR2GRAY);
		cvSmooth(frame, framedn, CV_BLUR, 15);
		cvCvtScale(framedn, Iscratch, 1.0, 0);
		cvAcc(Iscratch, IavgF);
		cvAbsDiff(Iscratch, IpreF, Iscratch2);
		cvAcc(Iscratch2, IdiffF);
		cvCopy(Iscratch, IpreF);
		fnum++;
	}
	cvConvertScale(IavgF, IavgF, (double)(1.0 / (Icount)));   /*均值*/
	cvConvertScale(IdiffF, IdiffF, (double)(1.0 / (Icount)));  /*帧间插值*/
	cvAddS(IdiffF, cvScalar(1.0), IdiffF);
	cvConvertScale(IdiffF, Iscratch, ht); /*high threshold = low threshold*/
	cvAdd(IdiffF, IavgF, IhiF);
	cvSub(IavgF, Iscratch, IlowF);

	while (true)
	{
		pic = cvQueryFrame(capture);
		if (!pic) 
			break;
		if (pic->depth != 1)
			cvCvtColor(pic, frame, CV_BGR2GRAY);
		cvSmooth(frame, framedn, CV_BLUR, 15);
		cvCvtScale(framedn, Iscratch, 1.0, 0);
		cvInRange(Iscratch, IlowF, IhiF, Imask);
		cvSubRS(Imask, cvScalar(255), Imask);
		cvNamedWindow("mask");
		cvShowImage("mask", Imask);
		cvNamedWindow("frame");
		cvShowImage("frame", frame);
		cvWaitKey(33);
	}
	return 1;
}
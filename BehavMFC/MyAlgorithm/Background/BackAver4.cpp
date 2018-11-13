#include "BackAver4.h"

BackAver4::BackAver4()
{
}

BackAver4::~BackAver4()
{
}

int BackAver4::funMain(string fileName)
{
	//open camera
	CvCapture* capture;
	if (fileName == "00")
	{
		capture = cvCreateCameraCapture(0);
	}
	else
	{
		char* myChar = const_cast<char*>(fileName.c_str());
		capture = cvCreateFileCapture(myChar);
	}
	if (!capture)
	{
		printf("Couldn't Open the file or camera");
		return -1;
	}

	IplImage *myImg = cvQueryFrame(capture);
	while (myImg == NULL)
	{
		cvWaitKey(1000);
		myImg = cvQueryFrame(capture);
	}
	//allocate
	CvSize img_sz = cvGetSize(myImg);
	IplImage *pic;
	IplImage *frame = cvCreateImage(img_sz, 8, 1);
	IplImage *frame2 = cvCreateImage(img_sz, 8, 1);
	IplImage *framedn = cvCreateImage(img_sz, 8, 1);
	Imask = cvCreateImage(img_sz, IPL_DEPTH_8U, 1);
	IplImage *Iscratch = cvCreateImage(img_sz, IPL_DEPTH_32F, 1);
	IplImage *IavgF = cvCreateImage(img_sz, IPL_DEPTH_32F, 1);
	IplImage *IpreF = cvCreateImage(img_sz, IPL_DEPTH_32F, 1);
	IplImage *Iscratch2 = cvCreateImage(img_sz, IPL_DEPTH_32F, 1);
	IplImage *IdiffF = cvCreateImage(img_sz, IPL_DEPTH_32F, 1);
	IplImage *IhiF = cvCreateImage(img_sz, IPL_DEPTH_32F, 1);
	IplImage *IlowF = cvCreateImage(img_sz, IPL_DEPTH_32F, 1);
	//edge part
	IplImage *edgeIn = cvCreateImage(img_sz, 8, 1);
	IplImage *edgeOut = cvCreateImage(img_sz, 8, 1);
	//numbers
	int fnum = 1, Icount = NUM_AVER;
	cvZero(IavgF); cvZero(Iscratch); cvZero(IpreF); cvZero(Iscratch2); cvZero(IdiffF);
	//creat background
	while (fnum <= Icount)
	{
		//get origin and gray
		pic = cvQueryFrame(capture);
		if (!pic) 
			break;
		if (pic->nChannels != 1)
			cvCvtColor(pic, frame, CV_BGR2GRAY);
		//edge
		cvCvtScale(frame, edgeIn);
		e1.funMain("can", edgeIn, edgeOut);
		//acculate
		cvCvtScale(edgeOut, Iscratch, 1.0, 0);
		cvAcc(Iscratch, IavgF);
		cvAbsDiff(Iscratch, IpreF, Iscratch2);
		cvAcc(Iscratch2, IdiffF);
		cvCopy(Iscratch, IpreF);
		cout << fnum << '\n';
		fnum++;
		//if (cvWaitKey(CVWAIT) == 'q')
		//	break;
	}
	cvConvertScale(IavgF, IavgF, (double)(1.0 / (Icount)));   /*均值*/
	cvConvertScale(IdiffF, IdiffF, (double)(1.0 / (Icount)));  /*帧间插值*/
	cvAddS(IdiffF, cvScalar(1.0), IdiffF);
	cvConvertScale(IdiffF, Iscratch, THR_ALL); /*high threshold = low threshold*/
	cvAdd(IavgF, Iscratch, IhiF);
	cvSub(IavgF, Iscratch, IlowF);
	//cvShowImage("Background", IavgF);
	cout << "background done" << '\n';
	//do sub
	while (true)
	{
		//get and gray
		pic = cvQueryFrame(capture);
		if (!pic) 
			break;
		if (pic->nChannels != 1)
			cvCvtColor(pic, frame, CV_BGR2GRAY);
		//edge
		cvCvtScale(frame, edgeIn);
		e1.funMain("can",edgeIn,edgeOut);
		//do range
		cvCvtScale(edgeOut,Iscratch);
		cvInRange(Iscratch, IlowF, IhiF, Imask);
		cvSubRS(Imask, cvScalar(255), Imask);
		////smooth open&close
		//process();

		//flk process
		if (flkMat1.empty() == false && flkMat2.empty() == false)
			flk.funMain(flkMat1, flkMat1);
		cvShowImage("mask", Imask);
		cvShowImage("frame", frame);
		if (cvWaitKey(CVWAIT) == 'q')
			break;
	}
	//flk.releasePara();
	return 1;
}

int BackAver4::process_common()
{
	Mat tmp(Imask);
	tmp.copyTo(mmask);
	medianBlur(mmask, mmask, 3);
	morphologyEx(mmask, mmask, MORPH_CLOSE, getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1)), Point(-1, -1), 2, BORDER_REPLICATE);
	if (mmask.empty() == false)
	{
		imshow("process", mmask);
		mmask.copyTo(out_ba4);
		//get flk
		flkMat2.copyTo(flkMat1);
		mmask.copyTo(flkMat2);
		//cnt
		out_num = cntpix(mmask);
	}

	return 1;
}

int BackAver4::cntpix(Mat matin)
{
	int cnt = 0;
	for (int i = 0; i < matin.rows; i++)
	{
		uchar* data = matin.ptr<uchar>(i);
		for (int j = 0; j < matin.cols; j++)
		{
			if (data[j] != 0)
				cnt++;
		}
	}
	printf("%d*%d=%d\n", matin.rows, matin.cols,cnt);
	return cnt;
}


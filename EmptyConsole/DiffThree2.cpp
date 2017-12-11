#include "DiffThree2.h"

DiffThree2::DiffThree2()
{
}

DiffThree2::~DiffThree2()
{
}

int DiffThree2::script(string fileName)
{
	VideoCapture videoCap(fileName);
	if (!videoCap.isOpened())
	{
		return -1;
	}
	double videoFPS = videoCap.get(CV_CAP_PROP_FPS);  //获取帧率
	double videoPause = 1000 / videoFPS;
	Mat framePrePre; //上上一帧
	Mat framePre; //上一帧
	Mat frameNow; //当前帧
	Mat frameDet; //运动物体
	videoCap >> framePrePre;
	videoCap >> framePre;
	if (framePrePre.depth() != 1)
		cvtColor(framePrePre, framePrePre, CV_RGB2GRAY);
	if (framePre.depth() != 1)
		cvtColor(framePre, framePre, CV_RGB2GRAY);
	int save = 0;
	while (true)
	{
		videoCap >> frameNow;
		if (frameNow.empty() || waitKey(30) == 'q')
		//if (frameNow.empty() || waitKey(videoPause) == 'q')
		{
			break;
		}
		if (frameNow.depth() != 1)
			cvtColor(frameNow, frameNow, CV_RGB2GRAY);
		Mat Det1;
		Mat Det2;
		absdiff(framePrePre, framePre, Det1);  //帧差1
		absdiff(framePre, frameNow, Det2);     //帧差2
		threshold(Det1, Det1, 0, 255, CV_THRESH_OTSU);  //自适应阈值化
		threshold(Det2, Det2, 0, 255, CV_THRESH_OTSU);
		Mat element = getStructuringElement(0, Size(3, 3));  //膨胀核
		dilate(Det1, Det1, element);    //膨胀
		dilate(Det2, Det2, element);
		bitwise_and(Det1, Det2, frameDet);
		framePrePre = framePre;
		framePre = frameNow;
		imshow("Video", frameNow);
		imshow("Detection", frameDet);
	}
	return 0;
}

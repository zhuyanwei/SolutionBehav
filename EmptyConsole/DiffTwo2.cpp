#include "DiffTwo2.h"

DiffTwo2::DiffTwo2()
{
}

DiffTwo2::~DiffTwo2()
{
}

int DiffTwo2::script(string fileName)
{
	VideoCapture videoCap(fileName);
	if (!videoCap.isOpened())
	{
		return -1;
	}
	double videoFPS = videoCap.get(CV_CAP_PROP_FPS);  //获取帧率
	double videoPause = 1000 / videoFPS;

	Mat framePre; //上一帧
	Mat frameNow; //当前帧
	Mat frameDet; //运动物体
	videoCap >> framePre;
	if (framePre.depth() != 1)
		cvtColor(framePre, framePre, CV_RGB2GRAY);
	while (true)
	{
		videoCap >> frameNow;
		if (frameNow.empty() || waitKey(30) == 'q')
		{
			break;
		}
		if (framePre.depth() != 1)
			cvtColor(frameNow, frameNow, CV_RGB2GRAY);
		absdiff(frameNow, framePre, frameDet);
		framePre = frameNow;
		imshow("Video", frameNow);
		imshow("Detection", frameDet);
	}
	return 1;
}

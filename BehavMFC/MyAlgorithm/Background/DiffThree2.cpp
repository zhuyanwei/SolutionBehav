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
	double videoFPS = videoCap.get(CV_CAP_PROP_FPS);  //��ȡ֡��
	double videoPause = 1000 / videoFPS;
	Mat framePrePre; //����һ֡
	Mat framePre; //��һ֡
	Mat frameNow; //��ǰ֡
	Mat frameDet; //�˶�����
	videoCap >> framePrePre;
	videoCap >> framePre;
	if (framePrePre.depth() != 1)
		cvtColor(framePrePre, framePrePre, CV_RGB2GRAY);
	if (framePre.depth() != 1)
		cvtColor(framePre, framePre, CV_RGB2GRAY);
	int save = 0;
	while (true)
	{
		double TRoberts;
		TRoberts = static_cast<double>(getTickCount());

		videoCap >> frameNow;
		if (frameNow.empty() || waitKey(CVWAIT) == 'q')
		//if (frameNow.empty() || waitKey(videoPause) == 'q')
		{
			break;
		}
		if (frameNow.depth() != 1)
			cvtColor(frameNow, frameNow, CV_RGB2GRAY);
		Mat Det1;
		Mat Det2;
		absdiff(framePrePre, framePre, Det1);  //֡��1
		absdiff(framePre, frameNow, Det2);     //֡��2
		threshold(Det1, Det1, 0, 255, CV_THRESH_OTSU);  //����Ӧ��ֵ��
		threshold(Det2, Det2, 0, 255, CV_THRESH_OTSU);
		Mat element = getStructuringElement(0, Size(3, 3));  //���ͺ�
		dilate(Det1, Det1, element);    //����
		dilate(Det2, Det2, element);
		bitwise_and(Det1, Det2, frameDet);
		framePrePre = framePre;
		framePre = frameNow;
		//time
		TRoberts = static_cast<double>(getTickCount()) - TRoberts;
		TRoberts /= getTickFrequency();
		cout << TRoberts << '\n';

		imshow("Video", frameNow);
		imshow("Detection", frameDet);
	}
	return 0;
}

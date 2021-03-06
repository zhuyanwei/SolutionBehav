#include "FlowImg.h"

FlowImg::FlowImg()
{
}

FlowImg::~FlowImg()
{
}

int FlowImg::funOne()
{
	Mat I1;
	Mat I2;
	vector<Point2f> features;
	int maxCout = 50;//定义最大个数
	double minDis = 20;//定义最小距离
	double qLevel = 0.01;//定义质量水平

	I1 = imread("MySrc/lena.jpg",0);//读取为灰度图像
	cout << "info:" << I1.cols << ',' << I1.rows << '\n';
	goodFeaturesToTrack(I1, features, maxCout, qLevel, minDis);
	for (int i = 0; i<features.size(); i++)
	{
		//将特征点画一个小圆出来--粗细为2
		circle(I1, features[i], 3, Scalar(255), 1);
		cout <<i<<':'<< features[i].x << ',' << features[i].y << '\n';
	}
	imshow("features", I1);
	waitKey(0);
	return 0;
}

int FlowImg::funTwo()
{
	Mat I1;
	Mat I2;
	vector<Point2f> features;
	vector<Point2f> features_after;
	vector<uchar> status;
	vector<float> err;
	int maxCout = 300;//定义最大个数
	double minDis = 10;//定义最小距离
	double qLevel = 0.01;//定义质量水平
	//读取两个图像---相邻帧
	I1 = imread("MySrc/frame1.jpg", 0);//读取为灰度图像
	I2 = imread("MySrc/frame2.jpg", 0);
	//检测第一帧的特征点
	goodFeaturesToTrack(I1, features, maxCout, qLevel, minDis);
	//计算出第二帧的特征点
	calcOpticalFlowPyrLK(I1, I2, features, features_after, status, err);
	//判别哪些属于运动的特征点
	int k = 0;
	for (int i = 0; i<features_after.size(); i++)
	{
		//状态要是1，并且坐标要移动下的那些点
		if (status[i] && ((abs(features[i].x - features_after[i].x) + abs(features[i].y - features_after[i].y))>4) )
		{
			features_after[k++] = features_after[i];
		}
	}
	features_after.resize(k);//截取
	cout << k << endl;
	for (int i = 0; i<features_after.size(); i++)
	{
		//将特征点画一个小圆出来--粗细为2
		circle(I1, features_after[i], 3, Scalar(255), 2);
	}
	imshow("features", I1);
	waitKey(0);
	return 0;
}
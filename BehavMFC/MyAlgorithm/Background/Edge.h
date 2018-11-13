//roberts,Prewitt:https://blog.csdn.net/qq_20823641/article/details/52079628
//used--------myKirsch:http://lib.csdn.net/article/opencv/24630
//myKirsch2:https://blog.csdn.net/u010350978/article/details/44874063
//myKirsch3:https://blog.csdn.net/baidu_30028771/article/details/60884871
#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
#include "../../GlobalSetting.h"
using namespace std;
using namespace cv;

class Edge
{
public:
	Edge();
	~Edge();
	Mat funMain(string ch, Mat min, Mat mout);
	//my function
	Mat myRoberts(Mat srcImage);
	Mat myPrewitt(Mat imageP);
	int myKirsch(IplImage *src, IplImage *dst);
	void myKirsch2(const IplImage* src, CvMat* dst);

private:
	string choose;
};


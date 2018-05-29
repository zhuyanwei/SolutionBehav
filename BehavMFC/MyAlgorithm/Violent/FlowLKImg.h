//c-type;
//optical flow,LK,2 images
//http://blog.csdn.net/gdut2015go/article/details/46043821
#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
#include "../../GlobalSetting.h"
using namespace std;
using namespace cv;

const int MAX_CORNERS = 500;

class FlowLKImg
{
public:
	FlowLKImg();
	~FlowLKImg();
	int funMain(Mat mat1,Mat mat2);
	int releasePara();
	//outputs
	float out_lkValue;
private:
	//local para to release
	Mat mat3;
	IplImage* imgA;
	IplImage* imgB;
	CvSize img_sz;
	IplImage* imgC;
	IplImage* eig_image;
	IplImage* tmp_image;
	CvPoint2D32f* cornersA;
	CvSize pyr_sz;
	IplImage* pyrA;
	IplImage* pyrB;
	CvPoint2D32f* cornersB;
	Mat tmp;
};


//get feature;circle;
//optical flow,differ two images
//http://blog.csdn.net/on2way/article/details/48954159
#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

class FlowImg
{
public:
	FlowImg();
	~FlowImg();
	int funOne();
	int funTwo();
};


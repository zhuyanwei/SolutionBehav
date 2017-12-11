//useless
//c-type;line;
//flow lk video
//http://blog.csdn.net/w12345_ww/article/details/44925403
#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

const int MAX_CORNERS2 = 500;

class FlowLKVideo
{
public:
	FlowLKVideo();
	~FlowLKVideo();
	int funMain();
};


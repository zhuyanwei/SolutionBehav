//star5
//colour space;
//optical flow,use camera and colour space
//http://blog.csdn.net/zouxy09/article/details/8683859
#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

#define UNKNOWN_FLOW_THRESH 1e9

class FlowColor
{
public:
	FlowColor();
	~FlowColor();
	int funMain(string fileName);
private:
	void makecolorwheel(vector<Scalar> &colorwheel);
	void motionToColor(Mat flow, Mat &color);
};


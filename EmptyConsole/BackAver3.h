//useless
// background average ,-->how to accumulate a video file
#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

class BackAver3
{
public:
	BackAver3();
	~BackAver3();
	int funMain(string fileName);
};


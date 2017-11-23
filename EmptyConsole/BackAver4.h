// background average ,a more simple one,website with some explanation
//http://blog.csdn.net/mingtian715/article/details/53008316
#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

class BackAver4
{
public:
	BackAver4();
	~BackAver4();
	int funMain(string fileName);
};


//para:filename;number of sum;low thread;high thread;blur para;
// background average ,a more simple one,website with some explanation.more clear,use only one channel,gassion blur
//http://blog.csdn.net/mingtian715/article/details/53008316
#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
#include "../../GlobalSetting.h"
using namespace std;
using namespace cv;

class BackAver4
{
public:
	BackAver4();
	~BackAver4();
	int funMain(string fileName);
	//outputs
	Mat out_ba4;
	int out_num;
private:
	IplImage *Imask;
	Mat mmask;

	int process();
	int cntpix(Mat matin);
};


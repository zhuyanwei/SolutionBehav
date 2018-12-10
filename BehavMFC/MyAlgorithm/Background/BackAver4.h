//para:filename;number of sum;low thread;high thread;blur para;
// background average ,a more simple one,website with some explanation.more clear,use only one channel,gassion blur
//http://blog.csdn.net/mingtian715/article/details/53008316
//*********************
//*********************this contains the whole thread loop
#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
#include "../../GlobalSetting.h"
#include "../Hog/Sol3/Hog3.h"
#include "../Violent/FlowLKImg.h"
#include "Edge.h"
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

	int process_common();
	int cntpix(Mat matin);
	//algorithm
	//edge
	Edge e1;
	//HOG
	Hog3 hg3;
	//flk
	FlowLKImg flk;
	Mat flkMat1, flkMat2;
};


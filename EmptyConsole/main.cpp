// This is based on 
// "VIBE: A POWERFUL RANDOM TECHNIQUE TO ESTIMATE THE BACKGROUND IN VIDEO SEQUENCES"
// by Olivier Barnich and Marc Van Droogenbroeck
// Author : zouxy
// Date   : 2013-4-13
// HomePage : http://blog.csdn.net/zouxy09
// Email  : zouxy09@qq.com

#include "opencv2/opencv.hpp"
#include "ViBe_BGS.h"
#include <iostream>
#include <cstdio>

using namespace cv;
using namespace std;

int main()
{
	Mat frame, gray, mask;
	VideoCapture capture;
	capture.open("MySrc/Curtain.avi");

	if (!capture.isOpened())
	{
		cout << "No camera or video input!\n" << endl;
		return -1;
	}

	ViBe_BGS Vibe_Bgs;
	int count = 0;

	while (1)
	{
		count++;
		capture >> frame;
		if (frame.empty())
			continue;
		cvtColor(frame, gray, CV_RGB2GRAY);

		if (count == 1)
		{
			Vibe_Bgs.init(gray);
			Vibe_Bgs.processFirstFrame(gray);
			cout << " Training GMM complete!" << endl;
		}
		else
		{
			Vibe_Bgs.testAndUpdate(gray);
			mask = Vibe_Bgs.getMask();
			morphologyEx(mask, mask, MORPH_OPEN, Mat());
			imshow("mask", mask);
		}

		imshow("input", frame);
		waitKey(30);
	}

	return 0;
}
#include "opencv2/opencv.hpp"
#include <iostream>
#include <cstdio>
//background substract
#include "DiffTwo.h"
#include "DiffTwo2.h"
#include "DiffThree.h"
#include "DiffThree2.h"
#include "BackAver.h"
#include "BackAver4.h"
#include "ViBe_BGS.h"
//optical flow
#include "FlowColor.h"
#include "FlowImg.h"
#include "FlowLKImg.h"

int playFile();
int test();

int main()
{
	//playFile();
	//test();

	//DiffTwo d1;
	////d1.funMain("00");
	//d1.funMain("MySrc/sample.avi");

	//DiffTwo2 d22;
	//d22.script("MySrc/sample.avi");

	//DiffThree d3;
	//d3.script("MySrc/sample.avi");

	//DiffThree2 d32;
	//d32.script("MySrc/sample.avi");

	//BackAver ba1;
	//ba1.funMain();

	//BackAver4 ba4;
	//ba4.funMain("MySrc/sample.avi");

	//ViBe_BGS v1;
	//v1.script("00");

	//FlowColor f1;
	//f1.funMain("00");

	//FlowImg f2;
	////f2.funOne();
	//f2.funTwo();

	FlowLKImg f3;
	f3.funMain();

	return 0;
}

int playFile()
{
	VideoCapture capture;
	Mat frame;
	capture.open("MySrc/goout.flv");
	//capture.open(0);
	if (!capture.isOpened())
	{
		cout << "No camera or video input!\n" << endl;
		return -1;
	}
	while (true)
	{
		capture.read(frame);
		if (frame.empty())
			continue;
		imshow("File", frame);
		waitKey(30);
	}
	return 1;
}

int test()
{
	Mat m1;
	m1 = imread("MySrc/lena.jpg");
	imshow("test", m1);
	if (waitKey(10000) == 'q')
	{
		cout << "button";
		return 0;
	}
	cout << "no button";
	return 1;
}

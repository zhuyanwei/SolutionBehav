#include "opencv2/opencv.hpp"
#include <iostream>
#include <cstdio>

#include "ViBe_BGS.h"
#include "DiffTwo.h"

int playFile();

int main()
{
	//playFile();

	DiffTwo d1;
	d1.funMain("MySrc/lobby.avi");

	//ViBe_BGS v1;
	//v1.script("MySrc/lobby.avi");
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
#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

class LKDemo
{
public:
	LKDemo();
	~LKDemo();
	int funMain(string fileName);

private:
	void help();
	Point2f point;

	bool addRemovePt = false;
	void onMouse(int event, int x, int y, int /*flags*/, void* /*param*/);


};


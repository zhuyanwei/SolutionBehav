//star5
//para:
//c++-type;morphologyKernel;medianBlur;counter;rect;draw;
#pragma once
#include "../../GlobalSetting.h"
#include <opencv2\opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

class DiffTwo
{
public:
	DiffTwo();
	~DiffTwo();
	int funMain(string fileName);
private:
	const unsigned char FORE_GROUD = 255;
	int thresh = 10;
};


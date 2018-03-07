//hog
//http://blog.csdn.net/dcrmg/article/details/53047009
#pragma once
#include <Windows.h>
#include <opencv2\opencv.hpp>
#include <iostream>
#include "../../GlobalSetting.h"
using namespace std;
using namespace cv;

class Hog1
{
public:
	Hog1();
	~Hog1();
	int funMain(string fileName);
};


//star3
//Dilates;
//a more simple 3frame diff ,including img process
//http://blog.csdn.net/dcrmg/article/details/52234929
#pragma once
#include "../../GlobalSetting.h"
#include <opencv2\opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

class DiffThree2
{
public:
	DiffThree2();
	~DiffThree2();
	int script(string fileName);
};


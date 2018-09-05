//single gussin 
//https://blog.csdn.net/haoxiaodao/article/details/39254605
#pragma once
#include <iostream>
#include "../../GlobalSetting.h"
#include "opencv2/opencv.hpp"
using namespace cv;
using namespace std;

class DanGao1
{
public:
	DanGao1();
	~DanGao1();
	int funMain(string fileName);
};


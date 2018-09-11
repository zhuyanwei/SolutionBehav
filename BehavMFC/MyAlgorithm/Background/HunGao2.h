//https://blog.csdn.net/swj110119/article/details/51840430
#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
#include "../../GlobalSetting.h"
using namespace std;
using namespace cv;

class HunGao2
{
public:
	HunGao2();
	~HunGao2();
	int funMain(string fileName);
private:
	int COMPONET = 5;   //混合高斯模型个数
	double ALPHA = 0.03;  //学习率
	int SD_INIT = 6;    //方差初始值
	double THRESHOLD = 0.25;   //前景所占比例
	double D = 2.5;
};


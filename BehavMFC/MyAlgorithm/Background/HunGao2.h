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
	int COMPONET = 5;   //��ϸ�˹ģ�͸���
	double ALPHA = 0.03;  //ѧϰ��
	int SD_INIT = 6;    //�����ʼֵ
	double THRESHOLD = 0.25;   //ǰ����ռ����
	double D = 2.5;
};


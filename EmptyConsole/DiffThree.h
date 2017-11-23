#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

#define threshold_diff1 10 //设置简单帧差法阈值
#define threshold_diff2 10 //设置简单帧差法阈值

class DiffThree
{
public:
	DiffThree();
	~DiffThree();
	int script(string fileName);
private:

};


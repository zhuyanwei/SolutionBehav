#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
#include "../../GlobalSetting.h"
using namespace std;
using namespace cv;

class Edge
{
public:
	Edge();
	~Edge();
	int funMain(string ch, Mat min, Mat mout);
private:
	string choose;
};


//a more simple 2frame diff 
//http://blog.csdn.net/dcrmg/article/details/52234929
#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

class DiffTwo2
{
public:
	DiffTwo2();
	~DiffTwo2();
	int script(string fileName);
};


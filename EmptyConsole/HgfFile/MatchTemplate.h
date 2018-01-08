//template match
//http://hgfai.lofter.com/post/33b0fd_1225974
#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

class MatchTemplate
{
public:
	MatchTemplate();
	~MatchTemplate();
	int funMain();
private:
	Rect box;
	bool drawing_box = false;
	bool gotBB = false;
};


//useless;
//c-type;
//another background average ,similar to $BackAver,add function multi camera
//http://www.cnblogs.com/tornadomeet/archive/2012/04/08/2438158.html
#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

//IMPORTANT DEFINES:
#define NUM_CAMERAS   1              //This function can handle an array of cameras
#define HIGH_SCALE_NUM 7.0            //How many average differences from average image on the high side == background
#define LOW_SCALE_NUM 6.0        //How many average differences from average image on the low side == background
#define cvCvtPixToPlane			//cvSplit,且cvSplit是将一个多通道矩阵转换为几个单通道矩阵

class BackAver2
{
public:
	BackAver2();
	~BackAver2();
	int script();
private:
	void AllocateImages(IplImage *I);
	void DeallocateImages();
	void accumulateBackground(IplImage *I, int number = 0);
	void scaleHigh(float scale = HIGH_SCALE_NUM, int num = 0);
	void scaleLow(float scale = LOW_SCALE_NUM, int num = 0);
	void createModelsfromStats();
	void backgroundDiff(IplImage *I, IplImage *Imask, int num = 0);
};

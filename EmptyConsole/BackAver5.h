// background average ,using cvOr
#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

class BackAver5
{
public:
	BackAver5();
	~BackAver5();
	int funMain(string fileName);
private:
	//为不同的临时指针图像和统计属性创建指针
	//Float, 3-channel images
	IplImage* IavgF, *IdiffF, *IprevF, *IhiF, *IlowF;
	IplImage* Iscratch, *Iscratch2;
	//Float 1-channel images
	IplImage* Igray1, *Igray2, *Igray3;
	IplImage* Ilow1, *Ilow2, *Ilow3;
	IplImage* Ihi1, *Ihi2, *Ihi3;
	//Byte, 1-channel image
	IplImage* Imask;
	IplImage* Imaskt;
	//Counts number of images learned for averaging later.
	float Icount;

	//functions
	void AllocateImages(IplImage* I);
	void accumulateBackground(IplImage *I);
	void setHighThreshold(float scale);
	void setLowThreshold(float scale);
	void createModelsfromStats();
	void backgroundDiff(IplImage* I);
	void DeallocateImage();

};


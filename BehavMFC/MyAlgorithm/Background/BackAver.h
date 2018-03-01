//para:filename;number of sum;low thread;high thread
//c-type;camera;low high thread;
#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
#include "../../GlobalSetting.h"
using namespace std;
using namespace cv;

class BackAver
{
public:
	BackAver();
	~BackAver();
	int funMain(string fileName);
private:
	//全局变量：我们为需要的不同临时图像和统计属性的图像创建指针
	/*Float 3-channel images*/
	IplImage* IavgF, *IdiffF, *IprevF, *IhiF, *IlowF;
	IplImage* Iscratch, *Iscratch2;

	/*Float 1-channel images*/
	IplImage* Igray1, *Igray2, *Igray3;
	IplImage* Ilow1, *Ilow2, *Ilow3;
	IplImage* Ihi1, *Ihi2, *Ihi3;

	/*Byte 1-channel image*/
	IplImage* Imaskt;
	IplImage* Imask;
	IplImage* IProc;
	Mat mmask;

	float Icount;

	//functions
	void AllocateImages(IplImage* I);
	void accumulateBackground(IplImage* I);
	void setHighThreshold(float scale);
	void setLowThreshold(float scale);
	void createModelsfromStats();
	void backgroundDiff(IplImage* I);
	void DeallocateImages();
	int process();
};


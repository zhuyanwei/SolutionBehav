//star5
//flow lk pyramid,somehow complex
//http://blog.csdn.net/u014568921/article/details/46638557
#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
#include <Windows.h>
using namespace std;
using namespace cv;

class FlowLKP
{
	struct DBPoint
	{
		double x;
		double y;
		DBPoint(const double X, const double Y) :x(X), y(Y){
		}
		DBPoint(){}
	};
	int*height;
	int*width;

public:
	FlowLKP();
	~FlowLKP();

	//LucasKanadeTracker(const int windowRadius, bool usePyr);
	//~LucasKanadeTracker();
	void get_pre_frame(BYTE*&gray);//use only at the beginning
	void discard_pre_frame();
	//set the next frame as pre_frame,must dicard pre_pyr in advance
	void get_pre_frame();
	//use every time,must after using get_pre_frame(BYTE**pyr)
	void get_next_frame(BYTE*&gray);
	void get_info(const int nh, const int nw);
	void get_target(POINT*target, int n);
	void run_single_frame();
	POINT get_result();
	BYTE*&get_pyramid(int th);
	int get_pyrH(int th){ return height[th]; }
	int get_pyrW(int th){ return width[th]; }

private:
	unsigned int max_pyramid_layer;
	unsigned int original_imgH;
	unsigned int original_imgW;
	unsigned int window_radius;
	BYTE**pre_pyr;//the pyramid of previous frame image,img1_pyr[0] is of max size
	BYTE**next_pyr;//the frame after img1_pyr
	bool isusepyramid;
	DBPoint*target, *endin;
	int numofpoint;

	void build_pyramid(BYTE**&original_gray);
	void lucaskanade(BYTE**&frame_pre, BYTE**&frame_cur, DBPoint*& start,
		DBPoint*& finish, unsigned int point_nums, char*state);
	void get_max_pyramid_layer();
	void pyramid_down(BYTE*&src_gray_data, const int src_h,
		const int src_w, BYTE*& dst, int&dst_h, int&dst_w);
	void lowpass_filter(BYTE*&src, const int H, const int W, BYTE*&smoothed);
	double get_subpixel(BYTE*&src, int h, int w, const DBPoint& point);
	// caculate the inverse mareix of pMatrix£¬the result is put into _pMatrix   
	void ContraryMatrix(double *pMatrix, double * _pMatrix, int dim);
	bool matrixMul(double *src1, int height1, int width1, double *src2, int height2, int width2, double *dst);
};

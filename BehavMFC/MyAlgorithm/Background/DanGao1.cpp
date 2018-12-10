#include "DanGao1.h"

DanGao1::DanGao1()
{
}

DanGao1::~DanGao1()
{
}

int DanGao1::funMain(string fileName)
{
	//新建窗口
	cvNamedWindow("origin", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("background", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("foreground", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("origin", 1100, 400);
	cvMoveWindow("background", 600, 400);
	cvMoveWindow("foreground", 100, 400);

	double alpha = 0.5;//背景建模alpha值
	double std_int = 20;//初始化标准差
	double var_int = std_int*std_int;//初始化方差
	double lamda = 2.5*1.2;//背景更新参数

	//open camera
	CvCapture* capture;
	if (fileName == "00")
	{
		capture = cvCreateCameraCapture(0);
	}
	else
	{
		char* myChar = const_cast<char*>(fileName.c_str());
		capture = cvCreateFileCapture(myChar);
	}
	if (!capture)
	{
		printf("Couldn't Open the file or camera");
		return -1;
	}

	IplImage *frame = NULL;//原始图像
	IplImage *frame_u = NULL;//期望图像（背景）
	IplImage *frame_d = NULL;//前景图像
	IplImage *frame_var = NULL;//方差图像
	IplImage *frame_std = NULL;//标准差

	CvScalar pixel = { 0 };//像素原始值
	CvScalar pixel_u = { 0 };//像素期望值
	CvScalar pixel_d = { 0 };//像素前景
	CvScalar pixel_var = { 0 };//像素方差
	CvScalar pixel_std = { 0 };//像素标准差

	//初始化frame_u,frame_d,frame_var,frame_std
	frame = cvQueryFrame(capture);
	frame_u = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 3);
	frame_d = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 3);
	frame_var = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 3);
	frame_std = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 3);

	for (int y = 0; y<frame->height; ++y)
	{
		for (int x = 0; x<frame->width; ++x)
		{
			pixel = cvGet2D(frame, y, x);

			pixel_u.val[0] = pixel.val[0];
			pixel_u.val[1] = pixel.val[1];
			pixel_u.val[2] = pixel.val[2];

			pixel_d.val[0] = 0;
			pixel_d.val[1] = 0;
			pixel_d.val[2] = 0;

			pixel_std.val[0] = std_int;
			pixel_std.val[1] = std_int;
			pixel_std.val[2] = std_int;

			pixel_var.val[0] = var_int;
			pixel_var.val[1] = var_int;
			pixel_var.val[2] = var_int;

			cvSet2D(frame_u, y, x, pixel_u);
			cvSet2D(frame_d, y, x, pixel_d);
			cvSet2D(frame_var, y, x, pixel_var);
			cvSet2D(frame_std, y, x, pixel_std);
		}
	}
	int finalCount = 0;
	while (cvWaitKey(CVWAIT) != 'q')//按ESC键退出，帧率33ms
	{
		cout << finalCount << '\n';
		finalCount++;
		double TRoberts;
		TRoberts = static_cast<double>(getTickCount());

		frame = cvQueryFrame(capture);
		//视频结束退出
		if (!frame)
		{
			break;
		}
		//单高斯背景更新
		for (int y = 0; y<frame->height; ++y)
		{
			for (int x = 0; x<frame->width; ++x)
			{
				pixel = cvGet2D(frame, y, x);
				pixel_u = cvGet2D(frame_u, y, x);
				pixel_d = cvGet2D(frame_d, y, x);
				pixel_std = cvGet2D(frame_std, y, x);
				pixel_var = cvGet2D(frame_var, y, x);

				//|I-u|<lamda*std时认为是背景，进行更新
				if (fabs(pixel.val[0] - pixel_u.val[0])<lamda*pixel_std.val[0] &&
					fabs(pixel.val[1] - pixel_u.val[1])<lamda*pixel_std.val[1] &&
					fabs(pixel.val[2] - pixel_u.val[2])<lamda*pixel_std.val[2])
				{
					//更新期望u=(1-alpha)*u+alpha*I
					pixel_u.val[0] = (1 - alpha)*pixel_u.val[0] + alpha*pixel.val[0];
					pixel_u.val[1] = (1 - alpha)*pixel_u.val[1] + alpha*pixel.val[1];
					pixel_u.val[2] = (1 - alpha)*pixel_u.val[2] + alpha*pixel.val[2];

					//更新方差var=(1-alpha)*var+alpha*(I-u)^2
					pixel_var.val[0] = (1 - alpha)*pixel_var.val[0] +
						alpha*(pixel.val[0] - pixel_u.val[0])*(pixel.val[0] - pixel_u.val[0]);
					pixel_var.val[1] = (1 - alpha)*pixel_var.val[1] +
						alpha*(pixel.val[1] - pixel_u.val[1])*(pixel.val[1] - pixel_u.val[1]);
					pixel_var.val[2] = (1 - alpha)*pixel_var.val[2] +
						alpha*(pixel.val[2] - pixel_u.val[2])*(pixel.val[2] - pixel_u.val[2]);
					//更新标准差
					pixel_std.val[0] = sqrt(pixel_var.val[0]);
					pixel_std.val[1] = sqrt(pixel_var.val[1]);
					pixel_std.val[2] = sqrt(pixel_var.val[2]);

					//写入矩阵
					cvSet2D(frame_u, y, x, pixel_u);
					cvSet2D(frame_var, y, x, pixel_var);
					cvSet2D(frame_std, y, x, pixel_std);
				}
				else
				{
					pixel_d.val[0] = pixel.val[0] - pixel_u.val[0];
					pixel_d.val[1] = pixel.val[1] - pixel_u.val[1];
					pixel_d.val[2] = pixel.val[2] - pixel_u.val[2];
					cvSet2D(frame_d, y, x, pixel_d);
				}
			}
		}
		TRoberts = static_cast<double>(getTickCount()) - TRoberts;
		TRoberts /= getTickFrequency();
		cout << TRoberts << '\n';

		//显示结果
		frame_u->origin = 1;
		frame_u->origin = 1;
		cvShowImage("origin", frame);
		cvShowImage("background", frame_u);
		cvShowImage("foreground", frame_d);
	}
	//释放内存
	cvReleaseCapture(&capture);
	cvReleaseImage(&frame);
	cvReleaseImage(&frame_u);
	cvReleaseImage(&frame_var);
	cvReleaseImage(&frame_std);
	cvDestroyWindow("origin");
	cvDestroyWindow("background");
	cvDestroyWindow("foreground");

	return 1;
}

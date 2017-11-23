#include "DiffThree.h"

DiffThree::DiffThree()
{
}

DiffThree::~DiffThree()
{
}

int DiffThree::script(string fileName)
{
	Mat img_src1, img_src2, img_src3;//3帧法需要3帧图片
	Mat img_dst, gray1, gray2, gray3;
	Mat gray_diff1, gray_diff2;//存储2次相减的图片
	Mat gray;//用来显示前景的
	bool pause = false;
	VideoCapture vido_file(fileName);//在这里改相应的文件名
	namedWindow("foreground", 0);
	for (;;)
	{
		if (!false)
		{
			vido_file >> img_src1;
			if (img_src1.empty())
				break;
			if (img_src1.depth() != 1)
				cvtColor(img_src1, gray1, CV_BGR2GRAY);
			waitKey(5);

			vido_file >> img_src2;
			if (img_src2.empty())
				break;
			if (img_src2.depth() != 1)
				cvtColor(img_src2, gray2, CV_BGR2GRAY);
			imshow("video_src", img_src2);//
			waitKey(5);

			vido_file >> img_src3;
			if (img_src3.empty())
				break;
			if (img_src3.depth() != 1)
				cvtColor(img_src3, gray3, CV_BGR2GRAY);

			subtract(gray2, gray1, gray_diff1);//第二帧减第一帧
			subtract(gray3, gray2, gray_diff2);//第三帧减第二帧

			for (int i = 0; i<gray_diff1.rows; i++)
				for (int j = 0; j<gray_diff1.cols; j++)
				{
					if (abs(gray_diff1.at<unsigned char>(i, j)) >= threshold_diff1)//这里模板参数一定要用unsigned char，否则就一直报错
						gray_diff1.at<unsigned char>(i, j) = 255;            //第一次相减阈值处理
					else gray_diff1.at<unsigned char>(i, j) = 0;

					if (abs(gray_diff2.at<unsigned char>(i, j)) >= threshold_diff2)//第二次相减阈值处理
						gray_diff2.at<unsigned char>(i, j) = 255;
					else gray_diff2.at<unsigned char>(i, j) = 0;
				}
			bitwise_and(gray_diff1, gray_diff2, gray);
			imshow("foreground", gray);
		}
		char c = (char)waitKey(10);
		if (c == 27)
		{
			break;
		}
		if (c == ' ')
			pause = !pause;//为什么暂停不了？？
	}
	return 1;
}


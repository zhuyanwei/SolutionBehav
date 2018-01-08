#include "BackAuto.h"

BackAuto::BackAuto()
{
}

BackAuto::~BackAuto()
{
}

int BackAuto::funMain()
{
	std::string videoFileName = "MySrc/sample.avi";

	int threshold = 25;    // 二值化阈值
	float alpha = 0.01;    // 更新速度 [0, 1]

	cv::VideoCapture capture;
	capture.open(videoFileName);
	if (!capture.isOpened())
	{
		std::cout << "cannot open video" << std::endl;
		return -1;
	}

	cv::Mat foregroundImg;
	cv::Mat foregroundMat;

	cv::Mat backgroundImg;
	cv::Mat backgroundMat;

	cv::Mat frame;
	cv::Mat grayImg;
	cv::Mat grayMat;

	while (capture.read(frame))
	{
		cv::cvtColor(frame, grayImg, CV_BGR2GRAY);
		grayImg.convertTo(grayMat, CV_32FC1);

		if (backgroundMat.empty())
		{
			grayImg.copyTo(backgroundImg);
			grayImg.convertTo(backgroundMat, CV_32FC1);
		}

		// 背景减除
		cv::absdiff(grayMat, backgroundMat, foregroundMat);

		// 自适应背景更新
		cv::addWeighted(backgroundMat, alpha, foregroundMat, 1 - alpha, 0, backgroundMat);

		// 二值化，获取前景像素点
		cv::threshold(foregroundMat, foregroundMat, threshold, 255, CV_THRESH_BINARY);

		// 为了显示用，将CV_32FC1转换为CV_8U
		cv::convertScaleAbs(foregroundMat, foregroundImg);
		cv::convertScaleAbs(backgroundMat, backgroundImg);

		cv::imshow("frame", frame);
		cv::imshow("foreground", foregroundImg);
		cv::imshow("background", backgroundImg);

		if (cv::waitKey(30) == 'q')
		{
			break;
		}
	}

	return 0;
}
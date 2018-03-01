#include "DiffTwo.h"

DiffTwo::DiffTwo()
{
}

DiffTwo::~DiffTwo()
{
}

int DiffTwo::funMain(string fileName)
{
	VideoCapture video;
	if (fileName == "00")
	{
		video.open(CAMERA_ID);
		Mat tmp00;
		video >> tmp00;
		while (!tmp00.data)
		{
			waitKey(1000);
			video >> tmp00;
		}
	}
	else
	{
		video.open(fileName);
		if (!video.isOpened())
		{
			cout << "no file" << '\n';
			return -1;
		}
	}
	//用于保存当前帧的图片  
	Mat currentBGRFrame;
	//用来保存上一帧和当前帧的灰度图片  
	Mat previousGrayFrame;
	Mat currentGaryFrame;
	//用来保存帧差  
	Mat frameDifference;//CV_16SC1  
	//用来保存帧差的绝对值  
	Mat absFrameDifferece;
	//用来显示前景  
	Mat segmentation;
	//显示原视频  
	namedWindow("Camera", 1);
	//显示前景  
	namedWindow("Forground", 1);
	//createTrackbar("阈值:", "segmentation", &thresh, FORE_GROUD, NULL);
	//帧数  
	int numberFrame = 0;
	//形态学处理用到的算子  
	Mat morphologyKernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	for (;;)
	{
		//读取当前帧  
		video >> currentBGRFrame;
		//判断当前帧是否存在  
		if (!currentBGRFrame.data)
			break;
		numberFrame++;
		if (numberFrame == 1)
		{
			//颜色空间的转换 
			if (currentBGRFrame.depth() != 1)
				cvtColor(currentBGRFrame, currentGaryFrame, COLOR_BGR2GRAY);
			//保存当前帧的灰度图  
			previousGrayFrame = currentGaryFrame.clone();
			imshow("Camera", currentBGRFrame);
			continue;
		}
		else
		{
			//颜色空间的转换  
			if (currentBGRFrame.depth() != 1)
				cvtColor(currentBGRFrame, currentGaryFrame, COLOR_BGR2GRAY);
			//src1-src2  
			subtract(currentGaryFrame, previousGrayFrame, frameDifference, Mat(), CV_16SC1);
			//取绝对值  
			absFrameDifferece = abs(frameDifference);
			//位深的改变  
			absFrameDifferece.convertTo(absFrameDifferece, CV_8UC1, 1, 0);
			//阈值处理  
			threshold(absFrameDifferece, segmentation, double(thresh), double(FORE_GROUD), THRESH_BINARY);
			//中值滤波  
			medianBlur(segmentation, segmentation, 3);
			//形态学处理(开闭运算)  
			//morphologyEx(segmentation,segmentation,MORPH_OPEN,morphologyKernel,Point(-1,-1),1,BORDER_REPLICATE);  
			morphologyEx(segmentation, segmentation, MORPH_CLOSE, morphologyKernel, Point(-1, -1), 2, BORDER_REPLICATE);
			//显示二值化图片  
			imshow("Forground", segmentation);
			////@J,sobel
			//Mat sobel1;
			//Sobel(segmentation, sobel1, CV_8U, 1, 0);
			//imshow("Sobel", sobel1);
			//找边界  
			vector< vector<Point> > contours;
			vector<Vec4i> hierarchy;
			findContours(segmentation, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));//CV_RETR_TREE  
			vector< vector<Point> > contours_poly(contours.size());
			/*存储运动物体*/
			vector<Rect> boundRect;
			boundRect.clear();
			//画出运动物体  
			//对视频中出现的运动物体，进行初次的筛选  
			for (int index = 0; index < contours.size(); index++)
			{
				approxPolyDP(Mat(contours[index]), contours_poly[index], 3, true);
				Rect rect = boundingRect(Mat(contours_poly[index]));
				rectangle(currentBGRFrame, rect, Scalar(0, 255, 255), 2);
			}
			//显示原视频  
			imshow("Camera", currentBGRFrame);
			//保存当前帧的灰度图  
			previousGrayFrame = currentGaryFrame.clone();
			//cout << "test--" << thresh << '\n';
		}
		if (waitKey(CVWAIT) == 'q')
			break;
	}
	video.release();
	return 1;
}
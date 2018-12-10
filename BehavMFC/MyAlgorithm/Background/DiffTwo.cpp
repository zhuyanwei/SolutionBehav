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
	//���ڱ��浱ǰ֡��ͼƬ  
	Mat currentBGRFrame;
	//����������һ֡�͵�ǰ֡�ĻҶ�ͼƬ  
	Mat previousGrayFrame;
	Mat currentGaryFrame;
	//��������֡��  
	Mat frameDifference;//CV_16SC1  
	//��������֡��ľ���ֵ  
	Mat absFrameDifferece;
	//������ʾǰ��  
	Mat segmentation;
	//��ʾԭ��Ƶ  
	namedWindow("Camera", 1);
	//��ʾǰ��  
	namedWindow("Forground", 1);
	//createTrackbar("��ֵ:", "segmentation", &thresh, FORE_GROUD, NULL);
	//֡��  
	int numberFrame = 0;
	//��̬ѧ�����õ�������  
	Mat morphologyKernel = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
	for (;;)
	{
		double TRoberts;
		TRoberts = static_cast<double>(getTickCount());

		//��ȡ��ǰ֡  
		video >> currentBGRFrame;
		//�жϵ�ǰ֡�Ƿ����  
		if (!currentBGRFrame.data)
			break;
		numberFrame++;
		if (numberFrame == 1)
		{
			//��ɫ�ռ��ת�� 
			if (currentBGRFrame.depth() != 1)
				cvtColor(currentBGRFrame, currentGaryFrame, COLOR_BGR2GRAY);
			//���浱ǰ֡�ĻҶ�ͼ  
			previousGrayFrame = currentGaryFrame.clone();
			imshow("Camera", currentBGRFrame);
			continue;
		}
		else
		{
			//��ɫ�ռ��ת��  
			if (currentBGRFrame.depth() != 1)
				cvtColor(currentBGRFrame, currentGaryFrame, COLOR_BGR2GRAY);
			//src1-src2  
			subtract(currentGaryFrame, previousGrayFrame, frameDifference, Mat(), CV_16SC1);
			//ȡ����ֵ  
			absFrameDifferece = abs(frameDifference);
			//λ��ĸı�  
			absFrameDifferece.convertTo(absFrameDifferece, CV_8UC1, 1, 0);
			//��ֵ����  
			threshold(absFrameDifferece, segmentation, double(thresh), double(FORE_GROUD), THRESH_BINARY);
			//��ֵ�˲�  
			medianBlur(segmentation, segmentation, 3);
			//��̬ѧ����(��������)  
			//morphologyEx(segmentation,segmentation,MORPH_OPEN,morphologyKernel,Point(-1,-1),1,BORDER_REPLICATE);  
			morphologyEx(segmentation, segmentation, MORPH_CLOSE, morphologyKernel, Point(-1, -1), 2, BORDER_REPLICATE);
			//time
			TRoberts = static_cast<double>(getTickCount()) - TRoberts;
			TRoberts /= getTickFrequency();
			cout << TRoberts << '\n';

			//��ʾ��ֵ��ͼƬ  
			imshow("Forground", segmentation);
			////@J,sobel
			//Mat sobel1;
			//Sobel(segmentation, sobel1, CV_8U, 1, 0);
			//imshow("Sobel", sobel1);
			//�ұ߽�  
			vector< vector<Point> > contours;
			vector<Vec4i> hierarchy;
			findContours(segmentation, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));//CV_RETR_TREE  
			vector< vector<Point> > contours_poly(contours.size());
			/*�洢�˶�����*/
			vector<Rect> boundRect;
			boundRect.clear();
			//�����˶�����  
			//����Ƶ�г��ֵ��˶����壬���г��ε�ɸѡ  
			for (int index = 0; index < contours.size(); index++)
			{
				approxPolyDP(Mat(contours[index]), contours_poly[index], 3, true);
				Rect rect = boundingRect(Mat(contours_poly[index]));
				rectangle(currentBGRFrame, rect, Scalar(0, 255, 255), 2);
			}
			//��ʾԭ��Ƶ  
			imshow("Camera", currentBGRFrame);
			//���浱ǰ֡�ĻҶ�ͼ  
			previousGrayFrame = currentGaryFrame.clone();
			//cout << "test--" << thresh << '\n';
		}
		if (waitKey(CVWAIT) == 'q')
			break;
	}
	video.release();
	return 1;
}
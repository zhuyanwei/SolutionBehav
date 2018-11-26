//https://blog.csdn.net/qq_37753409/article/details/79047063
#include "opencv2/core/core.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/core/cuda.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/ml.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <math.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
using namespace std;
using namespace cv;
using namespace cv::ml;

class Tools
{
public:
	Tools();
	int CropImageCount = 0; //�ü������ĸ�����ͼƬ����
	void ImgCut()
	{
		Mat src;
		string ImgName;

		char saveName[256];//�ü������ĸ�����ͼƬ�ļ���
		ifstream fin("E:\\Program\\Vstudio\\OpenCv\\picture\\INRIAPerson\\Train\\neg.lst");//��ԭʼ������ͼƬ�ļ��б�

		//һ��һ�ж�ȡ�ļ��б�
		while (getline(fin, ImgName))
		{
			cout << "����" << ImgName << endl;
			ImgName = "E:\\Program\\Vstudio\\OpenCv\\picture\\INRIAPerson\\" + ImgName;

			src = imread(ImgName, 1);//��ȡͼƬ
			//cout<<"��"<<src.cols<<"���ߣ�"<<src.rows<<endl;
			//ͼƬ��СӦ���������ٰ���һ��64*128�Ĵ���
			if (src.cols >= 64 && src.rows >= 128)
			{
				srand(time(NULL));//�������������  time(NULL)��ʾ��ǰϵͳʱ��

				//��ÿ��ͼƬ���������10��64*128��С�Ĳ������˵ĸ�����
				for (int i = 0; i<10; i++)
				{
					int x = (rand() % (src.cols - 64)); //���Ͻ�x����
					int y = (rand() % (src.rows - 128)); //���Ͻ�y����
					//cout<<x<<","<<y<<endl;
					Mat imgROI = src(Rect(x, y, 64, 128));
					sprintf(saveName, "E:\\Program\\Vstudio\\OpenCv\\picture\\INRIAPerson\\negphoto\\noperson%06d.jpg", ++CropImageCount);//���ɲü����ĸ�����ͼƬ���ļ���
					imwrite(saveName, imgROI);//�����ļ�
				}
			}
		}
	}
	~Tools();
};

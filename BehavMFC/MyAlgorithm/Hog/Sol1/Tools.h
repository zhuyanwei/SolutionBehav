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
	int CropImageCount = 0; //裁剪出来的负样本图片个数
	void ImgCut()
	{
		Mat src;
		string ImgName;

		char saveName[256];//裁剪出来的负样本图片文件名
		ifstream fin("E:\\Program\\Vstudio\\OpenCv\\picture\\INRIAPerson\\Train\\neg.lst");//打开原始负样本图片文件列表

		//一行一行读取文件列表
		while (getline(fin, ImgName))
		{
			cout << "处理：" << ImgName << endl;
			ImgName = "E:\\Program\\Vstudio\\OpenCv\\picture\\INRIAPerson\\" + ImgName;

			src = imread(ImgName, 1);//读取图片
			//cout<<"宽："<<src.cols<<"，高："<<src.rows<<endl;
			//图片大小应该能能至少包含一个64*128的窗口
			if (src.cols >= 64 && src.rows >= 128)
			{
				srand(time(NULL));//设置随机数种子  time(NULL)表示当前系统时间

				//从每张图片中随机采样10个64*128大小的不包含人的负样本
				for (int i = 0; i<10; i++)
				{
					int x = (rand() % (src.cols - 64)); //左上角x坐标
					int y = (rand() % (src.rows - 128)); //左上角y坐标
					//cout<<x<<","<<y<<endl;
					Mat imgROI = src(Rect(x, y, 64, 128));
					sprintf(saveName, "E:\\Program\\Vstudio\\OpenCv\\picture\\INRIAPerson\\negphoto\\noperson%06d.jpg", ++CropImageCount);//生成裁剪出的负样本图片的文件名
					imwrite(saveName, imgROI);//保存文件
				}
			}
		}
	}
	~Tools();
};

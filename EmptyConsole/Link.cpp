#include "Link.h"

Link::Link()
{
}

Link::~Link()
{
}

int Link::funMain()
{
	IplImage *src = cvLoadImage("MySrc/lena.jpg", CV_LOAD_IMAGE_COLOR);
	cvNamedWindow("原始图像");
	cvShowImage("原始图像", src);

	IplImage* dst = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, 1);
	cvCvtColor(src, dst, CV_BGR2GRAY);
	cvNamedWindow("灰度图像");
	cvShowImage("灰度图像", src);

	cvThreshold(dst, dst, 0.0, 255.0, CV_THRESH_BINARY | CV_THRESH_OTSU);//OTSU二值化   
	IplConvKernel *element = cvCreateStructuringElementEx(5, 5, 0, 0, CV_SHAPE_ELLIPSE);
	cvMorphologyEx(dst, dst, NULL, element, CV_MOP_OPEN);//开运算，去除比结构元素小的点   

	cvReleaseStructuringElement(&element);
	cvNamedWindow("二值图像");
	cvShowImage("二值图像", dst);

	int w, h;
	CvSize sz = cvGetSize(dst);
	int color = 254;//不对0计数，不可能为255，所以254   
	for(w = 0; w<sz.width; w++)
	{
		for (h = 0; h<sz.height; h++)
		{
			if (color > 0)
			{
				if (CV_IMAGE_ELEM(dst, unsigned char, h, w) == 255)
				{
					//把连通域标记上颜色   
					cvFloodFill(dst, cvPoint(w, h), CV_RGB(color, color, color));
					color--;
				}
			}
		}
	}
	cvNamedWindow("标记颜色后的图像");
	cvShowImage("标记颜色后的图像", dst);

	int colorsum[255] = { 0 };
	for (w = 0; w<sz.width; w++)
	{
		for (h = 0; h<sz.height; h++)
		{
			if (CV_IMAGE_ELEM(dst, unsigned char, h, w) > 0)
			{
				colorsum[CV_IMAGE_ELEM(dst, unsigned char, h, w)]++;//统计每种颜色的数量   
			}
		}
	}

	vector<int> v1(colorsum, colorsum + 255);//用数组初始化vector   
	//求出最多数量的染色，注意max_element的使用方法   
	int maxcolorsum = max_element(v1.begin(), v1.end()) - v1.begin();
	printf("%d\n", maxcolorsum);
	for (w = 0; w<sz.width; w++)
	{
		for (h = 0; h<sz.height; h++)
		{
			if (CV_IMAGE_ELEM(dst, unsigned char, h, w) == maxcolorsum)
			{
				CV_IMAGE_ELEM(dst, unsigned char, h, w) = 255;
			}
			else
			{
				CV_IMAGE_ELEM(dst, unsigned char, h, w) = 0;
			}
		}
	}
	cvNamedWindow("最大连通域图");
	cvShowImage("最大连通域图", dst);
	cvWaitKey(0);
	cvDestroyAllWindows();
	cvReleaseImage(&src);
	cvReleaseImage(&dst);
	return 0;
}
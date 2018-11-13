#include "Edge.h"

Edge::Edge()
{
}

Edge::~Edge()
{
}

Mat Edge::funMain(string ch, Mat min, Mat mout)
{
	Mat dstImage = min.clone();
	if (ch == "no")
	{
		return dstImage;
	}
	else if (ch == "rob")
	{

	}
	else if (ch == "pre")
	{

	}
	else if (ch == "sob")
	{
		//-->sobel
		Sobel(min, dstImage, -1, 1, 1);
	}
	else if (ch == "lap")
	{
		//-->Laplacian
		Laplacian(min, dstImage, -1);
	}
	else if (ch == "can")
	{
		//gray
		if (min.channels() != 1)
			cvtColor(min, min, CV_BGR2GRAY);
		////blur
		blur(min, min, Size(4, 4));
		//canny
		Canny(min, dstImage, 100, 250);
	}
	else
	{
		//return 0;
	}
	return dstImage;
}

Mat Edge::myRoberts(Mat srcImage)
{
	if (srcImage.channels() != 1)
	{
		cvtColor(srcImage, srcImage, CV_BGR2GRAY);
	}
	cv::Mat dstImage = srcImage.clone();
	int nRows = dstImage.rows;
	int nCols = dstImage.cols;
	for (int i = 0; i < nRows - 1; i++)
	{
		for (int j = 0; j < nCols - 1; j++)
		{
			int t1 = (srcImage.at<uchar>(i, j) -
				srcImage.at<uchar>(i + 1, j + 1)) *
				(srcImage.at<uchar>(i, j) -
				srcImage.at<uchar>(i + 1, j + 1));
			int t2 = (srcImage.at<uchar>(i + 1, j) -
				srcImage.at<uchar>(i, j + 1)) *
				(srcImage.at<uchar>(i + 1, j) -
				srcImage.at<uchar>(i, j + 1));
			dstImage.at<uchar>(i, j) = (uchar)sqrt(t1 + t2);
		}
	}
	return dstImage;
}

Mat Edge::myPrewitt(Mat imageP)
{
	cvtColor(imageP, imageP, CV_BGR2GRAY);
	float prewittx[9] =
	{
		-1, 0, 1,
		-1, 0, 1,
		-1, 0, 1
	};
	float prewitty[9] =
	{
		1, 1, 1,
		0, 0, 0,
		-1, -1, -1
	};
	Mat px = Mat(3, 3, CV_32F, prewittx);
	//cout << px << endl;
	Mat py = Mat(3, 3, CV_32F, prewitty);
	//cout << py << endl;
	Mat dstx = Mat(imageP.size(), imageP.type(), imageP.channels());
	Mat dsty = Mat(imageP.size(), imageP.type(), imageP.channels());
	Mat dst = Mat(imageP.size(), imageP.type(), imageP.channels());
	filter2D(imageP, dstx, imageP.depth(), px);
	filter2D(imageP, dsty, imageP.depth(), py);
	float tempx, tempy, temp;
	for (int i = 0; i<imageP.rows; i++)
	{
		for (int j = 0; j<imageP.cols; j++)
		{
			tempx = dstx.at<uchar>(i, j);
			tempy = dsty.at<uchar>(i, j);
			temp = sqrt(tempx*tempx + tempy*tempy);
			dst.at<uchar>(i, j) = temp;
		}
	}
	return dst;
}

//kirsch算子滤波
int Edge::myKirsch(IplImage *src, IplImage *dst)
{
	dst = cvCloneImage(src);
	//cvConvert(src,srcMat); //将图像转化成矩阵处理
	int x, y;
	float a, b, c, d;
	float p1, p2, p3, p4, p5, p6, p7, p8, p9;
	uchar* ps = (uchar*)src->imageData; //ps为指向输入图片数据的指针
	uchar* pd = (uchar*)dst->imageData; //pd为指向输出图片数据的指针
	int w = dst->width;
	int h = dst->height;
	int step = dst->widthStep;

	for (x = 0; x < w - 2; x++)      //取以（x+1，y+1)为中心的9个邻域像素  1 4 7
	{                                                            // 2 5 8
		for (y = 0; y < h - 2; y++)                                     // 3 6 9
		{
			p1 = ps[y*step + x];
			p2 = ps[y*step + (x + 1)];
			p3 = ps[y*step + (x + 2)];
			p4 = ps[(y + 1)*step + x];
			p5 = ps[(y + 1)*step + (x + 1)];
			p6 = ps[(y + 1)*step + (x + 2)];
			p7 = ps[(y + 2)*step + x];
			p8 = ps[(y + 2)*step + (x + 1)];
			p9 = ps[(y + 2)*step + (x + 2)];//得到(i+1,j+1)周围九个点的灰度值

			a = fabs(float(-5 * p1 - 5 * p2 - 5 * p3 + 3 * p4 + 3 * p6 + 3 * p7 + 3 * p8 + 3 * p9));    //计算4个方向的梯度值
			b = fabs(float(3 * p1 - 5 * p2 - 5 * p3 + 3 * p4 - 5 * p6 + 3 * p7 + 3 * p8 + 3 * p9));
			c = fabs(float(3 * p1 + 3 * p2 - 5 * p3 + 3 * p4 - 5 * p6 + 3 * p7 + 3 * p8 - 5 * p9));
			d = fabs(float(3 * p1 + 3 * p2 + 3 * p3 + 3 * p4 - 5 * p6 + 3 * p7 - 5 * p8 - 5 * p9));
			a = max(a, b);                                         //取各个方向上的最大值作为边缘强度
			a = max(a, c);
			a = max(a, d);

			if(a>200)
				pd[(y+1)*step+(x+1)]=255;
			else 
				pd[(y+1)*step+(x+1)]=0;

			//pd[(y + 1)*step + (x + 1)] = a;
		}
	}

	//double min_val = 0, max_val = 0;//取图并显示像中的最大最小像素值
	//cvMinMaxLoc(dst, &min_val, &max_val);
	////printf("max_val = %f\nmin_val = %f\n", max_val, min_val);

	//cvNormalize(dst, dst, 0, 255, CV_MINMAX); //归一化处理
	cvSaveImage("MyOutput/ExperimentEdge/aftKirsch.jpg", dst);//把图像存入文件
	cvShowImage("aftKirsch", dst);
	return 1;
}
//kirsch算子滤波
void Edge::myKirsch2(const IplImage* src, CvMat* dst)
{
	if (src->nChannels != 1 && dst->type != 1)
	{
		cvError(CV_BadNumChannels, "kirsch", "The channels of image must be 1", __FILE__, __LINE__);
		return;
	}
	if (src->width != dst->width || src->height != dst->height)
	{
		cvError(CV_StsUnmatchedSizes, "kirsch", "The sizes of input/output must be same", __FILE__, __LINE__);
		return;
	}
	cout << "b1";
	cvShowImage("begin", src);
	//保存当前像素的八个模板滤波的结果
	vector<double > r(8);
	vector<double > p(8);
	CvMat* temp = cvCreateMat(src->height + 2, src->width + 2, CV_8UC1);
	CvMat* src_copy = cvCreateMat(src->height, src->width, CV_8UC1);
	cvConvertScale(src, src_copy);
	cvCopyMakeBorder(src_copy, temp, cvPoint(1, 1), IPL_BORDER_REPLICATE);
	Mat bbb = Mat(temp, false);
	imshow("gg",bbb);

	for (int y = 1; y != temp->rows - 1; ++y)
	{
		for (int x = 1; x != temp->cols - 1; ++x)
		{
			//8个邻域像素值
			p[0] = CV_MAT_ELEM(*temp, double, y - 1, x - 1);
			p[1] = CV_MAT_ELEM(*temp, double, y - 1, x);
			p[2] = CV_MAT_ELEM(*temp, double, y - 1, x + 1);
			p[3] = CV_MAT_ELEM(*temp, double, y, x + 1);
			p[4] = CV_MAT_ELEM(*temp, double, y + 1, x + 1);
			p[5] = CV_MAT_ELEM(*temp, double, y + 1, x);
			p[6] = CV_MAT_ELEM(*temp, double, y + 1, x - 1);
			p[7] = CV_MAT_ELEM(*temp, double, y, x - 1);
			r[0] = 0.625*(p[0] + p[1] + p[2]) - 0.375*(p[3] + p[4] + p[5] + p[6] + p[7]);
			r[1] = r[0] + p[7] - p[2];
			r[2] = r[1] + p[6] - p[1];
			r[3] = r[2] + p[5] - p[0];
			r[4] = r[3] + p[4] - p[7];
			r[5] = r[4] + p[3] - p[6];
			r[6] = r[5] + p[2] - p[5];
			r[7] = r[6] + p[1] - p[4];
			//排序
			std::sort(r.begin(), r.end());
			//if (r[1] != 0)
			//{
			//             cout<<"r1 != 0"<<endl;
			//}
			*((double*)CV_MAT_ELEM_PTR(*dst, y - 1, x - 1)) = 8 * r[7];
		}
	}
	Mat mm1 = Mat(dst, true);
	imshow("aaa", mm1);
	//cvNamedWindow("aftkirsch2", 1);
	//cvShowImage("aftkirsch2", dst);
	cvReleaseMat(&src_copy);
	cvReleaseMat(&src_copy);
}

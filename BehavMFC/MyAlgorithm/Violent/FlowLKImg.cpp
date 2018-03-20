#include "FlowLKImg.h"

FlowLKImg::FlowLKImg()
{
}

FlowLKImg::~FlowLKImg()
{
}

int FlowLKImg::funMain(Mat mat1, Mat mat2)
{
	//// Initialize, load two images from the file system, and
	//// allocate the images and other structures we will need for
	//// results.
	//IplImage* imgA = cvLoadImage(MYPATHPIC1, CV_LOAD_IMAGE_GRAYSCALE);
	//IplImage* imgB = cvLoadImage(MYPATHPIC2, CV_LOAD_IMAGE_GRAYSCALE);
	//CvSize img_sz = cvGetSize(imgA);
	//IplImage* imgC = cvLoadImage(MYPATHPIC2, CV_LOAD_IMAGE_UNCHANGED);


	//@J interface,mat to iplimage
	out_lkValue = 0.0;
	Mat mat3;
	mat2.copyTo(mat3);
	if (mat1.channels() != 1)
		cvtColor(mat1, mat1, CV_BGR2GRAY);
	if (mat2.channels() != 1)
		cvtColor(mat2, mat2, CV_BGR2GRAY);
	IplImage* imgA = &IplImage(mat1);
	IplImage* imgB = &IplImage(mat2);
	CvSize img_sz = cvGetSize(imgA);
	//show on the second original pic
	IplImage* imgC = &IplImage(mat3);

	int win_size = 10;//窗口尺寸
	// The first thing we need to do is get the features
	// we want to track.
	IplImage* eig_image = cvCreateImage(img_sz, IPL_DEPTH_32F, 1);
	IplImage* tmp_image = cvCreateImage(img_sz, IPL_DEPTH_32F, 1);//临时变量
	int corner_count = MAX_CORNERS;//跟踪点的数目
	CvPoint2D32f* cornersA = new CvPoint2D32f[MAX_CORNERS];//缓存

	cvGoodFeaturesToTrack(
		imgA,//the input image
		eig_image,//temp image whose result is meaningful
		tmp_image,//temp image
		cornersA,//contains the result points
		&corner_count,//the maximum number of points
		0.01,//indicates the minimal acceptable lower eigenvalue for a point to be included as a corner
		5.0,//guarantees that no two returned points are within the indicated number of pixels.
		0,//no mask is used
		3,// the region around a given pixel that is considered when computing the autocorrelation matrix of derivatives.
		0,//use the the Shi-Tomasi deinition
		0.04
		);
	/* Further find more accurate points */
	cvFindCornerSubPix(
		imgA,
		cornersA,
		corner_count,
		cvSize(win_size, win_size),
		cvSize(-1, -1),
		cvTermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03)
		);
	// Call the Lucas Kanade algorithm
	char features_found[MAX_CORNERS];
	float feature_errors[MAX_CORNERS];
	CvSize pyr_sz = cvSize(imgA->width + 8, imgB->height / 3);

	IplImage* pyrA = cvCreateImage(pyr_sz, IPL_DEPTH_32F, 1);
	IplImage* pyrB = cvCreateImage(pyr_sz, IPL_DEPTH_32F, 1);
	CvPoint2D32f* cornersB = new CvPoint2D32f[MAX_CORNERS];
	//金字塔lk光流算法
	cvCalcOpticalFlowPyrLK(
		imgA,
		imgB,
		pyrA,
		pyrB,
		cornersA,
		cornersB,
		corner_count,
		cvSize(win_size, win_size),
		5,
		features_found,
		feature_errors,
		cvTermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, .3),
		0
		);
	// Now make some image of what we are looking at:
	Mat tmp = Mat(imgC, true);
	for (int i = 0; i<corner_count; i++)
	{
		if (features_found[i] == 0 || feature_errors[i]>550)
		{
			 //printf("Error is %f\n",feature_errors[i]);
			continue;
		}
		 //printf("Got it/n");
		out_lkValue += abs(cornersA[i].x - cornersB[i].x) + abs(cornersA[i].y - cornersB[i].y);
		CvPoint p0 = cvPoint(cvRound(cornersA[i].x), cvRound(cornersA[i].y));
		CvPoint p1 = cvPoint(cvRound(cornersB[i].x), cvRound(cornersB[i].y));
		//cvLine(imgC, p0, p1, CV_RGB(255, 0, 0), 2);
		arrowedLine(tmp, p0, p1, CV_RGB(255, 0, 0), 2);
	}
	printf("Value is %f\n", out_lkValue);
	cvNamedWindow("ImageA", 0);
	cvNamedWindow("ImageB", 0);
	cvNamedWindow("LKpyr_OpticalFlow", 0);
	cvShowImage("ImageA", imgA);
	cvShowImage("ImageB", imgB);
	cvShowImage("LKpyr_OpticalFlow", imgC);
	imshow("tmp", tmp);
	//imwrite("arr.jpg", tmp);

	cout << "done";
	cvWaitKey(0);
	return 0;
}
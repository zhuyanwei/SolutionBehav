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
	mat2.copyTo(mat3);
	if (mat1.channels() != 1)
		cvtColor(mat1, mat1, CV_BGR2GRAY);
	if (mat2.channels() != 1)
		cvtColor(mat2, mat2, CV_BGR2GRAY);
	imgA = &IplImage(mat1);
	imgB = &IplImage(mat2);
	img_sz = cvGetSize(imgA);
	//show on the second original pic
	imgC = &IplImage(mat3);

	int win_size = 10;//���ڳߴ�
	// The first thing we need to do is get the features
	// we want to track.
	eig_image = cvCreateImage(img_sz, IPL_DEPTH_32F, 1);
	tmp_image = cvCreateImage(img_sz, IPL_DEPTH_32F, 1);//��ʱ����
	int corner_count = MAX_CORNERS;//���ٵ����Ŀ
	cornersA = new CvPoint2D32f[MAX_CORNERS];//����

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
	pyr_sz = cvSize(imgA->width + 8, imgB->height / 3);

	pyrA = cvCreateImage(pyr_sz, IPL_DEPTH_32F, 1);
	pyrB = cvCreateImage(pyr_sz, IPL_DEPTH_32F, 1);
	cornersB = new CvPoint2D32f[MAX_CORNERS];
	//������lk�����㷨
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
	tmp = Mat(imgC, true);
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
	//cvNamedWindow("ImageA", 0);
	//cvNamedWindow("ImageB", 0);
	//cvNamedWindow("LKpyr_OpticalFlow", 0);
	//cvShowImage("ImageA", imgA);
	//cvShowImage("ImageB", imgB);
	//cvShowImage("LKpyr_OpticalFlow", imgC);
	imshow("tmp", tmp);
	//imwrite("arr.jpg", tmp);

	cout << "done";
	//cvWaitKey(CVWAIT);
	return 0;
}

int FlowLKImg::releasePara()
{
	cvReleaseImage(&imgA);
	cvReleaseImage(&imgB);
	cvReleaseImage(&imgC);
	cvReleaseImage(&eig_image);
	cvReleaseImage(&tmp_image);
	cvReleaseImage(&pyrA);
	cvReleaseImage(&pyrB);

	return 1;
}

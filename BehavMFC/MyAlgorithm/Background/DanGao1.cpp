#include "DanGao1.h"

DanGao1::DanGao1()
{
}

DanGao1::~DanGao1()
{
}

int DanGao1::funMain(string fileName)
{
	//�½�����
	cvNamedWindow("origin", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("background", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("foreground", CV_WINDOW_AUTOSIZE);
	cvMoveWindow("origin", 1100, 400);
	cvMoveWindow("background", 600, 400);
	cvMoveWindow("foreground", 100, 400);

	double alpha = 0.5;//������ģalphaֵ
	double std_int = 20;//��ʼ����׼��
	double var_int = std_int*std_int;//��ʼ������
	double lamda = 2.5*1.2;//�������²���

	//open camera
	CvCapture* capture;
	if (fileName == "00")
	{
		capture = cvCreateCameraCapture(0);
	}
	else
	{
		char* myChar = const_cast<char*>(fileName.c_str());
		capture = cvCreateFileCapture(myChar);
	}
	if (!capture)
	{
		printf("Couldn't Open the file or camera");
		return -1;
	}

	IplImage *frame = NULL;//ԭʼͼ��
	IplImage *frame_u = NULL;//����ͼ�񣨱�����
	IplImage *frame_d = NULL;//ǰ��ͼ��
	IplImage *frame_var = NULL;//����ͼ��
	IplImage *frame_std = NULL;//��׼��

	CvScalar pixel = { 0 };//����ԭʼֵ
	CvScalar pixel_u = { 0 };//��������ֵ
	CvScalar pixel_d = { 0 };//����ǰ��
	CvScalar pixel_var = { 0 };//���ط���
	CvScalar pixel_std = { 0 };//���ر�׼��

	//��ʼ��frame_u,frame_d,frame_var,frame_std
	frame = cvQueryFrame(capture);
	frame_u = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 3);
	frame_d = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 3);
	frame_var = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 3);
	frame_std = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 3);

	for (int y = 0; y<frame->height; ++y)
	{
		for (int x = 0; x<frame->width; ++x)
		{
			pixel = cvGet2D(frame, y, x);

			pixel_u.val[0] = pixel.val[0];
			pixel_u.val[1] = pixel.val[1];
			pixel_u.val[2] = pixel.val[2];

			pixel_d.val[0] = 0;
			pixel_d.val[1] = 0;
			pixel_d.val[2] = 0;

			pixel_std.val[0] = std_int;
			pixel_std.val[1] = std_int;
			pixel_std.val[2] = std_int;

			pixel_var.val[0] = var_int;
			pixel_var.val[1] = var_int;
			pixel_var.val[2] = var_int;

			cvSet2D(frame_u, y, x, pixel_u);
			cvSet2D(frame_d, y, x, pixel_d);
			cvSet2D(frame_var, y, x, pixel_var);
			cvSet2D(frame_std, y, x, pixel_std);
		}
	}

	while (cvWaitKey(CVWAIT) != 'q')//��ESC���˳���֡��33ms
	{
		frame = cvQueryFrame(capture);
		//��Ƶ�����˳�
		if (!frame)
		{
			break;
		}
		//����˹��������
		for (int y = 0; y<frame->height; ++y)
		{
			for (int x = 0; x<frame->width; ++x)
			{
				pixel = cvGet2D(frame, y, x);
				pixel_u = cvGet2D(frame_u, y, x);
				pixel_d = cvGet2D(frame_d, y, x);
				pixel_std = cvGet2D(frame_std, y, x);
				pixel_var = cvGet2D(frame_var, y, x);

				//|I-u|<lamda*stdʱ��Ϊ�Ǳ��������и���
				if (fabs(pixel.val[0] - pixel_u.val[0])<lamda*pixel_std.val[0] &&
					fabs(pixel.val[1] - pixel_u.val[1])<lamda*pixel_std.val[1] &&
					fabs(pixel.val[2] - pixel_u.val[2])<lamda*pixel_std.val[2])
				{
					//��������u=(1-alpha)*u+alpha*I
					pixel_u.val[0] = (1 - alpha)*pixel_u.val[0] + alpha*pixel.val[0];
					pixel_u.val[1] = (1 - alpha)*pixel_u.val[1] + alpha*pixel.val[1];
					pixel_u.val[2] = (1 - alpha)*pixel_u.val[2] + alpha*pixel.val[2];

					//���·���var=(1-alpha)*var+alpha*(I-u)^2
					pixel_var.val[0] = (1 - alpha)*pixel_var.val[0] +
						alpha*(pixel.val[0] - pixel_u.val[0])*(pixel.val[0] - pixel_u.val[0]);
					pixel_var.val[1] = (1 - alpha)*pixel_var.val[1] +
						alpha*(pixel.val[1] - pixel_u.val[1])*(pixel.val[1] - pixel_u.val[1]);
					pixel_var.val[2] = (1 - alpha)*pixel_var.val[2] +
						alpha*(pixel.val[2] - pixel_u.val[2])*(pixel.val[2] - pixel_u.val[2]);
					//���±�׼��
					pixel_std.val[0] = sqrt(pixel_var.val[0]);
					pixel_std.val[1] = sqrt(pixel_var.val[1]);
					pixel_std.val[2] = sqrt(pixel_var.val[2]);

					//д�����
					cvSet2D(frame_u, y, x, pixel_u);
					cvSet2D(frame_var, y, x, pixel_var);
					cvSet2D(frame_std, y, x, pixel_std);
				}
				else
				{
					pixel_d.val[0] = pixel.val[0] - pixel_u.val[0];
					pixel_d.val[1] = pixel.val[1] - pixel_u.val[1];
					pixel_d.val[2] = pixel.val[2] - pixel_u.val[2];
					cvSet2D(frame_d, y, x, pixel_d);
				}
			}
		}
		//��ʾ���
		frame_u->origin = 1;
		frame_u->origin = 1;
		cvShowImage("origin", frame);
		cvShowImage("background", frame_u);
		cvShowImage("foreground", frame_d);
	}
	//�ͷ��ڴ�
	cvReleaseCapture(&capture);
	cvReleaseImage(&frame);
	cvReleaseImage(&frame_u);
	cvReleaseImage(&frame_var);
	cvReleaseImage(&frame_std);
	cvDestroyWindow("origin");
	cvDestroyWindow("background");
	cvDestroyWindow("foreground");

	return 1;
}

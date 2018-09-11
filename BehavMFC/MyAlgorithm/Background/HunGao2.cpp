#include "HunGao2.h"

HunGao2::HunGao2()
{
}

HunGao2::~HunGao2()
{
}

int HunGao2::funMain(string fileName)
{
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

	IplImage *frame, *grayFrame, *foreground, *background;

	int *foreg, *backg, *rank_index;

	double *weight, *mean, *sigma, *u_diff, *rank;

	double p = ALPHA / (1 / (double)COMPONET);

	double rank_temp = 0;
	int rank_index_temp = 0;

	CvRNG state;  //随机生成状态器

	int match, height, width;

	frame = cvQueryFrame(capture);
	grayFrame = cvCreateImage(cvSize(frame->width,frame->height), IPL_DEPTH_8U, 1);
	foreground = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 1);
	background = cvCreateImage(cvSize(frame->width, frame->height), IPL_DEPTH_8U, 1);

	height = grayFrame->height;
	width = grayFrame->widthStep;

	foreg = (int*)malloc(sizeof(int)*width*height);
	backg = (int*)malloc(sizeof(int)*width*height);

	rank = (double*)malloc(sizeof(double) * 1 * COMPONET);    //优先级
	weight = (double*)malloc(sizeof(double)*width*height*COMPONET); //权重
	mean = (double *)malloc(sizeof(double)*width*height*COMPONET);         //pixel means
	sigma = (double *)malloc(sizeof(double)*width*height*COMPONET);           //pixel standard deviations
	u_diff = (double *)malloc(sizeof(double)*width*height*COMPONET);       //difference of each pixel from mean

	//初始化均值、方差、权重
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			for (int k = 0; k < COMPONET; k++)
			{
				mean[i*width*COMPONET + j*COMPONET + k] = cvRandReal(&state) * 255;
				sigma[i*width*COMPONET + j*COMPONET + k] = SD_INIT;
				weight[i*width*COMPONET + j*COMPONET + k] = (double)1 / COMPONET;
			}
		}
	}

	while (1){
		rank_index = (int *)malloc(sizeof(int)*COMPONET);
		cvCvtColor(frame, grayFrame, CV_BGR2GRAY);

		// calculate difference of pixel values from mean
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				for (int k = 0; k < COMPONET; k++)
				{
					u_diff[i*width*COMPONET + j*COMPONET + k] = abs((uchar)grayFrame->imageData[i*width + j] - mean[i*width*COMPONET + j*COMPONET + k]);
				}
			}
		}

		//update gaussian components for each pixel
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				match = 0;
				double sum_weight = 0;
				for (int k = 0; k < COMPONET; k++)
				{
					if (u_diff[i*width*COMPONET + j*COMPONET + k] <= D*sigma[i*width*COMPONET + j*COMPONET + k])      //pixel matches component
					{
						match = 1;                                                 // variable to signal component match

						//update weights, mean, sd, p
						weight[i*width*COMPONET + j*COMPONET + k] = (1 - ALPHA)*weight[i*width*COMPONET + j*COMPONET + k] + ALPHA;
						/*p = ALPHA / weight[i*width*COMPONET + j*COMPONET + k];
						mean[i*width*COMPONET + j*COMPONET + k] = (1 - p)*mean[i*width*COMPONET + j*COMPONET + k] + p*(uchar)grayFrame->imageData[i*width + j];
						sigma[i*width*COMPONET + j*COMPONET + k] = sqrt((1 - p)*(sigma[i*width*COMPONET + j*COMPONET + k] * sigma[i*width*COMPONET + j*COMPONET + k]) + p*(pow((uchar)grayFrame->imageData[i*width + j] - mean[i*width*COMPONET + j*COMPONET + k], 2)));
						*/
						mean[i*width*COMPONET + j*COMPONET + k] = (1 - ALPHA)*mean[i*width*COMPONET + j*COMPONET + k] + ALPHA*(uchar)grayFrame->imageData[i*width + j];
						sigma[i*width*COMPONET + j*COMPONET + k] = sqrt((1 - ALPHA)*(sigma[i*width*COMPONET + j*COMPONET + k] * sigma[i*width*COMPONET + j*COMPONET + k]) + ALPHA*(pow((uchar)grayFrame->imageData[i*width + j] - mean[i*width*COMPONET + j*COMPONET + k], 2)));

					}
					//else{
					//  weight[i*width*COMPONET + j*COMPONET + k] = (1 - ALPHA)*weight[i*width*COMPONET + j*COMPONET + k];           // weight slighly decreases
					//}
					sum_weight += weight[i*width*COMPONET + j*COMPONET + k];
				}

				//权重归一化
				for (int k = 0; k < COMPONET; k++)
				{
					weight[i*width*COMPONET + j*COMPONET + k] = weight[i*width*COMPONET + j*COMPONET + k] / sum_weight;
				}

				//获取权重最小下标
				double temp = weight[i*width*COMPONET + j*COMPONET];
				int min_index = 0;
				backg[i*width + j] = 0;
				for (int k = 0; k < COMPONET; k++)
				{
					backg[i*width + j] = backg[i*width + j] + mean[i*width*COMPONET + j*COMPONET + k] * weight[i*width*COMPONET + j*COMPONET + k];
					if (weight[i*width*COMPONET + j*COMPONET + k] < temp)
					{
						min_index = k;
						temp = weight[i*width*COMPONET + j*COMPONET + k];
					}
					rank_index[k] = k;
				}

				background->imageData[i*width + j] = (uchar)backg[i*width + j];

				//if no components match, create new component
				if (match == 0)
				{
					mean[i*width*COMPONET + j*COMPONET + min_index] = (uchar)grayFrame->imageData[i*width + j];
					sigma[i*width*COMPONET + j*COMPONET + min_index] = SD_INIT;
					weight[i*width*COMPONET + j*COMPONET + min_index] = 1 / COMPONET;
				}

				//计算优先级
				for (int k = 0; k < COMPONET; k++)
				{
					rank[k] = weight[i*width*COMPONET + j*COMPONET + k] / sigma[i*width*COMPONET + j*COMPONET + k];
				}

				//sort rank values
				for (int k = 1; k < COMPONET; k++)
				{
					for (int m = 0; m < k; m++)
					{
						if (rank[k] > rank[m])
						{
							//swap max values
							rank_temp = rank[m];
							rank[m] = rank[k];
							rank[k] = rank_temp;

							//swap max index values
							rank_index_temp = rank_index[m];
							rank_index[m] = rank_index[k];
							rank_index[k] = rank_index_temp;
						}
					}
				}

				//calculate foreground
				match = 0;
				int b = 0;

				while ((match == 0) && (b < COMPONET))
				{
					if (weight[i*width*COMPONET + j*COMPONET + rank_index[b]] >= THRESHOLD)
					{
						if (abs(u_diff[i*width*COMPONET + j*COMPONET + rank_index[b]]) <= D*sigma[i*width*COMPONET + j*COMPONET + rank_index[b]])
						{
							foreground->imageData[i*width + j] = 0;
							match = 1;
						}
						else
						{
							foreground->imageData[i*width + j] = (uchar)grayFrame->imageData[i*width + j];
						}
					}
					b++;
				}
			}
		}

		frame = cvQueryFrame(capture);
		cvShowImage("fore", foreground);
		cvShowImage("back", background);
		cvShowImage("frame", frame);
		char s = cvWaitKey(CVWAIT);
		if (s == 'q') break;
		free(rank_index);
	}
	return 0;
}
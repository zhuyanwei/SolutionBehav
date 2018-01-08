#include "MatchTemplate.h"

MatchTemplate::MatchTemplate()
{
}

MatchTemplate::~MatchTemplate()
{
}

// bounding box mouse callback
//void mouseHandler(int event, int x, int y, int flags, void *param){
//
//	switch (event){
//
//	case CV_EVENT_MOUSEMOVE:
//
//		if (drawing_box){
//
//			box.width = x - box.x;
//
//			box.height = y - box.y;
//
//		}
//
//		break;
//
//	case CV_EVENT_LBUTTONDOWN:
//
//		drawing_box = true;
//
//		box = Rect(x, y, 0, 0);
//
//		break;
//
//	case CV_EVENT_LBUTTONUP:
//
//		drawing_box = false;
//
//		if (box.width < 0){
//
//			box.x += box.width;
//
//			box.width *= -1;
//
//		}
//
//		if (box.height < 0){
//
//			box.y += box.height;
//
//			box.height *= -1;
//
//		}
//
//		gotBB = true;
//
//		break;
//
//	}
//
//}



// tracker: get search patches around the last tracking box,
// and find the most similar one
//void tracking(Mat frame, Mat &model, Rect &trackBox)
//{
//
//	Mat gray;
//
//	cvtColor(frame, gray, CV_RGB2GRAY);
//
//
//
//	Rect searchWindow;
//
//	searchWindow.width = trackBox.width * 3;
//
//	searchWindow.height = trackBox.height * 3;
//
//	searchWindow.x = trackBox.x + trackBox.width * 0.5 - searchWindow.width * 0.5;
//
//	searchWindow.y = trackBox.y + trackBox.height * 0.5 - searchWindow.height * 0.5;
//
//	searchWindow &= Rect(0, 0, frame.cols, frame.rows);
//
//
//
//	Mat similarity;
//
//	matchTemplate(gray(searchWindow), model, similarity, CV_TM_CCOEFF_NORMED);
//
//
//
//	double mag_r;
//
//	Point point;
//
//	minMaxLoc(similarity, 0, &mag_r, 0, &point);
//
//	trackBox.x = point.x + searchWindow.x;
//
//	trackBox.y = point.y + searchWindow.y;
//
//	model = gray(trackBox);
//
//}

int MatchTemplate::funMain()
{
	cout << "11";
	//CvCapture *capture = cvCaptureFromCAM(0);



	//bool fromfile = true;

	////Init camera

	//if (!capture)

	//{

	//	cout << "capture device failed to open!" << endl;

	//	return -1;

	//}

	////Register mouse callback to draw the bounding box

	//cvNamedWindow("Tracker", CV_WINDOW_AUTOSIZE);

	//cvSetMouseCallback("Tracker", mouseHandler, NULL);



	//Mat frame, model;

	//frame = cvQueryFrame(capture);

	//while (!gotBB)

	//{

	//	frame = cvQueryFrame(capture);



	//	IplImage fff = frame;

	//	cvShowImage("Tracker", &fff);

	//	if (cvWaitKey(20) == 'q')

	//		return 1;

	//}

	////Remove callback

	//cvSetMouseCallback("Tracker", NULL, NULL);



	//Mat gray;

	//cvtColor(frame, gray, CV_RGB2GRAY);

	//model = gray(box);



	//int frameCount = 0;



	//while (1)

	//{

	//	frame = cvQueryFrame(capture);

	//	if (frame.empty())

	//		return -1;

	//	double t = (double)cvGetTickCount();

	//	frameCount++;



	//	// tracking

	//	tracking(frame, model, box);



	//	// show

	//	stringstream buf;

	//	buf << frameCount;

	//	string num = buf.str();



	//	IplImage fff = frame;

	//	CvPoint p1, p2;

	//	p1 = cvPoint(box.x, box.y);

	//	p2 = cvPoint(box.x + box.width, box.y + box.height);

	//	cvRectangle(&fff, p1, p2, Scalar(0, 0, 255));

	//	cvShowImage("Tracker", &fff);



	//	t = (double)cvGetTickCount() - t;

	//	cout << "cost time: " << t / ((double)cvGetTickFrequency()*1000.) << endl;



	//	if (cvWaitKey(1) == 27)

	//		break;



	//}



	return 0;

}
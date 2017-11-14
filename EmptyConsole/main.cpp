#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
int main()
{
	std::cout << "hello";

	Mat m1;
	m1 = imread("lena.jpg");
	imshow("ss", m1);
	waitKey();
	int i;
	std::cin >> i;
	return 0;
}
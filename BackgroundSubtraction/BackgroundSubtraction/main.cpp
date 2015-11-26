#include "opencv2/opencv.hpp"

using namespace cv;

int main(int, char)
{
	
	Mat edges;
	Mat edges2;
	Mat edges3;
	namedWindow("edges", 1);
	namedWindow("edges the second", 1);
	for (;;)
	{
		Mat image = imread("C:/users/Johan/pictures/1.jpg",CV_LOAD_IMAGE_COLOR);
		Mat frame = imread("C:/users/Johan/pictures/2.jpg", CV_LOAD_IMAGE_COLOR); // get a new frame from camera
		cvtColor(frame, edges, CV_BGR2GRAY);
		cvtColor(image, edges2, CV_BGR2GRAY);
		GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
		GaussianBlur(edges2, edges2, Size(7, 7), 1.5, 1.5);
		Canny(edges, edges, 0, 30, 3);
		Canny(edges2, edges2, 0, 30, 3);
		imshow("edges", edges);
		imshow("edges the second", edges2);
		Mat bg = frame - image;
		cvtColor(bg, edges3, CV_BGR2GRAY);
		GaussianBlur(edges3, edges3, Size(7, 7), 1.5, 1.5);
		Canny(edges3, edges3, 0, 30, 3);
		imshow("blak", edges3);
		if (waitKey(30) >= 0)
			break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}
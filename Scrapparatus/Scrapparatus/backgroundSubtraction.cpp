#include "opencv2/opencv.hpp"

cv::Mat BackgroundSubtract(cv::Mat firstFrame, cv::Mat startingImage)
{
	cv::Mat backgSub;
	//namedWindow("edges", 1);
	//namedWindow("edges the second", 1);
		cv::Mat edges2 = firstFrame;
		cv::Mat edges = startingImage; // get a new frame from camera
		GaussianBlur(edges, edges, cv::Size(7, 7), 1.5, 1.5);
		GaussianBlur(edges2, edges2, cv::Size(7, 7), 1.5, 1.5);
		Canny(edges, edges, 0, 30, 5);
		Canny(edges2, edges2, 0, 30, 5);
		cv::Mat bg = edges - edges2;
		//GaussianBlur(bg, backgSub, cv::Size(7, 7), 1.5, 1.5);
		//Canny(backgSub, backgSub, 0, 30, 3);
	// the camera will be deinitialized automatically in VideoCapture destructor
	return bg;
}
#include "opencv2/opencv.hpp"

cv::Mat BackgroundSubtract(cv::Mat firstFrame, cv::Mat startingImage)
{
	cv::Mat backgSub;
	//namedWindow("edges", 1);
	//namedWindow("edges the second", 1);
		cv::Mat edges2 = firstFrame;
		cv::Mat edges = startingImage; // get a new frame from camera

		cv::Mat bg = edges - edges2;

		for (int i = 0; i < bg.rows; i++){
			for (int j = 0; j < bg.cols; j++){
				if (bg.at<uchar>(i, j) > 50){
					bg.at<uchar>(i, j) = 255;
				}
				else{
					bg.at<uchar>(i, j) = 0;
				}
			}
		}

		GaussianBlur(bg, bg, cv::Size(7, 7), 1.5, 1.5);
		//GaussianBlur(edges2, edges2, cv::Size(7, 7), 1.5, 1.5);
		Canny(bg, bg, 0, 30, 3);
		//Canny(edges2, edges2, 0, 30, 3);
		//GaussianBlur(bg, backgSub, cv::Size(7, 7), 1.5, 1.5);
		//Canny(backgSub, backgSub, 0, 30, 3);
	// the camera will be deinitialized automatically in VideoCapture destructor
	return bg;
}
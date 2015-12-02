#include "opencv2/opencv.hpp"

cv::Mat BackgroundSubtract(cv::Mat firstFrame, cv::Mat startingImage)
{
	cv::Mat backgSub;

		cv::Mat edges2 = firstFrame;
		cv::Mat edges = startingImage; // get a new frame from camera

		cv::Mat bg = edges - edges2;

		for (int i = 0; i < bg.rows; i++){
			for (int j = 0; j < bg.cols; j++){
				if (bg.at<uchar>(i, j) > 70){
					bg.at<uchar>(i, j) = 255;
				}
				else{
					bg.at<uchar>(i, j) = 0;
				}
			}
		}



	return bg;
}
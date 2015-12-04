#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

cv::Mat Dialate(cv::Mat inImage, int threshold){

	cv::Mat outImage = inImage.clone();
	outImage = cv::Mat::zeros(inImage.rows, inImage.cols, inImage.type());
	for (int run = 0; run < 3; run++){
	for (int i = 1; i < inImage.rows - 1; i++){
		for (int j = 1; j < inImage.cols - 1; j++){

			int sumVal = 
				inImage.at<uchar>(i - 2, j + 2) + inImage.at<uchar>(i - 2, j + 1) + inImage.at<uchar>(i - 2, j + 1) + inImage.at<uchar>(i - 2, j + 1) + inImage.at<uchar>(i - 2, j + 2) +
				inImage.at<uchar>(i - 1, j - 2) + inImage.at<uchar>(i - 1, j-1) + inImage.at<uchar>(i - 1, j) + inImage.at<uchar>(i - 1, j + 1) + inImage.at<uchar>(i - 1, j + 2) +
				+inImage.at<uchar>(i, j - 2) + inImage.at<uchar>(i, j - 1) + inImage.at<uchar>(i, j) + inImage.at<uchar>(i, j + 1) + inImage.at<uchar>(i, j+2)
				+ inImage.at<uchar>(i + 1, j - 2) + inImage.at<uchar>(i + 1, j-1) + inImage.at<uchar>(i + 1, j)+ inImage.at<uchar>(i-1, j+1)+inImage.at<uchar>(i-1, j+2)
				+ inImage.at<uchar>(i+2, j-2) + inImage.at<uchar>(i+2, j-1) + inImage.at<uchar>(i+2, j) + inImage.at<uchar>(i+2, j+1) + inImage.at<uchar>(i+2, j+2);

			if (sumVal > threshold){
				outImage.at<uchar>(i, j) = 255;
			}
			else{
				outImage.at<uchar>(i, j) = 0;
			}
		}
	}
	}
	return outImage;
}
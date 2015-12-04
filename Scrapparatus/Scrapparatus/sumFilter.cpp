#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

cv::Mat sumFilter(cv::Mat inImage, cv::Mat outImage, int threshold){

	outImage = cv::Mat::zeros(inImage.rows, inImage.cols, inImage.type());

	for (int i = 1; i < inImage.rows-1; i++){
		for (int j = 1; j < inImage.cols-1; j++){

			int sumVal =
				inImage.at<uchar>(i - 1, j)
				+ inImage.at<uchar>(i, j - 1) + inImage.at<uchar>(i, j + 1)
				+ inImage.at<uchar>(i + 1, j);

			if (sumVal == threshold){
				outImage.at<uchar>(i, j) = 255;
			}else
				outImage.at<uchar>(i, j) = 0;
		}
	}

	return outImage;
}
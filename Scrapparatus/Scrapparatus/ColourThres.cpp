#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

cv::Mat ColourThres(cv::Mat inImage){

	cv::Mat outImage = inImage.clone();
	outImage = cv::Mat::zeros(inImage.rows, inImage.cols, inImage.type());

	int ColourRed = 0, ColourGreen = 0, ColourBlue = 0;

	for (int y = 0; y < inImage.rows; ++y){
		for (int x = 0; x < inImage.cols; ++x){
			ColourBlue = inImage.at<cv::Vec3b>(y, x)[0];
			ColourGreen = inImage.at<cv::Vec3b>(y, x)[1];
			ColourRed = inImage.at<cv::Vec3b>(y, x)[2];
			if (ColourBlue < 5 && ColourGreen > 150 && ColourGreen < 210 && ColourRed > 85 && ColourRed < 125){
				outImage.at<cv::Vec3b>(y, x)[0] = 0;
				outImage.at<cv::Vec3b>(y, x)[1] = 0;
				outImage.at<cv::Vec3b>(y, x)[2] = 0;
			}
			else{
				outImage.at<cv::Vec3b>(y, x)[0] = inImage.at<cv::Vec3b>(y, x)[0];
				outImage.at<cv::Vec3b>(y, x)[1] = inImage.at<cv::Vec3b>(y, x)[1];
				outImage.at<cv::Vec3b>(y, x)[2] = inImage.at<cv::Vec3b>(y, x)[2];
			}
			if (ColourBlue < 143 && ColourBlue > 103 && ColourGreen > 141 && ColourGreen < 161 && ColourRed > 108 && ColourRed < 148){
				outImage.at<cv::Vec3b>(y, x)[0] = 0;
				outImage.at<cv::Vec3b>(y, x)[1] = 0;
				outImage.at<cv::Vec3b>(y, x)[2] = 0;
			}
			else{
				outImage.at<cv::Vec3b>(y, x)[0] = inImage.at<cv::Vec3b>(y, x)[0];
				outImage.at<cv::Vec3b>(y, x)[1] = inImage.at<cv::Vec3b>(y, x)[1];
				outImage.at<cv::Vec3b>(y, x)[2] = inImage.at<cv::Vec3b>(y, x)[2];
			}
		}
	}
			return outImage;
}
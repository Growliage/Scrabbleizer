//#include <iostream>
#include "opencv2\core\core.hpp"
//#include "opencv2\highgui\highgui.hpp"

cv::Mat factorScaling(float Sx, float Sy, cv::Mat imageIn)	{
	cv::Mat imageOut(imageIn.rows * Sy, imageIn.cols * Sx, CV_8UC1, cv::Scalar(0));

	for (int outY = 0; outY < imageOut.rows; outY++){
		for (int outX = 0; outX < imageOut.cols; outX++){
			imageOut.at<uchar>(outY, outX) = imageIn.at<uchar>((1 / Sy) * outY, (1 / Sx) * outX);
		}
	}

	return(imageOut);
}

cv::Mat pixelScaling(int Px, int Py, cv::Mat imageIn){

	//Calculate the scaling factors from input
float Sx = float(Px / imageIn.rows);
float Sy = float(Py / imageIn.cols);

return(factorScaling(Sx, Sy, imageIn));

}

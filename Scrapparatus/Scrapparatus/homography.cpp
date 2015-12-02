#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <vector>

using namespace cv;
using namespace std;

Mat A = (Mat_<double>(8, 1) << 0, 0, 0, 0, 0, 0, 0, 0); //We need this matrix in both the calculations for the coefficents and the calculations

double homography(int x1, int x2, int x3, int x4, int y1, int y2, int y3, int y4, cv::Point topLeft, cv::Point bottomRight){ //8 points are needed for this. The points should correspond to each other in the two pictures.

	Mat M = (Mat_<double>(8, 8) << x1, y1, 1, 0, 0, 0, -(x1*topLeft.x), -(y1*topLeft.y), //The matrix holding the linear system that has to be used
		0, 0, 0, x1, y1, 1, -(x1*topLeft.x), -(y1*topLeft.y),
		x2, y2, 1, 0, 0, 0, -(x2*bottomRight.x), -(y2*topLeft.y),
		0, 0, 0, x2, y2, 1, -(x2*bottomRight.x), -(y2*topLeft.y),
		x3, y3, 1, 0, 0, 0, -(x3*topLeft.x), -(y3*bottomRight.y),
		0, 0, 0, x3, y3, 1, -(x3*topLeft.x), -(y3*bottomRight.y),
		x4, y4, 1, 0, 0, 0, -(x4*bottomRight.x), -(y4*bottomRight.y),
		0, 0, 0, x4, y4, 1, -(x4*bottomRight.x), -(y4*bottomRight.y));

	Mat B = (Mat_<double>(8, 1) << topLeft.x, topLeft.y, bottomRight.x, topLeft.y, topLeft.x, bottomRight.y, bottomRight.x, bottomRight.y); //Our output image coordinates. This is used for our coefficient calculations.


	Mat gh = M.inv(); //Inverses our matrix.

	for (int i = 0; i < 8; ++i){ //Calculates out coefficients and puts them in the matrix we initialiazed at the beginning
		for (int j = 0; j < 8; ++j){
			A.at<double>(i, 0) = A.at<double>(i, 0) + gh.at<double>(i, j) * B.at<double>(j, 0);
		}
	}
	return 0;
}

Point2d homog(int x, int y){ //Calculates what the new point's x-value will be in the second image
	double xO = 0, yO = 0;
	xO = (( A.at<double>(0,0) * x + A.at<double>(1,0) * y + A.at<double>(2,0)) / (A.at<double>(6,0) * x + A.at<double>(7,0) * y + 1));
	yO = (A.at<double>(3, 0) * x + A.at<double>(4, 0) * y + A.at<double>(5, 0)) / (A.at<double>(6, 0) * x + A.at<double>(7, 0) * y + 1);
	Point2d Out(xO, yO);
	return Out;
}



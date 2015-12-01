#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;



void MyFilledCircle(Mat img, Point center) //This is simply to draw circles on the pictures so the two can be checked. This will not be in the final program
{
	int thickness = -1;
	int lineType = 8;
	circle(img,
		center,
		10,
		Scalar(0, 0, 0),
		thickness,
		lineType);
}

double homography(Vec vector, Point topLeft, Point bottomRight){ //8 points are needed for this. The points should correspond to each other in the two pictures.

	Mat A = (Mat_<double>(8, 1) << 0, 0, 0, 0, 0, 0, 0, 0); //We need this matrix in both the calculations for the coefficents and the calculations

	Mat M = (Mat_<double>(8, 8) << vector[0].first, vector[0].second, 1, 0, 0, 0, -(vector[0].first*topLeft.x), -(vector[0].second*topLeft.y), //The matrix holding the linear system that has to be used
		0, 0, 0, vector[0].first, vector[0].second, 1, -(vector[0].first*topLeft.x), -(vector[0].second*topLeft.y),
		vector[1].first, vector[1].second, 1, 0, 0, 0, -(vector[1].first*bottomRight.x), -(vector[1].second*topLeft.y),
		0, 0, 0, vector[1].first, vector[1].second, 1, -(vector[1].first*bottomRight.x), -(vector[1].second*topLeft.y),
		vector[2].first, vector[2].second, 1, 0, 0, 0, -(vector[2].first*topLeft.x), -(vector[2].second*bottomRight.y),
		0, 0, 0, vector[2].first, vector[2].second, 1, -(vector[2].first*topLeft.x), -(vector[2].second*bottomRight.y),
		vector[3].first, vector[3].second, 1, 0, 0, 0, -(vector[3].first*bottomRight.x), -(vector[3].second*bottomRight.y),
		0, 0, 0, vector[3].first, vector[3].second, 1, -(vector[3].first*bottomRight.x), -(vector[3].second*bottomRight.y));

	Mat B = (Mat_<double>(8, 1) << topLeft.x, topLeft.y, bottomRight.x, topLeft.y, topLeft.x, bottomRight.y, bottomRight.x, bottomRight.y); //Our output image coordinates. This is used for our coefficient calculations.


	Mat gh = M.inv(); //Inverses our matrix.

	for (int i = 0; i < 8; ++i){ //Calculates out coefficients and puts them in the matrix we initialiazed at the beginning
		for (int j = 0; j < 8; ++j){
			A.at<double>(i, 0) = A.at<double>(i, 0) + gh.at<double>(i, j) * B.at<double>(j, 0);
		}
	}
	
	
	return A;
}

double homog(int x, int y){ //Calculates what the new point's x-value will be in the second image
	double xO = 0, yO = 0;
	xO = (( A.at<double>(0,0) * x + A.at<double>(1,0) * y + A.at<double>(2,0)) / (A.at<double>(6,0) * x + A.at<double>(7,0) * y + 1));
	yO = (A.at<double>(3, 0) * x + A.at<double>(4, 0) * y + A.at<double>(5, 0)) / (A.at<double>(6, 0) * x + A.at<double>(7, 0) * y + 1);
	Point2f Out(xO, yO);
	return xO, yO;
}


int main(int, char) //Most of the image shows in here won't actually be used. But it is also through this we get the coordinates for all the calculations
{
	double x1 = 0, x2 = 0, x3 = 0, x4 = 0, y1 = 0, y2 = 0, y3 = 0, y4 = 0, xO1 = 0, xO2 = 0, xO3 = 0, xO4 = 0, yO1 = 0, yO2 = 0, yO3 = 0, yO4= 0;
	double xI1 = 0, yI1 = 0;
	Mat img = imread("C:/Users/Johan/pictures/snap1.jpg", CV_LOAD_IMAGE_COLOR); 
	Mat img2 = imread("C:/Users/Johan/pictures/snap2.jpg", CV_LOAD_IMAGE_COLOR);
	namedWindow("snoot", CV_WINDOW_AUTOSIZE);
	namedWindow("toot", CV_WINDOW_AUTOSIZE);
	imshow("snoot", img);
	imshow("toot", img2);
	waitKey(0);
	cout << "Choose 4 points in the original image ";
	cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4;
	cout << "Choose the corralating 4 points in the second image ";
	cin >> xO1 >> yO1 >> xO2 >> yO2 >> xO3 >> yO3 >> xO4 >> yO4;
	cout << "Choose a new point to find the corralating point in the other picture ";
	cin >> xI1 >> yI1;
	homography(x1, x2, x3, x4, y1, y2, y3, y4, xO1, xO2, xO3, xO4, yO1, yO2, yO3, yO4);
	
	MyFilledCircle(img, Point(x1, y1));
	MyFilledCircle(img, Point(x2, y2));
	MyFilledCircle(img, Point(x3, y3));
	MyFilledCircle(img, Point(x4, y4));
	MyFilledCircle(img2, Point(xO1, yO1));
	MyFilledCircle(img2, Point(xO2, yO2));
	MyFilledCircle(img2, Point(xO3, yO3));
	MyFilledCircle(img2, Point(xO4, yO4));

	
	MyFilledCircle(img, Point(xI1, yI1));
	MyFilledCircle(img2, Point(homogx(xI1, yI1), homogy(xI1, yI1)));
	cout << "The point in " << xI1 << " , " << yI1 << " will be in " << homogx(xI1, yI1) << " , " << homogy(xI1, yI1);

	imshow("snoot", img);
	imshow("toot", img2);
	
	waitKey(0);
	return 0;
}




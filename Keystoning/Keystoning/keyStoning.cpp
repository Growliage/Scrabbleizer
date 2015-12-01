#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

Mat A = (Mat_<double>(8, 1) << 0, 0, 0, 0, 0, 0, 0, 0); //We need this matrix in both the calculations for the coefficents and the calculations

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

double homography(int x1, int x2, int x3, int x4, int y1, int y2, int y3, int y4, int xO1, int xO2, int xO3, int xO4, int yO1, int yO2, int yO3, int yO4, int xI, int yI){ //8 points are needed for this. The points should correspond to each other in the two pictures.

	Mat M = (Mat_<double>(8, 8) << x1, y1, 1, 0, 0, 0, -(x1*xO1), -(y1*xO1), //The matrix holding the linear system that has to be used
		0, 0, 0, x1, y1, 1, -(x1*yO1), -(y1*yO1),
		x2, y2, 1, 0, 0, 0, -(x2*xO2), -(y2*xO2),
		0, 0, 0, x2, y2, 1, -(x2*yO2), -(y2*yO2),
		x3, y3, 1, 0, 0, 0, -(x3*xO3), -(y3*xO3),
		0, 0, 0, x3, y3, 1, -(x3*yO3), -(y3*yO3),
		x4, y4, 1, 0, 0, 0, -(x4*xO4), -(y4*xO4),
		0, 0, 0, x4, y4, 1, -(x4*yO4), -(y4*yO4));

	Mat B = (Mat_<double>(8, 1) << xO1, yO1, xO2, yO2, xO3, yO3, xO4, yO4); //Our output image coordinates. This is used for our coefficient calculations.


	Mat gh = M.inv(); //Inverses our matrix.

	for (int i = 0; i < 8; ++i){ //Calculates out coefficients and puts them in the matrix we initialiazed at the beginning
		for (int j = 0; j < 8; ++j){
			A.at<double>(i, 0) = A.at<double>(i, 0) + gh.at<double>(i, j) * B.at<double>(j, 0);
		}
	}
	waitKey(0); //From this we can now insert a random point, x, and calculate where it will be in the picture, xO.
	waitKey(0);
	
	
	return 0;
}

double homography(Point tLeft, Point tRight, Point bLeft, Point bRight, Point tLeft, Point tRight, Point bLeft, Point bRight){ //8 points are needed for this. The points should correspond to each other in the two pictures.

	Mat M = (Mat_<double>(8, 8) << x1, y1, 1, 0, 0, 0, -(x1*xO1), -(y1*xO1), //The matrix holding the linear system that has to be used
		0, 0, 0, x1, y1, 1, -(x1*yO1), -(y1*yO1),
		x2, y2, 1, 0, 0, 0, -(x2*xO2), -(y2*xO2),
		0, 0, 0, x2, y2, 1, -(x2*yO2), -(y2*yO2),
		x3, y3, 1, 0, 0, 0, -(x3*xO3), -(y3*xO3),
		0, 0, 0, x3, y3, 1, -(x3*yO3), -(y3*yO3),
		x4, y4, 1, 0, 0, 0, -(x4*xO4), -(y4*xO4),
		0, 0, 0, x4, y4, 1, -(x4*yO4), -(y4*yO4));
	
	Mat B = (Mat_<double>(8, 1) << xO1, yO1, xO2, yO2, xO3, yO3, xO4, yO4); //Our output image coordinates. This is used for our coefficient calculations.


	Mat gh = M.inv(); //Inverses our matrix.

	for (int i = 0; i < 8; ++i){ //Calculates out coefficients and puts them in the matrix we initialiazed at the beginning
		for (int j = 0; j < 8; ++j){
			A.at<double>(i, 0) = A.at<double>(i, 0) + gh.at<double>(i, j) * B.at<double>(j, 0);
		}
	}
	waitKey(0); //From this we can now insert a random point, x, and calculate where it will be in the picture, xO.
	waitKey(0);


	return 0;
}

double homogx(int x, int y){ //Calculates what the new point's x-value will be in the second image
	double xO = 0;
	xO = (( A.at<double>(0,0) * x + A.at<double>(1,0) * y + A.at<double>(2,0)) / (A.at<double>(6,0) * x + A.at<double>(7,0) * y + 1));
	return xO;
}

double homogy(int x, int y){ //Calculates what the new point's y-value will be in the second image
	double yO = 0;
	yO = (A.at<double>(3,0) * x + A.at<double>(4,0) * y + A.at<double>(5,0)) / (A.at<double>(6,0) * x + A.at<double>(7,0) * y + 1);
	return yO;
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
	homography(x1, x2, x3, x4, y1, y2, y3, y4, xO1, xO2, xO3, xO4, yO1, yO2, yO3, yO4, xI1, yI1);
	
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




#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>
#include <utility>

std::vector<std::pair<int, int>> boardDetection(cv::Mat image);

int main(){

	cv::Mat image;
	image = cv::imread("C:/users/Bo/Pictures/boardm.jpg");
	cv::resize(image, image, cv::Size(), 0.25, 0.25);

	std::vector<std::pair<int, int>>Board = boardDetection(image);
	//draw circles at line-intersection
	for (int i = 0; i < Board.size(); i++)
	{
		int x = Board[i].first;
		int y = Board[i].second;
	double radius = 2;

	std::cout << "x = " << x << "   y = " << y << std::endl;
	circle(image, cv::Point(x, y), radius, cv::Scalar(50, 255, 0), CV_FILLED, 5, 0);
	}
	cv::Mat circles = image.clone();
	cv::imshow("circles", circles);
	
	cv::waitKey(0);
	return(0);
}
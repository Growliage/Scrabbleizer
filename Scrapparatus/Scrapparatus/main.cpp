#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

cv::Mat histogramequalization(Mat image);
std::vector<pair<int, int>>boardDetection(cv::Mat image);

int main(int, char)
{
	cout << " Place the board so that it is withind the border shown on the screen. After this is done press anykey to continue " << endl;
	VideoCapture Livecap(0); // open the default camera
	if (!Livecap.isOpened()) // check if we succeeded
		return -1;
	for (;;)
	{
		Mat frame;
		Livecap >> frame; // get a new frame from camera
		imshow("image", frame);
		if (waitKey(30) >= 0)
			break;
	}

	Mat startingImage;
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened()) // check if we succeeded
		return -1;
	cap >> startingImage;
	
	cvtColor(startingImage, startingImage, CV_BGR2GRAY);
	Mat equalizedImage = histogramequalization(startingImage);
	std::vector< std::pair<int, int>> scrabbleBoard;

	while (scrabbleBoard.size() != 4){
			scrabbleBoard = boardDetection(equalizedImage);
	}



	imshow("image", scrabbleBoard);
	waitKey(0);
	return(0);
}
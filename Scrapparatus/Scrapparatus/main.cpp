#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui.hpp>"

using namespace cv;
using namespace std;

cv::Mat histogramequalization(Mat image);
std::vector<pair<int, int>>boardDetection(cv::Mat image);
cv::Mat BackgroundSubtract(Mat firstFrame, Mat startingImage);

int main(int, char)
{
		Mat frame, firstFrame, BackgSubs, startingImage;
		VideoCapture capture(1);
		if (!capture.isOpened())
			return -1;

		capture >> firstFrame;
		waitKey(0);
		cout << " Place the board so that it is withind the border shown on the screen. After this is done press anykey twice to continue " << endl;

		for (;;)
		{
			capture >> frame; // get a new frame from camera
			imshow("image", frame);
			if (waitKey(30) >= 0)
				break;
		}

		cvtColor(firstFrame, firstFrame, CV_BGR2GRAY);


		Mat equalizedImage1 = histogramequalization(firstFrame);
		Mat out(equalizedImage1.rows, equalizedImage1.cols, equalizedImage1.type());
		waitKey(0);

		capture >> startingImage;
		cvtColor(startingImage, startingImage, CV_BGR2GRAY);
		Mat equalizedImage2 = histogramequalization(startingImage);

		//waitKey(0);
		/*for (int i = 0; i <= 20; i++){
			capture >> startingImage;

			cvtColor(startingImage, startingImage, CV_BGR2GRAY);
			Mat equalizedImage2 = histogramequalization(startingImage);

			Mat temp = equalizedImage2;
			out = temp + out;

			}
			/*Mat noiseR(out.rows, out.cols, out.type());
			for (int j = 0; j < out.rows; j++)
			for (int k = 0; k < out.cols; k++){
			noiseR.at<unsigned char>(j, k) = out.at<unsigned char>(j, k) / 21;
			}*/

		//Mat noiseR = out;
		//background subtraction here/*
		Mat backGroundSubtraction = BackgroundSubtract(equalizedImage1, equalizedImage2);

		imshow("image", backGroundSubtraction);
		cout << "subtraction" << endl;
		waitKey(0);
		std::vector< std::pair<int, int>> scrabbleBoard;

		while (scrabbleBoard.size() != 4){
			scrabbleBoard = boardDetection(out);
		}

		imshow("image", scrabbleBoard);
		waitKey(0);
		return(0);
}
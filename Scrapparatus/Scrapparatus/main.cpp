#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>


std::vector<std::pair<int, int>>boardDetection(cv::Mat image);
cv::Mat BackgroundSubtract(cv::Mat firstFrame, cv::Mat startingImage, int threshold);
double homography(int,int,int,int,int,int,int,int, cv::Point topLeft, cv::Point bottomRight);
cv::Point2d homog(int x, int y); // call to find corrospoding points in perfect image
int changeTurn(int players, int x1, int y1,int x4,int y4);

int main(int, char)
{
	int players = 0;
	int pfbx4;
	int pfby4;
	int x1, y1, x2, y2, x3, y3, x4, y4;
	
	
	bool runOnce = true;
	do{
		cv::Mat frame, firstFrame, BackgSubs, startingImage;
		cv::VideoCapture capture(0);
		if (!capture.isOpened())
			return -1;

		capture >> firstFrame;
		capture >> firstFrame;

		cv::waitKey(0);
		std::cout << " Place the board grid so that it is withind the border shown on the screen, the word Scrabble should be at the buttom of the screen. After this is done press anykey three times to continue " << std::endl;
		for (;;)
		{
			capture >> frame; // get a new frame from camera
			//rectangle(frame, cvPoint(120, 30), cvPoint(frame.cols - 100, frame.rows - 20), CV_RGB(50, 255, 0), 2);
			rectangle(frame, cvPoint(150, 50), cvPoint(520, 420), CV_RGB(0, 20, 255), 2);
			imshow("image", frame);
			if (cv::waitKey(30) >= 0)
				break;
		}

		cvtColor(firstFrame, firstFrame, CV_BGR2GRAY);


		capture >> startingImage;
		capture >> startingImage;
		std::cout << startingImage.rows << "   " << startingImage.cols << std::endl;
		cvtColor(startingImage, startingImage, CV_BGR2GRAY);

		cv::Mat backGroundSubtraction = BackgroundSubtract(firstFrame, startingImage, 30);

		for (int i = 0; i < backGroundSubtraction.rows; i++){ //y
			for (int j = 0; j < backGroundSubtraction.cols; j++){ //x
				if (j < 100 || j > backGroundSubtraction.cols - 80 || i < 20 || i > backGroundSubtraction.rows - 10)
					backGroundSubtraction.at<unsigned char>(i, j) = 0;
				if (j > 160 && j < backGroundSubtraction.cols - 145){
					if (i > 70 && i < backGroundSubtraction.rows - 80){
						backGroundSubtraction.at<unsigned char>(i, j) = 0;
					}
				}
			}
		}
		
		GaussianBlur(backGroundSubtraction, backGroundSubtraction, cv::Size(7, 7), 1.5, 1.5);
		Canny(backGroundSubtraction, backGroundSubtraction, 15, 30, 3);
		imshow("bgs", backGroundSubtraction);
		cv::waitKey(0);
		std::vector< std::pair<int, int>> scrabbleBoard;

		while (scrabbleBoard.size() != 4){
			scrabbleBoard = boardDetection(backGroundSubtraction);
		}

		for (int i = 0; i < scrabbleBoard.size(); i++){
			int x = scrabbleBoard[i].first;
			int y = scrabbleBoard[i].second;

			cv::circle(startingImage, cv::Point(x, y),4,cv::Scalar(0,25,255),2);
			std::cout << "x = " << x << " y = " << y << std::endl;
		}

		x1 = scrabbleBoard[0].first;
		y1 = scrabbleBoard[0].second;
		x2 = scrabbleBoard[1].first;
		y2 = scrabbleBoard[1].second;
		x3 = scrabbleBoard[2].first;
		y3 = scrabbleBoard[2].second;
		x4 = scrabbleBoard[3].first;
		y4 = scrabbleBoard[3].second;


		int pfbx1 = 120;
		int pfby1 = 30;

		pfbx4 = 520;
	pfby4 = 420;
		
		//homography(x1, y1, x2, y2, x3, y3, x4, y4, cv::Point(120, 30), cv::Point(frame.cols - 100, frame.rows - 20));
		homography(x1, y1, x2, y2, x3, y3, x4, y4, cv::Point(150, 50), cv::Point(pfbx4, pfby4));


		runOnce = false;
	} while(runOnce == true);

		cv::waitKey(0);
		
		std::cout << "Choose number of players (1-4)\n";
		std::cin >> players;
		(changeTurn(players, 150, 50, pfbx4, pfby4)); // used to draw grid based on the perfect board.
		//(players, 150, 50, x4, y4)); // used to draw the grind based in the board detection.
		//in changeTurn.pfbx4
				//letter placement
				//letter recognition
				//contest word
				//SOWPODS
		return(0);
}
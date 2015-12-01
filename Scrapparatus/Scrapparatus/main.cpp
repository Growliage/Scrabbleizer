#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>


cv::Mat histogramequalization(cv::Mat image);
std::vector<std::pair<int, int>>boardDetection(cv::Mat image);
cv::Mat BackgroundSubtract(cv::Mat firstFrame, cv::Mat startingImage);

int main(int, char)
{

	//Load dictionary!
	std::cout << "Loading dictionary... This may take a while";

	//std::string fileName = "data/2.txt";
	std::ifstream dict("fileName");


	//Moving the filestream into a vector
	dict.open("data/2.txt");
	std::vector<std::string> lines2;
	for (std::string line; std::getline(dict, line);)
		lines2.push_back(line);
	dict.close();


	dict.open("data/3.txt");
	std::vector<std::string> lines3;
	for (std::string line; std::getline(dict, line);)
		lines3.push_back(line);
	dict.close();


	dict.open("data/4.txt");
	std::vector<std::string> lines4;
	for (std::string line; std::getline(dict, line);)
		lines4.push_back(line);
	dict.close();


	dict.open("data/5.txt");
	std::vector<std::string> lines5;
	for (std::string line; std::getline(dict, line);)
		lines5.push_back(line);
	dict.close();


	dict.open("data/6.txt");
	std::vector<std::string> lines6;
	for (std::string line; std::getline(dict, line);)
		lines6.push_back(line);
	dict.close();


	dict.open("data/7.txt");
	std::vector<std::string> lines7;
	for (std::string line; std::getline(dict, line);)
		lines7.push_back(line);
	dict.close();


	dict.open("data/8.txt");
	std::vector<std::string> lines8;
	for (std::string line; std::getline(dict, line);)
		lines8.push_back(line);
	dict.close();
	

	dict.open("data/9.txt");
	std::vector<std::string> lines9;
	for (std::string line; std::getline(dict, line);)
		lines9.push_back(line);
	dict.close();
	

	dict.open("data/10.txt");
	std::vector<std::string> lines10;
	for (std::string line; std::getline(dict, line);)
		lines10.push_back(line);
	dict.close();


	dict.open("data/11.txt");
	std::vector<std::string> lines11;
	for (std::string line; std::getline(dict, line);)
		lines11.push_back(line);
	dict.close();


	dict.open("data/12.txt");
	std::vector<std::string> lines12;
	for (std::string line; std::getline(dict, line);)
		lines12.push_back(line);
	dict.close();


	dict.open("data/13.txt");
	std::vector<std::string> lines13;
	for (std::string line; std::getline(dict, line);)
		lines13.push_back(line);
	dict.close();


	dict.open("data/14.txt");
	std::vector<std::string> lines14;
	for (std::string line; std::getline(dict, line);)
		lines14.push_back(line);
	dict.close();


	dict.open("data/15.txt");
	std::vector<std::string> lines15;
	for (std::string line; std::getline(dict, line);)
		lines15.push_back(line);
	dict.close();


	std::cout << "\nDictionary has been loaded." << std::endl;

		cv::Mat frame, firstFrame, BackgSubs, startingImage;
		cv::VideoCapture capture(1);
		if (!capture.isOpened())
			return -1;

		capture >> firstFrame;
		cv::waitKey(0);
		std::cout << " Place the board so that it is withind the border shown on the screen, the word Scrabble should be at the buttom of the green square. After this is done press anykey three times to continue " << std::endl;
		for (;;)
		{
			capture >> frame; // get a new frame from camera
			rectangle(frame, cvPoint(120,30), cvPoint(frame.cols-100,frame.rows-20), CV_RGB(50,255,0), 2);
			imshow("image", frame);
			if (cv::waitKey(30) >= 0)
				break;
		}

		cvtColor(firstFrame, firstFrame, CV_BGR2GRAY);

		cv::Mat equalizedImage1 = histogramequalization(firstFrame);

		capture >> startingImage;
		cvtColor(startingImage, startingImage, CV_BGR2GRAY);
		cv::Mat equalizedImage2 = histogramequalization(startingImage);

		cv::Mat backGroundSubtraction = BackgroundSubtract(equalizedImage1, equalizedImage2);
		
		for (int i = 0; i < backGroundSubtraction.rows; i++){ //y
			for (int j = 0; j < backGroundSubtraction.cols; j++){ //x
				if (j < 100 || j > backGroundSubtraction.cols - 80 || i < 20 || i > backGroundSubtraction.rows - 10)
					backGroundSubtraction.at<unsigned char>(i, j) = 0;
				if (j > 130 && j < backGroundSubtraction.cols - 115){
					if (i > 50 && i < backGroundSubtraction.rows - 40){
						backGroundSubtraction.at<unsigned char>(i, j) = 0;
					}
				}
			}
		}
		imshow("image", backGroundSubtraction);

		

		cv::waitKey(0);
		std::vector< std::pair<int, int>> scrabbleBoard;

		while (scrabbleBoard.size() != 4){
			scrabbleBoard = boardDetection(backGroundSubtraction);
		}

		for (int i = 0; i < scrabbleBoard.size(); i++){
			int x = scrabbleBoard[i].first;
			int y = scrabbleBoard[i].second;

			std::cout << "x = " << x << " y = " << y << std::endl;
		}
		cv::waitKey(0);
		return(0);
}
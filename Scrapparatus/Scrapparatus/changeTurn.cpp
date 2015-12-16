#include "opencv2/opencv.hpp"


//using namespace cv;
//using namespace std;

cv::Mat histogramequalization(cv::Mat image);
cv::Mat BackgroundSubtract(cv::Mat firstFrame, cv::Mat startingImage, int threshold);
cv::Mat sumFilter(cv::Mat inImage, cv::Mat outImage, int threshold);
cv::Mat ColourThres(cv::Mat inImage);
int homogX(int x, int y);
int homogY(int x, int y);
int VSBoard(cv::Mat image, cv::Mat imageSubtracted, int x1, int y1, int x4, int y4);
/*(VSBoard)NOTE: Index 0 = points awarded, 1 = skippedTurn(bool), 2 = contested(bool), 3 = contesting player(1 - 4), 4 = contest successful(bool)*/

int changeTurn(int players, int x1, int y1,int x4, int y4){
	cv::Mat frame, beforeTurn, afterTurn, temp;

	bool endGame = false; //A whole bunch of declarations. endGame is used to check if the game is ended or not
	cv::Mat points = (cv::Mat_<int>(4, 1) << 0, 0, 0, 0);	//Matrix used to hold the amount of points each player has and receives
	int nmbOfPlayers = players;
	int turn = 0; //Used for checking whose turn it is
	double key = 0; //Used to check what key has been pressed by the user
	int Threshold = 30;
	int sumThreshold = 1020;
	int addPoints;
	std::cout << nmbOfPlayers << " playing.\n";
	cv::waitKey(0); //to make sure that p1 has played a word before it starts
	while (endGame == false){
		cv::VideoCapture capture(0);
		if (!capture.isOpened())
			return -1;

		//std::thread second(VSBoard(image, imageSubtracted)); //Pass a regular and an image subtracted image

		if (turn == 0){ //Checking for whose turn it is
			std::cout << "P1's turn.\n"; //Shows whose turn it is
			capture >> beforeTurn;
			capture >> beforeTurn;

			cv::waitKey(0);

			capture >> afterTurn;
			capture >> afterTurn;
			imshow(" before", beforeTurn);
			std::cout << "capture after" << std::endl;

			beforeTurn = ColourThres(beforeTurn);

			cvtColor(beforeTurn, beforeTurn, CV_BGR2GRAY);
			cvtColor(afterTurn, afterTurn, CV_BGR2GRAY);

			temp = BackgroundSubtract(beforeTurn, afterTurn, Threshold);
			cv::Mat outImage = temp.clone();
			cv::Mat grayAfterImage = afterTurn.clone();
			sumFilter(temp, outImage, sumThreshold);
			
		/*	cv::Mat homogImage = temp.clone();
			for (int i = 15; i < outImage.rows-15; i++){
				for (int j = 50; j < outImage.cols-50; j++){
					int y = homogY(j, i);
					int x = homogX(j, i);
					homogImage.at<unsigned char>(y, x) =  outImage.at<unsigned char>(i,j);
					grayAfterImage.at<unsigned char>(y, x) = afterTurn.at<unsigned char>(i, j);
				}
			}*/
			
			imshow("letters", outImage);
			addPoints = VSBoard(afterTurn/*afterTurn*/, outImage/*outImage*/, x1, y1, x4, y4);

			key = cv::waitKey(0);
			if (key == 108){
				std::cout << "Word check!\n";
				/*if (SOWPODSsearch(string) == 0){
				cout << "Word does not exist, no points for you.";
				}
				else {
				cout << "Word does exist.";
				}*/
			}
			points.at<int>(0) = points.at<int>(0) + addPoints; //add the points
			std::cout << "P1 receives " << addPoints << " points.\n"; //Shows how many points the player is given
		}
		else if (turn == 1){
			std::cout << "P2's turn.\n";
			capture >> beforeTurn;
			capture >> beforeTurn;

			cv::waitKey(0);

			capture >> afterTurn;
			capture >> afterTurn;
			imshow(" before", beforeTurn);
			std::cout << "capture after" << std::endl;

			beforeTurn = ColourThres(beforeTurn);

			cvtColor(beforeTurn, beforeTurn, CV_BGR2GRAY);
			cvtColor(afterTurn, afterTurn, CV_BGR2GRAY);

			temp = BackgroundSubtract(beforeTurn, afterTurn, Threshold);
			cv::Mat outImage = temp.clone();
			cv::Mat grayAfterImage = afterTurn.clone();
			sumFilter(temp, outImage, sumThreshold);

			/*	cv::Mat homogImage = temp.clone();
			for (int i = 15; i < outImage.rows-15; i++){
			for (int j = 50; j < outImage.cols-50; j++){
			int y = homogY(j, i);
			int x = homogX(j, i);
			homogImage.at<unsigned char>(y, x) =  outImage.at<unsigned char>(i,j);
			grayAfterImage.at<unsigned char>(y, x) = afterTurn.at<unsigned char>(i, j);
			}
			}*/

			imshow("letters", outImage);
			addPoints = VSBoard(afterTurn/*afterTurn*/, outImage/*outImage*/, x1, y1, x4, y4);

			key = cv::waitKey(0);
			if (key == 108){
				std::cout << "Word check!\n";
				/*if (SOWPODSsearch(string) == 0){
				cout << "Word does not exist, no points for you.";
				}
				else {
				cout << "Word does exist.";
				}*/
			}
			points.at<int>(1) = points.at<int>(1) + addPoints;
			std::cout << "P2 receives " << addPoints << " points.\n";
		}
		else if (turn == 2){
			std::cout << "P3's turn.\n";
			capture >> beforeTurn;
			capture >> beforeTurn;

			cv::waitKey(0);

			capture >> afterTurn;
			capture >> afterTurn;
			imshow(" before", beforeTurn);
			std::cout << "capture after" << std::endl;
			beforeTurn = ColourThres(beforeTurn);
			cvtColor(beforeTurn, beforeTurn, CV_BGR2GRAY);
			cvtColor(afterTurn, afterTurn, CV_BGR2GRAY);

			temp = BackgroundSubtract(beforeTurn, afterTurn, Threshold);
			cv::Mat outImage = temp.clone();
			sumFilter(temp, outImage, sumThreshold);
			addPoints = VSBoard(afterTurn, outImage, x1, y1, x4, y4);

			imshow("sumThreshold", outImage);

			key = cv::waitKey(0);
			if (key == 108){
				std::cout << "Word check!\n";
				/*if (SOWPODSsearch(string) == 0){
				cout << "Word does not exist, no points for you.";
				}
				else {
				cout << "Word does exist.";
				}*/
			}
			points.at<int>(2) = points.at<int>(2) + addPoints;
			std::cout << "P3 receives " << addPoints << " points.\n";
		}
		else {
			std::cout << "P4's turn.\n";
			capture >> beforeTurn;
			capture >> beforeTurn;

			cv::waitKey(0);

			capture >> afterTurn;
			capture >> afterTurn;
			imshow(" before", beforeTurn);
			std::cout << "capture after" << std::endl;
			beforeTurn = ColourThres(beforeTurn);
			cvtColor(beforeTurn, beforeTurn, CV_BGR2GRAY);
			cvtColor(afterTurn, afterTurn, CV_BGR2GRAY);

			temp = BackgroundSubtract(beforeTurn, afterTurn, Threshold);
			cv::Mat outImage = temp.clone();
			sumFilter(temp, outImage, sumThreshold);
			addPoints = VSBoard(afterTurn, outImage, x1, y1, x4, y4);

			imshow("letters", temp);
			imshow("sumThreshold", outImage);
			imshow("afterTurn", afterTurn);

			key = cv::waitKey(0);
			if (key == 108){
				std::cout << "Word check!\n";
				/*if (SOWPODSsearch(string) == 0){
				cout << "Word does not exist, no points for you.";
				}
				else {
				cout << "Word does exist.";
				}*/
			}
			points.at<int>(3) = points.at<int>(3) + addPoints;
			std::cout << "P4 receives " << addPoints << " points.\n";
		}
		turn++; //Count the turn up to signify it's a new player's turn
		if (turn > nmbOfPlayers - 1){ //There can only be four players so it should reset if it goes above that
			turn = 0;
		}
		key = cv::waitKey(0); //q = 113
		if (key == 113){ //check to see if q has been pressed in between the turns. If yes, it breaks the loop. Does NOT work while it is a players turn
			std::cout << "Game ended!\n";
			endGame = true;
		}

	}
	int count = 0, highest = 0;
	for (int i = 0; i < nmbOfPlayers; i++){//This prints the standings of the players. It is is a for loop to make sure it only prints the 
		std::cout << "P" << count + 1 << " has " << points.at<int>(count) << " points.\n";  //players that have actually played
		if (highest < points.at<int>(count)){
			highest = points.at<int>(count);
		}
		count++;
	}

	return highest;
}
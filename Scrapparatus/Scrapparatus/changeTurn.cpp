#include "opencv2/opencv.hpp"


using namespace cv;
using namespace std;

int changeTurn(int players){
	bool endGame = false; //A whole bunch of declarations. endGame is used to check if the game is ended or not
	Mat points = (Mat_<int>(4, 1) << 0, 0, 0, 0);	//Matrix used to hold the amount of points each player has and receives
	int nmbOfPlayers = players;
	int turn = 0; //Used for checking whose turn it is
	double key = 0; //Used to check what key has been pressed by the user
	
	cv::Mat frame, firstFrame, BackgSubs, startingImage;
	cv::VideoCapture capture(1);
	if (!capture.isOpened())
		return -1;

	for (;;)
	{
		capture >> frame; // get a new frame from camera
		rectangle(frame, cvPoint(120, 30), cvPoint(frame.cols - 100, frame.rows - 20), CV_RGB(50, 255, 0), 2);
		imshow("image", frame);
		if (cv::waitKey(30) >= 0)
			break;
	}

	cout << nmbOfPlayers << " playing.\n";
	waitKey(0); //to make sure that p1 has played a word before it starts
	while (endGame == false){
		int counter = rand() % 100; //This is is just a placeholder for the point counting class/program
		if (turn == 0){ //Checking for whose turn it is
			cout << "P1's turn.\n"; //Shows whose turn it is
			key = waitKey(0); //Wait for the player to put in a word
			if (key == 108){
				cout << "Word check!\n";
				/*if (SOWPODSsearch(string) == 0){
				cout << "Word does not exist, no points for you.";
				}
				else {
				cout << "Word does exist.";
				}*/
			}
			points.at<int>(0) = points.at<int>(0) + counter; //add the points
			cout << "P1 receives " << counter << " points.\n"; //Shows how many points the player is given
		}
		else if (turn == 1){
			cout << "P2's turn.\n";
			key = waitKey(0);
			if (key == 108){
				cout << "Word check!\n";
				/*if (SOWPODSsearch(string) == 0){
				cout << "Word does not exist, no points for you.";
				}
				else {
				cout << "Word does exist.";
				}*/
			}
			points.at<int>(1) = points.at<int>(1) + counter;
			cout << "P2 receives " << counter << " points.\n";
		}
		else if (turn == 2){
			cout << "P3's turn.\n";
			key = waitKey(0);
			if (key == 108){
				cout << "Word check!\n";
				/*if (SOWPODSsearch(string) == 0){
				cout << "Word does not exist, no points for you.";
				}
				else {
				cout << "Word does exist.";
				}*/
			}
			points.at<int>(2) = points.at<int>(2) + counter;
			cout << "P3 receives " << counter << " points.\n";
		}
		else {
			cout << "P4's turn.\n";
			key = waitKey(0);
			if (key == 108){
				cout << "Word check!\n";
				/*if (SOWPODSsearch(string) == 0){
				cout << "Word does not exist, no points for you.";
				}
				else {
				cout << "Word does exist.";
				}*/
			}
			points.at<int>(3) = points.at<int>(3) + counter;
			cout << "P4 receives " << counter << " points.\n";
		}
		turn++; //Count the turn up to signify it's a new player's turn
		if (turn > nmbOfPlayers - 1){ //There can only be four players so it should reset if it goes above that
			turn = 0;
		}
		key = waitKey(0); //q = 113
		if (key == 113){ //check to see if q has been pressed in between the turns. If yes, it breaks the loop. Does NOT work while it is a players turn
			cout << "Game ended!\n";
			endGame = true;
		}

	}
	int count = 0, highest = 0;
	for (int i = 0; i < nmbOfPlayers; i++){//This prints the standings of the players. It is is a for loop to make sure it only prints the 
		cout << "P" << count + 1 << " has " << points.at<int>(count) << " points.\n";  //players that have actually played
		if (highest < points.at<int>(count)){
			highest = points.at<int>(count);
		}
		count++;
	}

	return highest;
}
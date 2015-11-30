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
		cout << " Place the board so that it is withind the border shown on the screen, the word Scrabble should be at the buttom of the green square. After this is done press anykey twice to continue " << endl;
		for (;;)
		{
			capture >> frame; // get a new frame from camera
			rectangle(frame, cvPoint(120,30), cvPoint(frame.cols-100,frame.rows-20), CV_RGB(50,255,0), 2);
			imshow("image", frame);
			if (waitKey(30) >= 0)
				break;
		}

		cvtColor(firstFrame, firstFrame, CV_BGR2GRAY);

		Mat equalizedImage1 = histogramequalization(firstFrame);

		capture >> startingImage;
		cvtColor(startingImage, startingImage, CV_BGR2GRAY);
		Mat equalizedImage2 = histogramequalization(startingImage);

		Mat backGroundSubtraction = BackgroundSubtract(equalizedImage1, equalizedImage2);
		
		/*for (int i = 0; i < backGroundSubtraction.rows; i++){
			for (int j = 0; j < backGroundSubtraction.cols; j++){
				if (j < 100 || j > backGroundSubtraction.cols - 80 || i < 20 || i > backGroundSubtraction.rows-10)
					backGroundSubtraction.at<unsigned char>(i, j) = 0;
			}
		}*/

		imshow("image", backGroundSubtraction);

		

		waitKey(0); 
		std::vector< std::pair<int, int>> scrabbleBoard;

		while (scrabbleBoard.size() != 4){
			scrabbleBoard = boardDetection(backGroundSubtraction);
		}

		for (int i = 0; i < scrabbleBoard.size(); i++){
			int x = scrabbleBoard[i].first;
			int y = scrabbleBoard[i].second;

			cout << "x = " << x << " y = " << y << endl;
		}
		waitKey(0);
		return(0);
}
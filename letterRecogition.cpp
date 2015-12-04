#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>

//all the functions in the program
cv::Mat resizeImage(cv::Mat image);
cv::Mat divideImage(cv::Mat image, int bx, int by, int h, int w);
int compareHistograms(cv::Mat image, int w);
void boundingBox(cv::Mat image);
bool compareRectx(const cv::Rect &a, const cv::Rect &b);
bool compareRecty(const cv::Rect &a, const cv::Rect &b);

int main(int, char){
	cv::Mat letter = cv::imread("C:/LetterRecognition/word.jpg", 0);
	//***************************Threshold & scale image*********************
	for (int y = 0; y < letter.rows; y++){
		for (int x = 0; x < letter.cols; x++){
			if (letter.at<unsigned char>(y, x) >= 170){
				letter.at<unsigned char>(y, x) = 255;
			}
			else{
				letter.at<unsigned char>(y, x) = 0;
			}
		}
	}
	imshow("input", letter);
	//scale image so we are always working with the same size
	double scale;
	//calculate the scale when it is scaled down to 128 pixels in height
	
	if (letter.cols < letter.rows){
		scale = 128.0 / double(letter.cols);
	}
	else{
		scale = 128.0 / double(letter.rows);
	}
	cv::Mat imgResize = cv::Mat::zeros(letter.rows * scale, letter.cols * scale, letter.type());
	for (int outY = 0; outY < imgResize.rows; outY++){
		for (int outX = 0; outX < imgResize.cols; outX++){
			imgResize.at<uchar>(outY, outX) = letter.at<uchar>((1 / scale) * outY, (1 / scale) * outX);
		}
	}
	letter = imgResize.clone();
	for (int y = 0; y < letter.rows; y++){
		for (int x = 0; x < letter.cols; x++){
			if (letter.at<unsigned char>(y, x) == 0){
				letter.at<unsigned char>(y, x) = 255;
			}
			else{
				letter.at<unsigned char>(y, x) = 0;
			}
		}
	}
	boundingBox(letter);
	cv::waitKey(0);
	return 0;
}
cv::Mat resizeImage(cv::Mat image)
{
	double scalex;
	double scaley;
	scalex = 32 / double(image.cols);
	scaley = 32 / double(image.rows);
	cv::Mat imgResize = cv::Mat::zeros(32, 32, image.type());
	for (int outY = 0; outY < imgResize.rows; outY++){
		for (int outX = 0; outX < imgResize.cols; outX++){
			imgResize.at<unsigned char>(outY, outX) = image.at<unsigned char>((1 / scaley) * outY, (1 / scalex) * outX);
		}
	}
	for (int y = 0; y < image.rows; y++){
		for (int x = 0; x < image.cols; x++){
			if (image.at<unsigned char>(y, x) >= 170){
				image.at<unsigned char>(y, x) = 255;
			}
			else{
				image.at<unsigned char>(y, x) = 0;
			}
		}
	}
	return imgResize;
}
int compareHistograms(cv::Mat image, int w){
	//*****************************Declare Histograms***********************
	int cordsA[65] = { 3, 6, 8, 11, 13, 16, 17, 18, 18, 18, 20, 23, 24, 21, 19, 16, 17, 20, 22, 25, 22, 20, 18, 17, 18, 18, 15, 13, 10, 8, 6, 3, 8, 8, 9, 10, 10, 12, 12, 14, 14, 14, 14, 15, 15, 14, 15, 14, 15, 15, 22, 24, 24, 25, 26, 26, 28, 14, 16, 14, 14, 15, 14, 13, 69 };
	int cordsB[65] = { 32, 32, 32, 32, 32, 32, 32, 32, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 21, 21, 22, 25, 28, 28, 30, 30, 29, 26, 23, 17, 11, 8, 23, 26, 28, 28, 29, 29, 20, 17, 18, 16, 17, 17, 19, 28, 27, 27, 29, 30, 31, 18, 17, 16, 16, 17, 18, 32, 32, 31, 30, 29, 27, 23, 55 };
	int cordsC[65] = { 10, 14, 18, 20, 22, 24, 25, 26, 22, 18, 18, 15, 14, 16, 14, 14, 14, 13, 13, 13, 14, 14, 14, 14, 14, 16, 16, 16, 14, 10, 7, 5, 10, 17, 21, 24, 27, 28, 28, 19, 14, 11, 9, 9, 9, 8, 8, 8, 8, 8, 8, 9, 9, 9, 11, 13, 18, 26, 28, 27, 24, 21, 17, 11, 60 };
	int cordsD[65] = { 32, 32, 32, 32, 32, 32, 32, 32, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 16, 18, 18, 24, 26, 25, 23, 22, 20, 18, 14, 8, 19, 22, 24, 26, 27, 28, 29, 19, 18, 18, 17, 16, 17, 17, 16, 16, 16, 16, 17, 17, 16, 17, 18, 18, 20, 29, 28, 26, 25, 24, 22, 18, 61 };
	int cordsE[65] = { 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 12, 12, 12, 32, 32, 32, 32, 32, 32, 10, 10, 10, 10, 10, 10, 10, 29, 29, 29, 29, 29, 29, 10, 10, 10, 10, 10, 10, 10, 32, 32, 32, 32, 32, 32, 50 };
	int cordsF[65] = { 32, 32, 32, 32, 32, 32, 32, 32, 32, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 7, 7, 7, 32, 32, 32, 32, 32, 32, 32, 9, 9, 9, 9, 9, 9, 29, 29, 29, 29, 29, 29, 29, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 50 };
	int cordsG[65] = { 11, 15, 18, 21, 23, 25, 26, 27, 21, 19, 16, 16, 16, 14, 14, 12, 12, 18, 18, 18, 19, 20, 20, 21, 20, 25, 25, 23, 21, 18, 15, 14, 12, 18, 21, 24, 25, 26, 21, 16, 13, 9, 9, 9, 9, 8, 23, 23, 23, 23, 23, 23, 16, 16, 16, 17, 19, 23, 29, 28, 26, 22, 18, 13, 59 };
	int cordsH[65] = { 32, 32, 32, 32, 32, 32, 32, 32, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 32, 32, 32, 32, 32, 32, 32, 32, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 32, 32, 32, 32, 32, 32, 32, 32, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 55 };
	int cordsI[65] = { 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 44 };
	int cordsJ[65] = { 3, 4, 5, 7, 7, 8, 8, 8, 7, 8, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 31, 31, 31, 31, 30, 30, 29, 29, 28, 27, 25, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 17, 22, 30, 31, 30, 29, 24, 20, 11, 58 };
	int cordsK[65] = { 32, 32, 32, 32, 32, 32, 32, 32, 9, 9, 9, 9, 9, 11, 14, 16, 18, 20, 22, 21, 21, 21, 21, 19, 19, 16, 14, 12, 10, 7, 5, 3, 18, 18, 18, 18, 18, 18, 18, 18, 17, 17, 17, 18, 18, 18, 19, 19, 20, 21, 22, 22, 19, 20, 18, 18, 18, 18, 19, 18, 18, 18, 18, 17, 47 };
	int cordsL[65] = { 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 32, 32, 32, 32, 32, 32, 32, 66 };
	int cordsM[65] = { 32, 32, 32, 32, 32, 32, 32, 12, 13, 13, 12, 12, 12, 12, 12, 12, 11, 12, 12, 12, 12, 13, 12, 13, 12, 32, 32, 32, 32, 32, 32, 32, 16, 18, 19, 20, 22, 22, 24, 25, 26, 28, 28, 30, 29, 30, 28, 26, 25, 24, 22, 21, 20, 18, 18, 16, 14, 14, 14, 14, 14, 14, 14, 14, 58 };
	int cordsN[65] = { 32, 32, 32, 32, 32, 32, 32, 32, 12, 12, 12, 11, 11, 11, 12, 12, 11, 12, 11, 11, 12, 12, 12, 11, 31, 32, 32, 32, 32, 32, 32, 32, 16, 17, 18, 19, 19, 20, 21, 22, 23, 24, 24, 26, 26, 26, 26, 26, 26, 26, 25, 26, 25, 24, 23, 22, 21, 20, 20, 19, 18, 17, 16, 15, 68 };
	int cordsO[65] = { 12, 15, 18, 22, 24, 26, 26, 22, 20, 16, 14, 14, 14, 14, 12, 12, 12, 12, 14, 14, 15, 14, 16, 20, 23, 26, 26, 24, 22, 18, 16, 10, 10, 16, 18, 22, 24, 26, 22, 19, 17, 16, 17, 16, 16, 15, 14, 14, 14, 14, 14, 16, 16, 17, 16, 18, 18, 22, 26, 24, 22, 18, 16, 10, 51 };
	int cordsP[65] = { 32, 32, 32, 32, 32, 32, 32, 32, 32, 14, 13, 13, 13, 13, 13, 13, 13, 13, 13, 14, 15, 16, 18, 20, 20, 19, 18, 16, 16, 14, 11, 8, 23, 25, 27, 29, 30, 30, 31, 21, 19, 18, 18, 18, 18, 19, 20, 21, 31, 30, 29, 27, 26, 23, 10, 9, 9, 9, 9, 9, 9, 9, 9, 9, 70 };
	int cordsQ[65] = { 11, 16, 18, 22, 24, 26, 26, 22, 18, 16, 14, 16, 14, 14, 14, 14, 18, 20, 22, 23, 23, 21, 21, 20, 22, 27, 27, 28, 27, 24, 18, 12, 10, 14, 18, 21, 24, 26, 26, 20, 17, 16, 16, 16, 16, 14, 14, 14, 16, 19, 21, 24, 23, 24, 22, 21, 21, 27, 29, 28, 27, 24, 18, 12, 56 };
	int cordsR[65] = { 32, 32, 32, 32, 32, 32, 32, 32, 14, 14, 14, 14, 14, 14, 14, 14, 16, 17, 18, 19, 20, 23, 26, 29, 30, 29, 27, 24, 21, 18, 15, 11, 23, 26, 28, 29, 30, 31, 32, 18, 17, 17, 16, 16, 17, 18, 18, 31, 30, 29, 28, 27, 24, 25, 18, 19, 18, 18, 18, 18, 18, 19, 18, 17, 51 };
	int cordsS[65] = { 3, 12, 16, 20, 20, 23, 24, 23, 24, 24, 23, 22, 22, 19, 19, 19, 20, 19, 19, 20, 21, 22, 23, 25, 25, 24, 22, 21, 17, 15, 10, 8, 12, 20, 23, 27, 28, 28, 22, 16, 12, 10, 10, 12, 16, 21, 23, 25, 24, 23, 20, 16, 12, 10, 9, 13, 17, 23, 32, 30, 29, 25, 21, 15, 54 };
	int cordsT[65] = { 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 32, 32, 32, 32, 32, 32, 32, 32, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 32, 32, 32, 32, 32, 32, 32, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 57 };
	int cordsU[65] = { 22, 25, 27, 28, 29, 30, 30, 31, 13, 10, 8, 8, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 9, 10, 31, 30, 30, 29, 28, 27, 26, 24, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 17, 17, 17, 18, 18, 19, 21, 29, 28, 26, 24, 22, 18, 14, 66 };
	int cordsV[65] = { 4, 6, 9, 11, 13, 16, 18, 20, 21, 20, 20, 19, 19, 16, 15, 12, 11, 14, 17, 19, 19, 19, 20, 20, 20, 18, 15, 13, 11, 9, 6, 4, 16, 16, 17, 18, 16, 18, 16, 16, 18, 16, 18, 16, 16, 16, 16, 16, 16, 17, 16, 16, 15, 16, 16, 14, 14, 12, 12, 11, 10, 8, 8, 8, 98 };
	int cordsW[65] = { 5, 9, 14, 18, 22, 24, 24, 20, 14, 13, 17, 21, 21, 21, 19, 14, 14, 18, 23, 21, 22, 18, 14, 14, 19, 22, 23, 23, 19, 14, 10, 6, 15, 15, 16, 17, 18, 18, 18, 19, 19, 20, 20, 20, 21, 22, 19, 20, 20, 20, 21, 20, 20, 20, 19, 16, 16, 16, 15, 12, 12, 12, 12, 8, 60 };
	int cordsX[65] = { 4, 7, 9, 12, 15, 17, 20, 23, 26, 25, 25, 23, 21, 18, 16, 13, 12, 16, 18, 20, 24, 25, 24, 25, 22, 20, 17, 16, 12, 10, 6, 4, 18, 19, 19, 18, 19, 19, 18, 19, 18, 19, 18, 16, 15, 14, 12, 10, 11, 12, 14, 16, 17, 18, 18, 20, 20, 18, 18, 19, 19, 18, 18, 18, 60 };
	int cordsY[65] = { 2, 4, 5, 7, 8, 10, 11, 13, 13, 14, 13, 14, 25, 23, 22, 20, 20, 22, 23, 25, 13, 13, 13, 13, 13, 12, 10, 9, 7, 5, 4, 3, 16, 18, 18, 18, 18, 17, 18, 18, 17, 18, 18, 17, 18, 16, 14, 14, 12, 10, 10, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 63 };
	int cordsZ[65] = { 5, 14, 15, 16, 17, 18, 18, 19, 21, 22, 23, 24, 24, 23, 23, 23, 23, 23, 24, 23, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 31, 31, 31, 31, 31, 31, 30, 10, 11, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 10, 10, 11, 11, 11, 11, 11, 31, 32, 32, 32, 32, 32, 55 };
	//***************************Output Histogram****************************
	//find the histogram of the input image
	std::string letters[26] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z" };
	int cords[65] = { 0 };
	for (int y = 0; y < image.rows; y++){
		for (int x = 0; x < image.cols; x++){
			if (image.at<unsigned char>(y, x) != 0){
				cords[x] += 1;
				cords[y + 32] += 1;
				cords[64] = w;
			}
		}
	}
	int value = 0;
	int lettervalue = 800;
	int letternumber;
	//compare each histogram with the new
	for (int j = 0; j < 26; j++){
		for (int i = 0; i < 64; i++){
			if (j == 0)
				value += pow(cords[i] - cordsA[i], 2);
			else if (j == 1)
				value += pow(cords[i] - cordsB[i], 2);
			else if (j == 2)
				value += pow(cords[i] - cordsC[i], 2);
			else if (j == 3)
				value += pow(cords[i] - cordsD[i], 2);
			else if (j == 4)
				value += pow(cords[i] - cordsE[i], 2);
			else if (j == 5)
				value += pow(cords[i] - cordsF[i], 2);
			else if (j == 6)
				value += pow(cords[i] - cordsG[i], 2);
			else if (j == 7)
				value += pow(cords[i] - cordsH[i], 2);
			else if (j == 8)
				value += pow(cords[i] - cordsI[i], 2);
			else if (j == 9)
				value += pow(cords[i] - cordsJ[i], 2);
			else if (j == 10)
				value += pow(cords[i] - cordsK[i], 2);
			else if (j == 11)
				value += pow(cords[i] - cordsL[i], 2);
			else if (j == 12)
				value += pow(cords[i] - cordsM[i], 2);
			else if (j == 13)
				value += pow(cords[i] - cordsN[i], 2);
			else if (j == 14)
				value += pow(cords[i] - cordsO[i], 2);
			else if (j == 15)
				value += pow(cords[i] - cordsP[i], 2);
			else if (j == 16)
				value += pow(cords[i] - cordsQ[i], 2);
			else if (j == 17)
				value += pow(cords[i] - cordsR[i], 2);
			else if (j == 18)
				value += pow(cords[i] - cordsS[i], 2);
			else if (j == 19)
				value += pow(cords[i] - cordsT[i], 2);
			else if (j == 20)
				value += pow(cords[i] - cordsU[i], 2);
			else if (j == 21)
				value += pow(cords[i] - cordsV[i], 2);
			else if (j == 22)
				value += pow(cords[i] - cordsW[i], 2);
			else if (j == 23)
				value += pow(cords[i] - cordsX[i], 2);
			else if (j == 24)
				value += pow(cords[i] - cordsY[i], 2);
			else if (j == 25)
				value += pow(cords[i] - cordsZ[i], 2);
		}
		value = sqrt(value);
		if (lettervalue > value){//check if the new value is lower than the previous
			std::cout << std::endl <<letters[j] << " is now: " << value;
			lettervalue = value;//save this value as the new lowest
			letternumber = j;//save the j value as the letter number				
		}
		value = 0;
	}
	if (lettervalue > 50){
		imshow("ugly", image);
		std::cout << "ugly: " << lettervalue << std::endl;
		return 1337;
	}
	std::cout << "value: " << lettervalue << ", letter: " << letters[letternumber] << std::endl;
	return letternumber;
}
void boundingBox(cv::Mat image){
	std::string letters[27] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "*" };
	cv::Mat imgClone = image.clone();
	cv::Mat imgDivide;
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	int h;
	int w;
	int bx;
	int by;
	int letternumber;
	//imshow("halp", image);
	/// Find contours in the image, these are the outlines of each object in the image
	findContours(imgClone, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
	/// Approximate contours to polygons + get bounding rects
	std::vector<std::vector<cv::Point> > contoursPoly(contours.size());
	std::vector<cv::Rect> boundRect(contours.size());
	std::vector<int> word;
	std::vector<int> letterword;
	std::vector<int> gapword;
	//Create the bounding boxes around the contours and save it in the array
	for (int i = 0; i < contours.size(); i++){
		approxPolyDP(cv::Mat(contours[i]), contoursPoly[i], 3, true);
		boundRect[i] = boundingRect(cv::Mat(contoursPoly[i]));
	}
	if (image.cols < image.rows){
		std::sort(boundRect.begin(), boundRect.end(), compareRecty);
	}
	else{
		std::sort(boundRect.begin(), boundRect.end(), compareRectx);
	}
	for (int i = 0; i < contours.size(); i++){
		h = boundRect[i].height;
		w = boundRect[i].width;
		bx = boundRect[i].x;
		by = boundRect[i].y;
		if (400 < h*w && h*w < 4000 && w < 70 && 40 < h && h < 70){
			imgDivide = image.clone(); 
			imgDivide = divideImage(image, bx, by, h, w); 
			imgDivide = resizeImage(imgDivide);
			letternumber = compareHistograms(imgDivide, w);
			if (image.cols < image.rows){
				bx = by;
			}
			//make sure its an M or an N
			if (letternumber == 13 && w >= 55){
				letterword.push_back(12);
				gapword.push_back(bx);
			}
			else if (letternumber == 12 && w < 55){
				letterword.push_back(13);
				gapword.push_back(bx);
			}
			else if (letternumber != 1337){
				letterword.push_back(letternumber);
				gapword.push_back(bx);
			}
		}
	}

	int lastgap = image.cols;
	if (image.cols < image.rows){
		lastgap = image.rows;
	}
	if (gapword.size() != 0){
		if (gapword[0] > 120){
			word.push_back(26);
		}
		for (int i = 0; i < gapword.size(); i++){
			word.push_back(letterword[i]);
			if (i + 1 < gapword.size()){
				if (gapword[i + 1] - gapword[i] > 170){
					word.push_back(26);
				}
			}
		}
		if (lastgap - gapword[letterword.size() - 1] > 170){
			word.push_back(26);
		}
		for (int i = 0; i < word.size(); i++){
			std::cout << word[i];
			std::cout << letters[word[i]] << std::endl;
		}
	}

	/********************debugging************************/
	// Draw polygonal contour + bonding rects + circles for debugging	
	cv::Mat drawing = cv::Mat::zeros(image.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++){
		cv::Scalar color = cv::Scalar(255, 255, 255);
		drawContours(drawing, contoursPoly, i, color, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
		rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
	}
	// Show in a window
	cv::namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", drawing);
	//*/
}
bool compareRecty(const cv::Rect &a, const cv::Rect &b){
	return a.y < b.y;
}
bool compareRectx(const cv::Rect &a, const cv::Rect &b){
	return a.x < b.x;
}
cv::Mat divideImage(cv::Mat image, int bx, int by, int h, int w){
	cv::Mat imgDivide = cv::Mat::zeros(h, w, image.type());
	for (int y = 0; y < h; y++){
		for (int x = 0; x < w; x++){
			imgDivide.at<unsigned char>(y, x) = image.at<unsigned char>(by + y, bx + x);
		}
	}
	return imgDivide;
}

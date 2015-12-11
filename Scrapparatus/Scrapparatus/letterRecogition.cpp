#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>

//all the functions in the program
cv::Mat resizeImage(cv::Mat image);
cv::Mat removeNoise(cv::Mat image);
int compareHistograms(cv::Mat image);
std::string letterRecognition(cv::Mat image);
cv::Mat cropImage(cv::Mat image);

std::string letterRecognition(cv::Mat image){
	imshow("asd", image);
	std::string letters[27] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "*" };
	cv::Mat imgDivide = image.clone();
	imgDivide = removeNoise(image);
	imgDivide = resizeImage(imgDivide);
	int letterInt = compareHistograms(imgDivide);
	//make sure its an M or an N
	if (letterInt == 13 && image.cols >= 55){
		return "M";
	}
	else if (letterInt == 12 && image.cols < 55){
		return "N";
	}
	if (letterInt == 1337)
	{
		return "Error";
	}
	return letters[letterInt];
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
			if (image.at<unsigned char>(y, x) >= 240){
				image.at<unsigned char>(y, x) = 255;
			}
			else{
				image.at<unsigned char>(y, x) = 0;
			}
		}
	}
	return imgResize;
}
int compareHistograms(cv::Mat image){
	//*****************************Declare Histograms***********************
	int cordsBlank[64] = { 0 };
	int cordsA[64] = { 3, 6, 8, 11, 13, 16, 17, 18, 18, 18, 20, 23, 24, 21, 19, 16, 17, 20, 22, 25, 22, 20, 18, 17, 18, 18, 15, 13, 10, 8, 6, 3, 8, 8, 9, 10, 10, 12, 12, 14, 14, 14, 14, 15, 15, 14, 15, 14, 15, 15, 22, 24, 24, 25, 26, 26, 28, 14, 16, 14, 14, 15, 14, 13};
	int cordsB[64] = { 32, 32, 32, 32, 32, 32, 32, 32, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 21, 21, 22, 25, 28, 28, 30, 30, 29, 26, 23, 17, 11, 8, 23, 26, 28, 28, 29, 29, 20, 17, 18, 16, 17, 17, 19, 28, 27, 27, 29, 30, 31, 18, 17, 16, 16, 17, 18, 32, 32, 31, 30, 29, 27, 23};
	int cordsC[64] = { 10, 14, 18, 20, 22, 24, 25, 26, 22, 18, 18, 15, 14, 16, 14, 14, 14, 13, 13, 13, 14, 14, 14, 14, 14, 16, 16, 16, 14, 10, 7, 5, 10, 17, 21, 24, 27, 28, 28, 19, 14, 11, 9, 9, 9, 8, 8, 8, 8, 8, 8, 9, 9, 9, 11, 13, 18, 26, 28, 27, 24, 21, 17, 11};
	int cordsD[64] = { 32, 32, 32, 32, 32, 32, 32, 32, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 16, 18, 18, 24, 26, 25, 23, 22, 20, 18, 14, 8, 19, 22, 24, 26, 27, 28, 29, 19, 18, 18, 17, 16, 17, 17, 16, 16, 16, 16, 17, 17, 16, 17, 18, 18, 20, 29, 28, 26, 25, 24, 22, 18};
	int cordsE[64] = { 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 12, 12, 12, 32, 32, 32, 32, 32, 32, 10, 10, 10, 10, 10, 10, 10, 29, 29, 29, 29, 29, 29, 10, 10, 10, 10, 10, 10, 10, 32, 32, 32, 32, 32, 32};
	int cordsF[64] = { 32, 32, 32, 32, 32, 32, 32, 32, 32, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 7, 7, 7, 32, 32, 32, 32, 32, 32, 32, 9, 9, 9, 9, 9, 9, 29, 29, 29, 29, 29, 29, 29, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9};
	int cordsG[64] = { 11, 15, 18, 21, 23, 25, 26, 27, 21, 19, 16, 16, 16, 14, 14, 12, 12, 18, 18, 18, 19, 20, 20, 21, 20, 25, 25, 23, 21, 18, 15, 14, 12, 18, 21, 24, 25, 26, 21, 16, 13, 9, 9, 9, 9, 8, 23, 23, 23, 23, 23, 23, 16, 16, 16, 17, 19, 23, 29, 28, 26, 22, 18, 13};
	int cordsH[64] = { 32, 32, 32, 32, 32, 32, 32, 32, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 32, 32, 32, 32, 32, 32, 32, 32, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 32, 32, 32, 32, 32, 32, 32, 32, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16};
	int cordsI[64] = { 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32};
	int cordsJ[64] = { 3, 4, 5, 7, 7, 8, 8, 8, 7, 8, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 31, 31, 31, 31, 30, 30, 29, 29, 28, 27, 25, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 17, 22, 30, 31, 30, 29, 24, 20, 11};
	int cordsK[64] = { 32, 32, 32, 32, 32, 32, 32, 32, 9, 9, 9, 9, 9, 11, 14, 16, 18, 20, 22, 21, 21, 21, 21, 19, 19, 16, 14, 12, 10, 7, 5, 3, 18, 18, 18, 18, 18, 18, 18, 18, 17, 17, 17, 18, 18, 18, 19, 19, 20, 21, 22, 22, 19, 20, 18, 18, 18, 18, 19, 18, 18, 18, 18, 17};
	int cordsL[64] = { 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 32, 32, 32, 32, 32, 32, 32};
	int cordsM[64] = { 32, 32, 32, 32, 32, 32, 32, 12, 13, 13, 12, 12, 12, 12, 12, 12, 11, 12, 12, 12, 12, 13, 12, 13, 12, 32, 32, 32, 32, 32, 32, 32, 16, 18, 19, 20, 22, 22, 24, 25, 26, 28, 28, 30, 29, 30, 28, 26, 25, 24, 22, 21, 20, 18, 18, 16, 14, 14, 14, 14, 14, 14, 14, 14};
	int cordsN[64] = { 32, 32, 32, 32, 32, 32, 32, 32, 12, 12, 12, 11, 11, 11, 12, 12, 11, 12, 11, 11, 12, 12, 12, 11, 31, 32, 32, 32, 32, 32, 32, 32, 16, 17, 18, 19, 19, 20, 21, 22, 23, 24, 24, 26, 26, 26, 26, 26, 26, 26, 25, 26, 25, 24, 23, 22, 21, 20, 20, 19, 18, 17, 16, 15};
	int cordsO[64] = { 12, 15, 18, 22, 24, 26, 26, 22, 20, 16, 14, 14, 14, 14, 12, 12, 12, 12, 14, 14, 15, 14, 16, 20, 23, 26, 26, 24, 22, 18, 16, 10, 10, 16, 18, 22, 24, 26, 22, 19, 17, 16, 17, 16, 16, 15, 14, 14, 14, 14, 14, 16, 16, 17, 16, 18, 18, 22, 26, 24, 22, 18, 16, 10};
	int cordsP[64] = { 32, 32, 32, 32, 32, 32, 32, 32, 32, 14, 13, 13, 13, 13, 13, 13, 13, 13, 13, 14, 15, 16, 18, 20, 20, 19, 18, 16, 16, 14, 11, 8, 23, 25, 27, 29, 30, 30, 31, 21, 19, 18, 18, 18, 18, 19, 20, 21, 31, 30, 29, 27, 26, 23, 10, 9, 9, 9, 9, 9, 9, 9, 9, 9};
	int cordsQ[64] = { 11, 16, 18, 22, 24, 26, 26, 22, 18, 16, 14, 16, 14, 14, 14, 14, 18, 20, 22, 23, 23, 21, 21, 20, 22, 27, 27, 28, 27, 24, 18, 12, 10, 14, 18, 21, 24, 26, 26, 20, 17, 16, 16, 16, 16, 14, 14, 14, 16, 19, 21, 24, 23, 24, 22, 21, 21, 27, 29, 28, 27, 24, 18, 12};
	int cordsR[64] = { 32, 32, 32, 32, 32, 32, 32, 32, 14, 14, 14, 14, 14, 14, 14, 14, 16, 17, 18, 19, 20, 23, 26, 29, 30, 29, 27, 24, 21, 18, 15, 11, 23, 26, 28, 29, 30, 31, 32, 18, 17, 17, 16, 16, 17, 18, 18, 31, 30, 29, 28, 27, 24, 25, 18, 19, 18, 18, 18, 18, 18, 19, 18, 17};
	int cordsS[64] = { 3, 12, 16, 20, 20, 23, 24, 23, 24, 24, 23, 22, 22, 19, 19, 19, 20, 19, 19, 20, 21, 22, 23, 25, 25, 24, 22, 21, 17, 15, 10, 8, 12, 20, 23, 27, 28, 28, 22, 16, 12, 10, 10, 12, 16, 21, 23, 25, 24, 23, 20, 16, 12, 10, 9, 13, 17, 23, 32, 30, 29, 25, 21, 15};
	int cordsT[64] = { 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 32, 32, 32, 32, 32, 32, 32, 32, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 32, 32, 32, 32, 32, 32, 32, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8};
	int cordsU[64] = { 22, 25, 27, 28, 29, 30, 30, 31, 13, 10, 8, 8, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 9, 10, 31, 30, 30, 29, 28, 27, 26, 24, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 17, 17, 17, 18, 18, 19, 21, 29, 28, 26, 24, 22, 18, 14};
	int cordsV[64] = { 4, 6, 9, 11, 13, 16, 18, 20, 21, 20, 20, 19, 19, 16, 15, 12, 11, 14, 17, 19, 19, 19, 20, 20, 20, 18, 15, 13, 11, 9, 6, 4, 16, 16, 17, 18, 16, 18, 16, 16, 18, 16, 18, 16, 16, 16, 16, 16, 16, 17, 16, 16, 15, 16, 16, 14, 14, 12, 12, 11, 10, 8, 8, 8};
	int cordsW[64] = { 5, 9, 14, 18, 22, 24, 24, 20, 14, 13, 17, 21, 21, 21, 19, 14, 14, 18, 23, 21, 22, 18, 14, 14, 19, 22, 23, 23, 19, 14, 10, 6, 15, 15, 16, 17, 18, 18, 18, 19, 19, 20, 20, 20, 21, 22, 19, 20, 20, 20, 21, 20, 20, 20, 19, 16, 16, 16, 15, 12, 12, 12, 12, 8};
	int cordsX[64] = { 4, 7, 9, 12, 15, 17, 20, 23, 26, 25, 25, 23, 21, 18, 16, 13, 12, 16, 18, 20, 24, 25, 24, 25, 22, 20, 17, 16, 12, 10, 6, 4, 18, 19, 19, 18, 19, 19, 18, 19, 18, 19, 18, 16, 15, 14, 12, 10, 11, 12, 14, 16, 17, 18, 18, 20, 20, 18, 18, 19, 19, 18, 18, 18};
	int cordsY[64] = { 2, 4, 5, 7, 8, 10, 11, 13, 13, 14, 13, 14, 25, 23, 22, 20, 20, 22, 23, 25, 13, 13, 13, 13, 13, 12, 10, 9, 7, 5, 4, 3, 16, 18, 18, 18, 18, 17, 18, 18, 17, 18, 18, 17, 18, 16, 14, 14, 12, 10, 10, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8};
	int cordsZ[64] = { 5, 14, 15, 16, 17, 18, 18, 19, 21, 22, 23, 24, 24, 23, 23, 23, 23, 23, 24, 23, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 31, 31, 31, 31, 31, 31, 30, 10, 11, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 10, 10, 11, 11, 11, 11, 11, 31, 32, 32, 32, 32, 32};
	//***************************Output Histogram****************************
	//find the histogram of the input image
	std::string letters[27] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "?" };
	int cords[64] = { 0 };
	for (int y = 0; y < image.rows; y++){
		for (int x = 0; x < image.cols; x++){
			if (image.at<unsigned char>(y, x) != 0){
				cords[x] += 1;
				cords[y + 32] += 1;
			}
		}
	}
	int value = 0;
	int lettervalue = 800;
	int letternumber;
	//compare each histogram with the new
	for (int j = 0; j < 27; j++){
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
			else if (j = 26)
				value += pow(cords[i] - cordsBlank[i], 2);
		}
		value = sqrt(value);
		if (lettervalue > value){//check if the new value is lower than the previous
			std::cout << std::endl << letters[j] << " is now: " << value;
			lettervalue = value;//save this value as the new lowest
			letternumber = j;//save the j value as the letter number				
		}
		value = 0;
	}
	if (lettervalue > 50){
		//imshow("ugly", image);
		std::cout << "ugly: " << lettervalue << std::endl;
		return 1337;
	}
	std::cout << "value: " << lettervalue << ", letter: " << letters[letternumber] << std::endl;
	return letternumber;
}
cv::Mat cropImage(cv::Mat image)
{
	std::cout << "rows: "<< image.rows << ", cols: " << image.cols << std::endl;
	std::vector<cv::Point> points;
	for (int y = 0; y < image.rows; y++){
		for (int x = 0; x < image.cols; x++){
			if (image.at<unsigned char>(y, x) >= 180){
				points.push_back(cv::Point(x, y));
			}
		}
	}
	std::cout << "image size: " << image.cols*image.rows; 
	std::cout << ", 1 pixels: " << points.size();
	int lowx = points[0].x;
	int lowy = points[0].y;
	int highx = points[0].x;
	int highy = points[0].y;
	for (int i = 0; i < points.size(); i++){
		if (points[i].x < lowx){
			lowx = points[i].x;
		}
		if (points[i].x > highx){
			highx = points[i].x;
		}
		if (points[i].y < lowy){
			lowy = points[i].y;
		}
		if (points[i].y > highy){
			highy = points[i].y;
		}
	}
	std::cout << ", low: (" << lowx << ", " << lowy << ") high: (" << highx << ", " << highy << ")" << std::endl;
	cv::Rect roi(lowx, lowy, highx - lowx, highy - lowy);
	return image(roi);
}
cv::Mat removeNoise(cv::Mat image){
	//scale image so we are always working with the same size
	//calculate the scale when it is scaled down to 128 pixels in height

	double scale = 128.0 / double(image.cols);

	cv::Mat imgResize = cv::Mat::zeros(image.rows * scale, image.cols * scale, image.type());
	for (int outY = 0; outY < imgResize.rows; outY++){
		for (int outX = 0; outX < imgResize.cols; outX++){
			imgResize.at<uchar>(outY, outX) = image.at<uchar>((1 / scale) * outY, (1 / scale) * outX);
		}
	}
	image = imgResize.clone();
	//Threshold after resize
	for (int y = 0; y < image.rows; y++){
		for (int x = 0; x < image.cols; x++){
			if (image.at<unsigned char>(y, x) >= 170){
				image.at<unsigned char>(y, x) = 0;
			}
			else{
				image.at<unsigned char>(y, x) = 255;
			}
		}
	}
	cv::Mat element = getStructuringElement(cv::MORPH_RECT,
		cv::Size(3, 3));
	erode(image, image, element);
	dilate(image, image, element);
	//imshow("after closing", image);

	image = cropImage(image);
	imshow("after crop", image);

	return image;
}

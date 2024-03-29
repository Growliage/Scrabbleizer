#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>

# define M_PIl          3.141592653589793238462643383279502884L /* pi */

using namespace cv;
using namespace std;

std::vector<pair<int, int>>boardDetection(cv::Mat image)
{
	Mat img_edge, img_blur;
	img_edge = image;

	int RHO = ((img_edge.rows * img_edge.rows) + (img_edge.cols * img_edge.cols));
	int rho = sqrt(RHO);
	cv::Mat accumulator = Mat::zeros(180, rho, CV_8UC1);

	//rho = xcos(theta) + ysin(theta)
	for (int i = 0; i < img_edge.rows; i++) //y
	{
		for (int j = 0; j < img_edge.cols; j++) //x
		{
			if (img_edge.at<unsigned char>(i, j) != 0)
			{
				for (int t = 0; t < 180; t++)
				{
					double  r = abs(round((j*cos(t)) + (i*sin(t))));
					int theta = t;

					//increment the accumulator (+1) 
					accumulator.at<unsigned char>(theta, r) = accumulator.at<unsigned char>(theta, r) + 1;
				}
			}
		}
	}
	//find global maxima
	int gMax = 0;
	for (int i = 0; i < accumulator.rows; i++){ //y
		for (int j = 0; j < accumulator.cols; j++){ //x
			if (accumulator.at<unsigned char>(i, j) > gMax)
			{
				gMax = accumulator.at<unsigned char>(i, j);
			}
		}
	}
	//threshold for votes in the accumulator
	int thresh = gMax * 0.50;

	//different vectors for start and end point for the line calc and line intersection point
	std::vector< std::pair<int, int>> firstPoint;
	std::vector< std::pair<int, int>> secondPoint;
	std::vector< std::pair<int, int>> intersections;

	waitKey(0);
	int tres = 5;
	int value;
	//Search the accuulator for maxima
	for (int i = 0; i < accumulator.rows; i++) //y
	{
		for (int j = 0; j < accumulator.cols; j++) //x
			if (accumulator.at<unsigned char>(i, j) >= thresh) // threshold the points in the accumulator
			{
				value = accumulator.at<unsigned char>(i, j);

				for (int ly = -tres; ly <= tres; ly++)
				{
					for (int lx = -tres; lx <= tres; lx++)
					{
						if ((ly + i >= 0 && ly + i < accumulator.rows) && (lx + j >= 0 && lx + j < accumulator.cols))
						{
							if ((accumulator.at<unsigned char>(i + ly, j + lx)) > value)
							{
								value = accumulator.at<unsigned char>(i + ly, j + lx);
								lx = ly = tres + 1;
							}
						}
					}
				}
				if (value > accumulator.at<unsigned char>(i, j))
					continue;
				//compute points back into lines
				int x1, x2, y1, y2;
				x1 = x2 = y1 = y1 = 0;

				//threshold the points computed back into lines. i = theta
				if (i > 0 && i < 20)
				{
					//y = (r-x*cos(theta) / sin(theta)
					x1 = 0;
					y1 = (j - (x1*cos(i)) / sin(i));
					x2 = img_edge.cols - 0;
					y2 = (j - (x2*cos(i)) / sin(i));
				}else{
					//x = r-x*sin(theta) / cos(theta)
					y1 = 0;
					x1 = (j - (x1*sin(i)) / cos(i));
					y2 = img_edge.rows - 0;
					x2 = (j - (x2*sin(i)) / cos(i));
				}
				//holds the lines starting point in first and end point in second
				firstPoint.push_back(std::pair<int, int>(std::pair<int, int>(x1, y1)));
				secondPoint.push_back(std::pair<int, int>(std::pair<int, int>(x2, y2)));
			}
	}
//finding and segmenting the line intersection
for (int i = 0; i < firstPoint.size(); i++)
{
	for (int j = 1; j < firstPoint.size(); j++)
	{
		//firstline
		int x1 = firstPoint[i].first;
		int y1 = firstPoint[i].second;
		int x2 = secondPoint[i].first;
		int y2 = secondPoint[i].second;

		//secondline
		int x3 = firstPoint[j].first;
		int y3 = firstPoint[j].second;
		int x4 = secondPoint[j].first;
		int y4 = secondPoint[j].second;

		//tholding for line intersections
		int Lthres = 0;
		int Pxthres = img_edge.cols * 0.1;

		//calc the point of intersection
		float devi = (float)((x1 - x2)*(y3 - y4) - ((y1 - y2)*(x3 - x4)));

		if (devi != 0)
		{
			int Px = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / devi;
			int Py = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / devi;
			if (Px > Pxthres && Px < img_edge.cols-Pxthres)
			{
						if (Py > Lthres && Py < img_edge.rows)
						{
							intersections.push_back(std::pair<int, int>(std::pair<int, int>(Px, Py)));
						}
			}
		}
	}
}

//segment intersections to 1ind board
int topLineLength = img_edge.cols * 0.80; // line threshold % of screen
int MaxLineLength = img_edge.cols * 0.90;

std::vector< std::pair<int, int>> board;

for (int i = 0; i < intersections.size(); i++)
{
	for (int j = 1; j < intersections.size(); j++)
	{
		int ipx = intersections[i].first;
		int ipy = intersections[i].second;
		int jpx = intersections[j].first;
		int jpy = intersections[j].second;

		//find length of a
		int a2 = abs(((ipx - jpx)*(ipx - jpx)) + ((ipy - jpy)*(ipy - jpy)));
		int a = sqrt(a2);
		
		if (a > topLineLength && a < MaxLineLength)
			for (int k = 2; k < intersections.size(); k++)
			{
				int kpx = intersections[k].first;
				int kpy = intersections[k].second;
				//find length of b
				int b2 = abs(((jpx - kpx)*(jpx - kpx)) + ((jpy - kpy)*(jpy - kpy)));
				int b = sqrt(b2);

				//if (b < LL + pl && b > LL - pl)
				if (b > topLineLength && b < MaxLineLength)//b length
				{
					//find length of abc
					int abc2 = abs((a*a) + (b*b));
					int abc = sqrt(abc2);

					//find angle of ABC
					float AngleABC = ((a*a) + (b*b) - (abc * abc)) / (2 * (a*b));
					float Angleabc = acos(AngleABC) * 180.0 / M_PIl;

					if (Angleabc < 91 && Angleabc > 89)
						for (int l = 3; l < intersections.size(); l++)
						{
							int lpx = intersections[l].first;
							int lpy = intersections[l].second;
							//find length of c
							int c2 = abs(((kpx - lpx)*(kpx - lpx)) + ((kpy - lpy)*(kpy - lpy)));
							int c = sqrt(c2);

							int BCC = ((b*b) + (c*c));
							int bbc = sqrt(BCC);

							float AngleBCC = ((a*a) + (b*b) - (bbc*bbc)) / (2 * (a*b));
							float Anglebcc = acos(AngleBCC) * 180.0 / M_PIl;
							
							if (Anglebcc < 91 && Anglebcc > 89 && c == a)
							{
								//find lendth of d
								int d2 = abs(((ipx - lpx) * (ipx - lpx)) + ((ipy - lpy)*(ipy - lpy)));
								int d = sqrt(d2);

								if (d > 0)
								{
									int CDC = ((c*c) + (d*d));
									int cdc = sqrt(CDC);

									float AngleCDC = ((c*c) + (d*d) - (cdc*cdc)) / (2 * (c*d));
									float Anglecdc = acos(AngleCDC) * 180.0 / M_PIl;

									if (Anglecdc < 91 && Anglecdc > 89 && d ==b)
									{
										int ADC = ((a*a) + (d*d));
										int adc = sqrt(ADC);

										float AngleADC = ((a*a) + (d*d) - (adc*adc)) / (2 * (a*d));
										float Angleadc = acos(AngleADC)* 180.0 / M_PIl;

										int angleSum = Angleabc + Angleadc + Anglebcc + Anglecdc;

										if (Angleadc < 91 && Angleadc > 89 && angleSum == 360)
										{
												board.push_back(std::pair<int, int>(std::pair<int, int>(ipx, ipy)));
												board.push_back(std::pair<int, int>(std::pair<int, int>(jpx, jpy)));
												board.push_back(std::pair<int, int>(std::pair<int, int>(kpx, kpy)));
												board.push_back(std::pair<int, int>(std::pair<int, int>(lpx, lpy)));
										}
									}
								}
							}
						}
				}
			}
	}
	
}
std::vector< std::pair<int, int>> TR;
std::vector< std::pair<int, int>> TL;
std::vector< std::pair<int, int>> BR;
std::vector< std::pair<int, int>> BL;
int centerX = img_edge.cols / 2;
int centerY = img_edge.rows / 2;

for (int i = 0; i < board.size(); i++){
	int x = board[i].first;
	int y = board[i].second;

	if (x < centerX && y < centerY){
		TL.push_back(std::pair<int, int>(std::pair<int, int>(x, y)));
	}
	else if (x > centerX && y < centerY){
		TR.push_back(std::pair<int, int>(std::pair<int, int>(x, y)));
	}
	else if (x < centerX && y > centerY){
		BL.push_back(std::pair<int, int>(std::pair<int, int>(x, y)));
	}
	else {
		BR.push_back(std::pair<int, int>(std::pair<int, int>(x, y)));
	}
}
std::vector< std::pair<int, int>> finalBoard;

if (TL.size() == 0){
	std::cout << "could not find upper left corner try again " << std::endl;
	exit(1);
}
if (TR.size() == 0){
	std::cout << "could not find upper right corner try again " << std::endl;
	exit(1);
}
if (BL.size() == 0){
	std::cout << "could not find buttom left corner try again " << std::endl;
	exit(1);
}
if (BR.size() == 0){
	std::cout << "could not find buttom right corner try again " << std::endl;
	exit(1);
}

int sumX = 0;
int sumY = 0;
int meanX = 0;
int meanY = 0;
for (int i = 0; i < TL.size(); i++){
	int x = TL[i].first;
	int y = TL[i].second;

	sumX = sumX + x;
	sumY = sumY + y;
}

meanX = sumX / TL.size();
meanY = sumY / TL.size();

finalBoard.push_back(std::pair<int, int>(std::pair<int, int>(meanX, meanY)));


sumX = sumY = meanX = meanY = 0;
for (int i = 0; i < TR.size(); i++){
	int x = TR[i].first;
	int y = TR[i].second;

	sumX = sumX + x;
	sumY = sumY + y;
}

meanX = sumX / TR.size();
meanY = sumY / TR.size();

finalBoard.push_back(std::pair<int, int>(std::pair<int, int>(meanX, meanY)));

sumX = sumY = meanX = meanY = 0;
for (int i = 0; i < BL.size(); i++){
	int x = BL[i].first;
	int y = BL[i].second;

	sumX = sumX + x;
	sumY = sumY + y;
}

meanX = sumX / BL.size();
meanY = sumY / BL.size();

finalBoard.push_back(std::pair<int, int>(std::pair<int, int>(meanX, meanY)));

sumX = sumY = meanX = meanY = 0;
for (int i = 0; i < BR.size(); i++){
	int x = BR[i].first;
	int y = BR[i].second;

	sumX = sumX + x;
	sumY = sumY + y;
}

meanX = sumX / BR.size();
meanY = sumY / BR.size();

finalBoard.push_back(std::pair<int, int>(std::pair<int, int>(meanX, meanY)));
return(finalBoard);

}
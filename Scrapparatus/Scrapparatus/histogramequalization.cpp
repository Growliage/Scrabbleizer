#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//using namespace cv;
//using namespace std;

int bin = 256;

void imageHistogram(cv::Mat image, int histogram[]);
void cdfcalc(int histogram[], int cumuhistogram[]);

cv::Mat histogramequalization(cv::Mat image)
{
	image = image;
	int nPixels = image.rows * image.cols;

	int histogram[256];
	imageHistogram(image, histogram);

	int cdf[256];
	cdfcalc(histogram, cdf);

	int equalize[256];
	for (int i = 0; i < bin; i++)
	{
		equalize[i] = ((double)(cdf[i] - 1) / (nPixels - 1) * 256 - 1);
	}

	cv::Mat eImage = image.clone();

	for (int y = 0; y < image.rows; y++)
		for (int x = 0; x < image.cols; x++)
			eImage.at<unsigned char>(y, x) = (equalize[image.at<unsigned char>(y, x)]);

	for (int i = 0; i < image.rows; i++){
		for (int j = 0; j < image.cols; j++){
			if (eImage.at<unsigned char>(i, j) > 200){
				eImage.at<unsigned char>(i, j) = 0;
			}
}
	}
	return(eImage);
}

	void imageHistogram(cv::Mat image, int histogram[])
	{
		for (int i = 0; i < 256; i++)
		{
			histogram[i] = 0;
		}
		for (int y = 0; y < image.rows; y++)
			for (int x = 0; x < image.cols; x++)
				histogram[(int)image.at<unsigned char>(y, x)]++;
	}

	void cdfcalc(int histogram[], int cumuhistogram[])
	{
		cumuhistogram[0] = histogram[0];

		for (int i = 1; i < bin; i++)
		{
			cumuhistogram[i] = histogram[i] + cumuhistogram[i - 1];
		}	
	}
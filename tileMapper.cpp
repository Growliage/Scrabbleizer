#include <iostream>
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"


using namespace cv;
using namespace std;

/*Used for testing purposes*/

int width = 500;
int height = 500;

cv::Mat image(width, height, CV_8UC1, cv::Scalar(0));	//Stand in for input image

/*Testing stuff ends here!*/

int main(int, char)
{
	static float xOffset = 6.5; //% offset on the scrabble board from the edge to the playing field on x-axis(Assume upright board)
	static float tempCols = (image.cols)/15;
	static float tempRows = (image.rows) / 15;

	struct mapperInfo {
		float x, y;
		float w = tempCols;
		float h = tempRows;
	} mapInfo[15][15];

	for (int rows = 0; rows < 15; rows++){
		for (int cols = 0; cols < 15; cols++){
			mapInfo[rows][cols].x = rows * tempRows;
			mapInfo[rows][cols].y = cols * tempCols;
		}
	}
	for (int rows = 0; rows < 15; rows++){
		for (int cols = 0; cols < 15; cols++){
			rectangle(image,
				Point(mapInfo[rows][cols].x, mapInfo[rows][cols].y), //Point 1
				Point(mapInfo[rows][cols].x + mapInfo[rows][cols].w, mapInfo[rows][cols].y + mapInfo[rows][cols].h), //point 2
				CV_RGB(0, 255, 255), 1);
		}
	}
	imshow("Image", image);


	cv::waitKey(0);


}
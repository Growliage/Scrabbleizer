#include <iostream>
#include <utility>
#include <tuple>
#include <string>
#include <vector>
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2/opencv.hpp"



static float xOffset = 6.5; //% offset on the scrabble board from the edge to the playing field on x-axis(Assume upright board)
static float tempCols = 640 / 15;//(image.cols) / 15;
static float tempRows = 640 / 15;//(image.rows) / 15;

extern struct tileStruct {
	bool newTile = false;	//Was the tile played this turn?
	bool playablePos = false;	//Is it a playable position?
	cv::String cvLetterTile = "";	//Because openCV doesn't cooperate unless it's treated like a special fucking snowflake.
	char letterTile = '0';	//The letter found at the spcified coordinated
	int tileValue;	//What kind of (premium) tile is it. Gotten from boardValues
	int x, y;	//Overlay rectanlges position on the board
	int w = tempCols;	//Used to find the width on a single tile
	int h = tempRows;	//Used to find the height of a single tile
} tileInfo[15][15];

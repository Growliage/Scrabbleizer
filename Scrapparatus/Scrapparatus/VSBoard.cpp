#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <tuple>

//Structs array used for initialization
struct TileStruct {
	bool newTile = false;	//Was the tile played this turn?
	bool playablePos = false;	//Is it a playable position?
	cv::String cvLetterTile = "";	//Because openCV doesn't cooperate unless it's treated like a special fucking snowflake.
	char letterTile = '0';	//The letter found at the spcified coordinated
	int tileValue;	//What kind of (premium) tile is it. Gotten from boardValues
	int x, y;	//Tiles (x,y) coords on the picture
	float w;	//Used to find the width on a single tile
	float h;	//Used to find the height of a single tile
} tileInfo[15][15];

int VSBoard(cv::Mat image, cv::Mat imageSubtracted, int x1, int  y1, int x2, int y2, int x3, int y3, int x4, int y4){

	/*Forward declarations*/
	//Tile manipulators
	std::vector<std::pair<int, int>> checkTiles(int startX, int startY, int wordLength);
	int placeTiles(std::string input, std::vector<std::pair<int, int>> coords);
	void removeTiles(std::vector<std::pair<int, int>>);
	bool hori(std::vector<std::pair<int, int>> tileLoc);

	//Image manipulators
	std::vector<std::pair<int, int>> tileAnalyzer(cv::Mat imageSubtracted);
	std::string tileCropper(cv::Mat image, std::vector<std::pair<int, int>> tileLoc);
	std::string letterRecognition(cv::Mat imageSlice);

	//Board manipulators
	bool checkTiles(std::vector<std::pair<int, int>> tileLoc, std::string input);
	int placeTiles(std::vector<std::pair<int, int>> tileLoc, std::string input);
	void removeTiles(std::vector<std::pair<int, int>> tileLoc, std::string input);

	//Setting up the tiles
	float tileWidth = (x2 - x1) / 15;
	float tileHeight = (y3 - y1) / 15;

	//Values used by the pointCounter()
	int boardValues[15][15] =
	{
		{ 5, 1, 1, 2, 1, 1, 1, 5, 1, 1, 1, 2, 1, 1, 5 },	//A
		{ 1, 4, 1, 1, 1, 3, 1, 1, 1, 3, 1, 1, 1, 4, 1 },	//B
		{ 1, 1, 4, 1, 1, 1, 2, 1, 2, 1, 1, 1, 4, 1, 1 },	//C
		{ 2, 1, 1, 4, 1, 1, 1, 2, 1, 1, 1, 4, 1, 1, 2 },	//D
		{ 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1 },	//E
		{ 1, 3, 1, 1, 1, 3, 1, 1, 1, 3, 1, 1, 1, 3, 1 },	//F
		{ 1, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1 },	//G
		{ 5, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 5 },	//H
		{ 1, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1 },	//I
		{ 1, 3, 1, 1, 1, 3, 1, 1, 1, 3, 1, 1, 1, 3, 1 },	//J
		{ 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1 },	//K
		{ 2, 1, 1, 4, 1, 1, 1, 2, 1, 1, 1, 4, 1, 1, 2 },	//L
		{ 1, 1, 4, 1, 1, 1, 2, 1, 2, 1, 1, 1, 4, 1, 1 },	//M
		{ 1, 4, 1, 1, 1, 3, 1, 1, 1, 3, 1, 1, 1, 4, 1 },	//N
		{ 5, 1, 1, 2, 1, 1, 1, 5, 1, 1, 1, 2, 1, 1, 5 }		//O
	};

	for (int i = 0; i < 15; i++){
		for (int j = 0; j < 15; j++){
			tileInfo[i][j].w = tileWidth;
			tileInfo[i][j].h = tileHeight;
			tileInfo[i][j].tileValue = boardValues[i][j];
		}
	}

	tileInfo[7][7].playablePos = true;	//Set the middle as the only playable position

	//Set the (x,y) coords of all the tiles
	for (int rows = 0; rows < 15; rows++){
		for (int cols = 0; cols < 15; cols++){
			tileInfo[rows][cols].x = x1 + (tileWidth * cols);
			tileInfo[rows][cols].y = y1 + (tileHeight * rows);
		}
	}

	for (int cols = 0; cols < 15; cols++){
		for (int rows = 0; rows < 15; rows++){
			cv::rectangle(image,
				cv::Point(tileInfo[rows][cols].x, tileInfo[rows][cols].y), //Point 1
				cv::Point(tileInfo[rows][cols].x + tileInfo[rows][cols].w, tileInfo[rows][cols].y + tileInfo[rows][cols].h), //point 2
				CV_RGB(0, 255, 255), 1);
		}
	}
	std::vector<std::pair<int, int>> tileLoc = tileAnalyzer(imageSubtracted);	//Find where new tiles are placed
	bool horizontal = hori(tileLoc);	//Check if it's horiszontal or vertical
	std::string input = tileCropper(image, tileLoc);	//Have tileCropper send individual letters to letterRecognition

	if (checkTiles(tileLoc, input) == true){	//Preliminary check to see if tiles are placed according to the rules
	
		placeTiles(tileLoc, input);

	}
	/*PSEUDO: If true, stop and wait for input to check the word in SOWPODS or for the user to acknowledge the next play.
	If false, ask the user to remove the tiles due to illegal play*/

	return(1);

}

std::vector<std::pair<int, int>> tileAnalyzer(cv::Mat imageSubtracted){

	float threshold = 0.1;	//Threshold for when a location needs to be noted

	std::vector<std::pair<int, int>> tileLoc;
	int totalPixelsinStruct = tileInfo[0][0].w * tileInfo[0][0].h;

	for (int rows = 0; rows < 15; rows++){
		for (int cols = 0; cols < 15; cols++){


			int pixelsCounter = 0;
			for (int tileRows = tileInfo[rows][cols].x; tileRows < tileInfo[rows][cols].x + tileInfo[rows][cols].w; tileRows++){
				for (int tileCols = tileInfo[rows][cols].y; tileCols < tileInfo[rows][cols].y + tileInfo[rows][cols].h; tileCols++){

					if ((imageSubtracted.at<unsigned char>(tileRows, tileCols) > 128)){
						pixelsCounter++;
					}
				}	//tile cols
			}	//tile rows

			if (((float)pixelsCounter / (float)totalPixelsinStruct) > threshold){
				tileLoc.push_back(std::make_pair(cols, rows));
			}

		}	//Cols on image
	}	//Rows on image

	return(tileLoc);

}

std::string tileCropper(cv::Mat image, std::vector<std::pair<int, int>> tileLoc){

	//Forward declaration
	std::string letterRecognition(cv::Mat imageSlice);

	cv::Mat imageSlice;
	cv::Mat imageROI;
	std::string sWord = "";
	imshow("Full image", image);

	for (int i = 0; i < tileLoc.size(); i++){
		int x = tileInfo[tileLoc[i].first][tileLoc[i].second].x;
		int y = tileInfo[tileLoc[i].first][tileLoc[i].second].y;
		int w = tileInfo[tileLoc[i].first][tileLoc[i].second].w;
		int h = tileInfo[tileLoc[i].first][tileLoc[i].second].h;
		imageROI = image(cv::Rect(y, x, w, h));
		imageROI.copyTo(imageSlice);
		imshow("slice", imageSlice);
		cv::waitKey(0);
		sWord.append(letterRecognition(imageSlice));
	}

	return(sWord);
	//return("word");
}

//This doesn't look like it's going to be needed
bool hori(std::vector<std::pair<int, int>> tileLoc){

	bool hori;

	if (tileLoc[0].first == tileLoc[1].first){
		hori = true;
	}
	else{
		hori = false;
	}

	return(hori);

}

bool checkTiles(std::vector<std::pair<int, int>> tileLoc, std::string input){

	bool playablePos = false;
	bool letterMatch = true;

	//Check for playable position. This only needs to be true on one tile.
	for (int i = 0; i < tileLoc.size(); i++){
		if (tileInfo[tileLoc[i].first][tileLoc[i].second].playablePos == true){
			playablePos = true;
			break;
		}
	}

	for (int i = 0; i < tileLoc.size(); i++){
		if (tileInfo[tileLoc[i].first][tileLoc[i].second].letterTile == '0'){	//If the tile is empty, the position is valid
			continue;
		}
		else if (tileInfo[tileLoc[i].first][tileLoc[i].second].letterTile == input.at(i)) {	//If a tile has been played, check to see if it's the same letter
			continue;
		}
		else {	//If neither is true, the placement is invalid
			letterMatch = false;
			break;
		}
	}

	bool validPlacement = playablePos && letterMatch;	//(AND GATE) Both statements must be true for it to be a valid placement of tiles

	return(validPlacement);
}

int placeTiles(std::vector<std::pair<int, int>> tileLoc, std::string input){

	//Forward declaration
	int pointCounter(std::string input, std::vector<int> premiumTiles, bool allTiles);

	int tilesPlayed = 0;
	bool allTiles = false;
	std::vector<int> premiumTiles;

	for (int i = 0; i < 15; i++){	//Lock in all tiles
		for (int j = 0; j < 15; j++){
			tileInfo[i][j].newTile = false;
		}
	}

	for (int i = 0; i < tileLoc.size(); i++){
		if (tileInfo[tileLoc[i].first][tileLoc[i].second].letterTile == '0'){	//Check if a tile is played on a blank space
			tileInfo[tileLoc[i].first][tileLoc[i].second].newTile = true;	//Set position as newly played tile to allow for removing
			tilesPlayed++;
		}
		premiumTiles.push_back(tileInfo[tileLoc[i].first][tileLoc[i].second].tileValue);	//Get the premium values for point counting
		tileInfo[tileLoc[i].first][tileLoc[i].second].letterTile = input.at(i);	//Set letter at position as a char
		tileInfo[tileLoc[i].first][tileLoc[i].second].cvLetterTile = input.at(i);	//Set letter at position as cvString. Used to draw the board

		//Set playable positions around the played letters as true
		tileInfo[(tileLoc[i].first) - 1][tileLoc[i].second].playablePos = true;
		tileInfo[(tileLoc[i].first) + 1][tileLoc[i].second].playablePos = true;
		tileInfo[(tileLoc[i].first)][(tileLoc[i].second) - 1].playablePos = true;
		tileInfo[(tileLoc[i].first)][(tileLoc[i].second) + 1].playablePos = true;
		tileInfo[(tileLoc[i].first)][(tileLoc[i].second)].playablePos = false; //Set the actual position to false(WOW SUCH HACK! MUCH UGLY!)
	}

	if (tilesPlayed == 7){
		allTiles = true;
	}

	return(pointCounter(input, premiumTiles, allTiles));

}

void removeTiles(std::vector<std::pair<int, int>> tileLoc, std::string input){


	for (int i = 0; i < 15; i++){	//Lock in all tiles
		for (int j = 0; j < 15; j++){
			if (tileInfo[i][j].newTile == true){
				tileInfo[i][j].letterTile = '0';
				tileInfo[i][j].cvLetterTile = "";
				tileInfo[i][j].newTile = false;
			}
		}
	}

}
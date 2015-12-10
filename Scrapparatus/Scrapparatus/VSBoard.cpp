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
	int x, y;	//Tiles upper left corner (x,y) coords on the picture
	int w;	//Used to find the width on a single tile
	int h;	//Used to find the height of a single tile
} tileInfo[15][15];

std::vector<int> VSBoard(cv::Mat image, cv::Mat imageSubtracted, int x1, int  y1, int x4, int y4){

	std::vector<int> returnVector;
	returnVector.push_back(0);	//Points (if any) awarded
	returnVector.push_back(0);	//Did the player skip their turn(1 true, 0 false)
	returnVector.push_back(0); //Was the word contested?
	returnVector.push_back(0); //If it was contested, which player was it?
	returnVector.push_back(0); //Was it successfully contested?

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
	void removeTiles(std::vector<std::pair<int, int>> tileLoc);
	bool SOWPODSsearch(std::string input);

	//Setting up the tiles
	int tileWidth = (x4 - x1) / 15;
	int tileHeight = (y4 - y1) / 15;

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

	bool keepRunning = true;

	do{
		std::vector<std::pair<int, int>> tileLoc = tileAnalyzer(imageSubtracted);	//Find where new tiles are placed

		if (tileLoc.empty()){	
			returnVector[1] = 1;
			keepRunning = false;
			return(returnVector);
		}

		std::string input = tileCropper(image, tileLoc);	//Have tileCropper send individual letters to letterRecognition

		if (checkTiles(tileLoc, input) == true){	//Preliminary check to see if tiles are placed according to the rules

			std::cout << "\nThe word played is" << input << ". Type \"C\" to contest.";
			std::string playerInput;
			std::cin >> playerInput;

			if (playerInput == "c" || "C"){
				returnVector[2] = 1;
				std::cout << "\nWhich player is contesting the word?" << std::endl;
				int player;
				std::cin >> player;
				returnVector[3] = player;
				bool validWord = SOWPODSsearch(input);

				if (validWord == false){
					std::cout << "\nThe word does not exist! Please remove the tiles.";
					returnVector[4] = 1;
					removeTiles(tileLoc);

				}
				else {
					std::cout << "\nThat is a real word.";
					returnVector[0] = placeTiles(tileLoc, input);
					keepRunning = false;
					return(returnVector);
				}
			}
			else {
				returnVector[0] = placeTiles(tileLoc, input);
				keepRunning = false;
				return(returnVector);
			}
		}
		else {
			std::cout << "\nInvalid placement.";
		}
	} while ( keepRunning == true);

}

std::vector<std::pair<int, int>> tileAnalyzer(cv::Mat imageSubtracted){

	float threshold = 0.1;	//Threshold for when a location needs to be noted

	std::vector<std::pair<int, int>> tileLoc;
	int totalPixelsinStruct = tileInfo[0][0].w * tileInfo[0][0].h;

	for (int structRow = 0; structRow < 15; structRow++){
		for (int structCol = 0; structCol < 15; structCol++){
			
			int pixelsCounter = 0;
			for (int tileRows = tileInfo[structRow][structCol].x; tileRows < tileInfo[structRow][structCol].x + tileInfo[structRow][structCol].w; tileRows++){
				for (int tileCols = tileInfo[structRow][structCol].y; tileCols < tileInfo[structRow][structCol].y + tileInfo[structRow][structCol].h; tileCols++){
					std::cout << tileRows << "   " << tileCols << std::endl;
					if ((imageSubtracted.at<unsigned char>(tileCols, tileRows) > 128)){
						pixelsCounter++;
					}
				}	//tile cols
			}	//tile rows

			if (((float)pixelsCounter / (float)totalPixelsinStruct) > threshold){
				tileLoc.push_back(std::make_pair(structCol, structRow));
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

		std::string tempString = letterRecognition(imageSlice);

		if (tempString == "Error"){
			sWord.append("?");
		}
		else {
			sWord.append(tempString);
		}
	}

	return(sWord);
	//return("word");
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

void removeTiles(std::vector<std::pair<int, int>> tileLoc){


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
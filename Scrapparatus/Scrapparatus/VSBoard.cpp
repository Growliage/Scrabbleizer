#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <math.h>

//Structs array used for initialization
struct TileStruct {
	bool playablePos = false;	//Is it a playable position?
	char letterTile = '0';	//The letter found at the spcified coordinated
	int tileValue;	//What kind of (premium) tile is it. Gotten from boardValues
	int x, y;	//Tiles upper left corner (x,y) coords on the picture
	float w;	//Used to find the width on a single tile
	float h;	//Used to find the height of a single tile
} tileInfo[15][15];

int VSBoard(cv::Mat image, cv::Mat imageSubtracted, int x1, int  y1, int x4, int y4){

	/*Forward declarations*/
	//Tile manipulators
	std::vector<std::pair<int, int>> checkTiles(int startX, int startY, int wordLength);
	int placeTiles(std::string input, std::vector<std::pair<int, int>> coords);

	//Image manipulators
	std::vector<std::pair<int, int>> tileAnalyzer(cv::Mat imageSubtracted);
	std::string tileCropper(cv::Mat image, std::vector<std::pair<int, int>> tileLoc);
	std::string letterRecognition(cv::Mat imageSlice);

	//Board manipulators
	bool checkTiles(std::vector<std::pair<int, int>> tileLoc, std::string input);
	int placeTiles(std::vector<std::pair<int, int>> tileLoc, std::string input);
	bool SOWPODSsearch(std::string input);

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
				tileInfo[i][j].w = (float)(x4 - x1) / 15;
				tileInfo[i][j].h = (float)(y4 - y1) / 15;
			tileInfo[i][j].tileValue = boardValues[i][j];
		}
	}

	tileInfo[7][7].playablePos = true;	//Set the middle as the only playable position

	//Set the (x,y) coords of all the tiles
	for (int rows = 0; rows < 15; rows++){
		for (int cols = 0; cols < 15; cols++){
			tileInfo[rows][cols].x = x1 + (tileInfo[rows][cols].w * cols);
			tileInfo[rows][cols].y = y1 + (tileInfo[rows][cols].h * rows);
		}
	}


	do{
		std::vector<std::pair<int, int>> tileLoc = tileAnalyzer(imageSubtracted);	//Find where new tiles are placed

		if (tileLoc.empty()){	//If the returned value is empty, assume that no tiles were played
			return(0);
		}

		std::string input = tileCropper(image, tileLoc);	//Have tileCropper send individual letters to letterRecognition

		if (checkTiles(tileLoc, input) == true){	//Preliminary check to see if tiles are placed according to the rules

			std::cout << "\nThe word played is" << input << ". Type \"C\" to contest.";
			std::string playerInput;
			std::cin >> playerInput;

			if (playerInput.compare("c") == 0 || playerInput.compare("C") == 0){
				bool validWord = SOWPODSsearch(input);

				if (validWord == false){
					std::cout << "\nThe word does not exist! Please remove the tiles.";
					return(0);

				}
				else {
					std::cout << "\nThat word exists";
					return(placeTiles(tileLoc, input));

				}
			}
			else {
				return(placeTiles(tileLoc, input));
			}
		}
		else {
			std::cout << "\nInvalid placement.";
		}

	} while (true);

}

std::vector<std::pair<int, int>> tileAnalyzer(cv::Mat imageSubtracted){

	float threshold = 0.10;	//Threshold for when a location needs to be noted (1 = all, 0 = none)

	std::vector<std::pair<int, int>> tileLoc;
	int totalPixelsinStruct = tileInfo[0][0].w * tileInfo[0][0].h;

	for (int structRow = 0; structRow < 15; structRow++){
		for (int structCol = 0; structCol < 15; structCol++){
			
			int pixelsCounter = 0;
			for (int tileRows = tileInfo[structRow][structCol].x + 5; tileRows < tileInfo[structRow][structCol].x + tileInfo[structRow][structCol].w - 5; tileRows++){
				for (int tileCols = tileInfo[structRow][structCol].y + 5; tileCols < tileInfo[structRow][structCol].y + tileInfo[structRow][structCol].h - 5; tileCols++){
					
					if ((imageSubtracted.at<unsigned char>(tileCols, tileRows) > 128)){
						pixelsCounter++;
					}
				}	//tile cols
			}	//tile rows

			if (((float)pixelsCounter / (float)totalPixelsinStruct) > threshold){
				tileLoc.push_back(std::make_pair(structRow, structCol));

			}

		}	//Struct column
	}	//Struct row

	return(tileLoc);

}

std::string tileCropper(cv::Mat image, std::vector<std::pair<int, int>> tileLoc){

	//Forward declaration
	std::string letterRecognition(cv::Mat imageSlice);

	cv::Mat imageSlice;	//The image to send to letter recognition
	cv::Mat imageROI;	//The region of interest that needs to be sliced
	std::string sWord = "";	

	for (int i = 0; i < tileLoc.size(); i++){
		int x = tileInfo[tileLoc[i].first][tileLoc[i].second].x;
		int y = tileInfo[tileLoc[i].first][tileLoc[i].second].y;
		int w = tileInfo[tileLoc[i].first][tileLoc[i].second].w;
		int h = tileInfo[tileLoc[i].first][tileLoc[i].second].h;
		imageROI = image(cv::Rect(x, y, w, h));
		imageROI.copyTo(imageSlice);

		
		std::string tempString = letterRecognition(imageSlice);

		if (tempString == "Error"){
			sWord.append("?");	//Translate recognition errors to blank tiles
		}
		else {
			sWord.append(tempString);
		}
	}

	for (int i = 0; i < sWord.length(); i++){
		if (sWord.at(i) == '?'){
			std::cout << "\nWhat is letter number " << i + 1 << "?" << std::endl;	//Ask the player what letter is on a blank tile
			char letter;
			std::cin >> letter;
			sWord.at(i) = letter;
		}
	}
	return(sWord);
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

	bool validPlacement = playablePos && letterMatch;	//(AND) Both statements must be true for it to be a valid placement of tiles

	return(validPlacement);
}

int placeTiles(std::vector<std::pair<int, int>> tileLoc, std::string input){

	//Forward declaration
	int pointCounter(std::string input, std::vector<int> premiumTiles, bool allTiles);

	int tilesPlayed = 0;
	bool allTiles = false;
	std::vector<int> premiumTiles;


	for (int i = 0; i < tileLoc.size(); i++){
		if (tileInfo[tileLoc[i].first][tileLoc[i].second].letterTile == '0'){	//Check if a tile is played on an empty spot
			tilesPlayed++;
		}
		premiumTiles.push_back(tileInfo[tileLoc[i].first][tileLoc[i].second].tileValue);	//Get the premium values for point counting
		
		tileInfo[tileLoc[i].first][tileLoc[i].second].letterTile = input.at(i);	//Set letter at position as a char
		
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
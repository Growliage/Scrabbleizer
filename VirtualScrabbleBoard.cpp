#include <iostream>
#include <string>
#include <vector>
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

int main(){

/*-----TESTING STUFF START!-----*/

int width = 500;
int height = 500;

cv::Mat image(width, height, CV_8UC1, cv::Scalar(0));	//Stand in for input image

/*-----TESTING STUFF END!-----*/

//Forward declarations
int placeTiles();
bool checkTiles(int startX, int startY, std::string input, bool hori);
//int pointCounter(std::string input, std::vector<int> premiumTiles, bool allTiles);

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

static float xOffset = 6.5; //% offset on the scrabble board from the edge to the playing field on x-axis(Assume upright board)
static float tempCols = (image.cols) / 15;
static float tempRows = (image.rows) / 15;

//Structs array used for initialization
struct tileStruct {
	bool newTile = false;	//Was the tile played this turn?
	bool playablePos = false;	//Is it a playable position?
	char letterTile = '0';	//The letter found at the spcified coordinated
	int tileValue;	//What kind of (premium) tile is it. Gotten from boardValues
	float x, y;	//Overlay rectanlges position on the board
	float w = tempCols;	//Used to find the width on a single tile
	float h = tempRows;	//Used to find the height of a single tile
} tileInfo[15][15];


	//Initialize all the structs!

	for (int rows = 0; rows < 15; rows++){
		for (int cols = 0; cols < 15; cols++){
			tileInfo[rows][cols].tileValue = boardValues[rows][cols];
			tileInfo[rows][cols].x = rows * tempRows;
			tileInfo[rows][cols].y = cols * tempCols;
		}
	}
	tileInfo[7][7].playablePos = true;	//Set the middle tile to be a place where a tile can be placed
	do{
		/*-----TESTING STUFF START!-----*/
		for (int cols = 0; cols < 15; cols++){
			for (int rows = 0; rows < 15; rows++){
				rectangle(image,
					cv::Point(tileInfo[rows][cols].x, tileInfo[rows][cols].y), //Point 1
					cv::Point(tileInfo[rows][cols].x + tileInfo[rows][cols].w, tileInfo[rows][cols].y + tileInfo[rows][cols].h), //point 2
					CV_RGB(0, 255, 255), 1);
			}
		}
		/*----TESTING STUFF END!-----*/

		placeTiles();
		imshow("Image", image);
		cv::waitKey(0);


	} while (true);
}

bool checkTiles(int startX, int startY, std::string input, bool hori){

	bool validPlacement = false;

	if (hori == true){
		for (int rowPos = 0; rowPos < input.length(); rowPos++){
			if (tileInfo[startX + rowPos][startY].playablePos == true){ //Check if ANY ONE TILE is in a valid position
				validPlacement = true;
				break;
			}
		}
		for (int rowPos = 0; rowPos < input.length(); rowPos++){	//This must be true at EVERY position to be accepted
			if (tileInfo[startX + rowPos][startY].letterTile != (char)'0'){	//If the tile is not empty check to see if the letters match
				if (tileInfo[startX + rowPos][startY].letterTile != (char)input.at(rowPos)){
					validPlacement = false;
					break;
				}
			}
		}
	}
	else
	{
		for (int colPos = 0; colPos < input.length(); colPos++){
			if (tileInfo[startX][startY + colPos].playablePos == true){
				validPlacement = true;
				break;
			}
		}
		for (int colPos = 0; colPos < input.length(); colPos++){
			if (tileInfo[startX][startY + colPos].letterTile != (char)'0'){
				if (tileInfo[startX][startY + colPos].letterTile != (char)input.at(colPos)){
					validPlacement = false;
					break;
				}
			}
		}
	}

	return(validPlacement);

}

int placeTiles(int startX, int startY, bool hori){

	std::string input;
	int startX;
	int startY;
	std::string temp;
	int tilesPlayed = 0;
	std::vector<int> premiumTiles;
	bool allTiles = false;
	bool hori;

	std::cout << "\nEnter the word to be played:" << std::endl;
	std::cin >> input;
	std::cout << "\nX: ";
	std::cin >> startX;
	std::cout << "\nY: ";
	std::cin >> startY;
	std::cout << "\n(V)ertical or (H)orizontal? ";
	std::cin >> temp;

	if (temp == "h" || temp == "H"){
		hori = true;
	}
	else {
		hori = false;
	}

	if (checkTiles(startX, startY, input, hori) == true){

		if (hori == true){

			for (int rowPos = 0; rowPos < input.length(); rowPos++){
				if ((char)tileInfo[startX][startY + rowPos].letterTile == '0'){
					tilesPlayed++;
				}
				premiumTiles.push_back(tileInfo[startX + rowPos][startY].tileValue);
				tileInfo[startX + rowPos][startY].letterTile = input.at(rowPos);
				tileInfo[startX + rowPos][startY].newTile = true;
				tileInfo[startX + rowPos][startY + 1].playablePos = true;
				tileInfo[startX + rowPos][startY - 1].playablePos = true;
			}
			tileInfo[startX - 1][startY].playablePos = true;
			tileInfo[startX + input.length()][startY].playablePos = true;
		}
		else {

			for (int colPos = 0; colPos < input.length(); colPos++){
				if ((char)tileInfo[startX][startY + colPos].letterTile == '0'){
					tilesPlayed++;
				}
				premiumTiles.push_back(tileInfo[startX][startY + colPos].tileValue);
				tileInfo[startX][startY + colPos].letterTile = input.at(colPos);
				tileInfo[startX][startY + colPos].newTile = true;
				tileInfo[startX + 1][startY + colPos].playablePos = true;
				tileInfo[startX - 1][startY + colPos].playablePos = true;

			}
			tileInfo[startX][startY - 1].playablePos = true;
			tileInfo[startX][startY + input.length()].playablePos = true;
		}

		if (tilesPlayed == 7){
			allTiles = true;
		}

		return(pointCounter(input, premiumTiles, allTiles));
	}
	else
	{
		std::cout << "\nInvalid placement!";
	}

}

int removeTiles(int startX, int startY, bool hori){

	if (hori == true){

		for (int rowPos = 0; rowPos < input.length(); rowPos++){
			if (tileInfo[startX + rowPos][startY].newTile == true){
				tileInfo[startX + rowPos][startY].letterValue = '0';
				tileInfo[startX + rowPos][startY].newTile = false;
			}
		}
	}
	else 
	{
		for (int colPos = 0; colPos < input.length(); colPos++){
			if (tileInfo[startX][startY + colPos].newTile == true){
				tileInfo[startX][startY + colPos].letterValue = '0'
					tileInfo[startX][startY + colPos].newTile = false;
			}
		}
	}
}

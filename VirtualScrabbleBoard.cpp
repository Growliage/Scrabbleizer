#include <iostream>
#include <string>
#include <vector>

//Forward declarations
int placeTiles();
bool checkTiles(int startX, int startY, std::string input, bool hori);
int pointCounter(std::string input, std::vector<int> premiumTiles, bool allTiles);
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

//Structs array used for initialization
struct tileStruct {
	bool newTile = false;
	bool playablePos = false;
	char letterTile = '0';
	int tileValue;
} tileInfo[15][15];

int main(){

	//Initialize all the structs!

		for (int rows = 0; rows < 15; rows++){
			for (int cols = 0; cols < 15; cols++){
				tileInfo[rows][cols].tileValue = boardValues[rows][cols];
			}
		}
		tileInfo[7][7].playablePos = true;	//Set the middle tile to be a place where a tile can be placed
		do{
			std::cout << "Board:" << std::endl;
			for (int cols = 0; cols < 15; cols++){
				std::cout << std::endl;
				for (int rows = 0; rows < 15; rows++){
					std::cout << tileInfo[rows][cols].playablePos;
				}
			}

			placeTiles();

	} while (true);
}

int placeTiles(){	

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

};

bool checkTiles(int startX, int startY, std::string input, bool hori){

	bool validPlacement = false;

	if (hori == true){
		for (int rowPos = 0; rowPos < input.length(); rowPos++){
			if (tileInfo[startX + rowPos][startY].playablePos == true){
				validPlacement = true;
				break;
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
	}

	return(validPlacement);

}

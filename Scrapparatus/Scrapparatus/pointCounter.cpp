#include <map>
#include <string>
#include <vector>

int pointCounter(std::string input, std::vector<int> premiumTiles, bool allTiles){

	std::map<char, int>pointValue;

	//blank
	pointValue['?'] = 0;

	//1 point letter tiles
	pointValue['a'] = 1; pointValue['e'] = 1; pointValue['i'] = 1; pointValue['l'] = 1; pointValue['n'] = 1; 
	pointValue['o'] = 1; pointValue['r'] = 1; pointValue['s'] = 1; pointValue['t'] = 1; pointValue['u'] = 1;

	//2 point letter tiles
	pointValue['d'] = 2; pointValue['g'] = 2;

	//3 point letter tiles
	pointValue['b'] = 3; pointValue['c'] = 3; pointValue['m'] = 3; pointValue['p'] = 3;

	//4 point letter tiles
	pointValue['f'] = 4; pointValue['h'] = 4; pointValue['v'] = 4; pointValue['w'] = 4; pointValue['y'] = 4;

	//5 point letter tiles
	pointValue['k'] = 5;

	//8 point letter tiles
	pointValue['j'] = 8; pointValue['x'] = 8;

	//10 point letter tiles
	pointValue['q'] = 10; pointValue['z'] = 10;

	int totalPoints = 0, doubleWord = 0, tripleWord = 0;

	for (unsigned i = 0; i <= input.length() - 1; i++){
		if (premiumTiles.at(i) == 4){
			doubleWord = doubleWord++;
		}
		else if (premiumTiles.at(i) == 5){
			tripleWord = tripleWord++;
		} else
		totalPoints = totalPoints + (pointValue[input.at(i) + 1]*premiumTiles.at(i));
		}

	while (doubleWord != 0){
		totalPoints = totalPoints * 2;
		doubleWord = doubleWord--;
	}

	while (tripleWord != 0){
		totalPoints = totalPoints * 3;
		tripleWord = tripleWord--;
	}

	if (allTiles == true){
		totalPoints = totalPoints + 50;
	}
	
	return(totalPoints);
}
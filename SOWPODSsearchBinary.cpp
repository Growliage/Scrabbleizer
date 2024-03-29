#include <fstream>
#include <string>
#include <iostream>
#include <vector>

bool debugMode = 0;

bool SOWPODSsearch(std::string input){

	//Assume the word doesn't exist in the library
	bool realWord = false;
	bool binarySearch(std::vector<std::string> lines, std::string input);

	if (input.length() < 2 || input.length() > 15){
		std::cout << "The word does not conform to the correct length!" << std::endl;
		return(0);
	}
	else {
		//Set the fileName variable to the amount of letters in the word to ensure it looks in the right .txt file
		std::string fileName = "data/" + std::to_string(input.length()) + ".txt";
		std::ifstream dict("fileName");

		dict.open(fileName);

		//Moving the filestream into a vector
		std::vector<std::string> lines;
		for (std::string line; std::getline(dict, line);)
			lines.push_back(line);

		std::cout << "Dictionary has been loaded." << std::endl;

		//Binary search
		//if(std::binary_search(lines, input) {
		//realWord = true;
		//}

		return(binarySearch(lines, input));

		dict.close();
	}
		//return(realWord);
}
	//bool binarySearch(std::vector<std::string> lines, std::string input, int vectorMiddle, int stringIndexToCheck);
	//bool realWord = (binarySearch(lines, input, (lines.size() / 2), 0));
	
	
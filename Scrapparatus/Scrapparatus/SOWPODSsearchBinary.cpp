#include <string>
#include <iostream>
#include <vector>

bool debugMode = 0;

bool SOWPODSsearch(std::string input){

	//Assume the word doesn't exist in the library
	bool realWord = false;
	bool binarySearch(std::vector<std::string> linesN, std::string input);

	if (input.length() < 2 || input.length() > 15){
		std::cout << "The word does not conform to the correct length!" << std::endl;
		return(0);
	}
	else {
		//Set the fileName variable to the amount of letters in the word to ensure it looks in the right .txt file
		std::string linesN = "lines" + std::to_string(input.length());

		//Binary search
		//if(std::binary_search(lines, input) {
		//realWord = true;
		//}

		return(binarySearch(linesN, input));

	}
		//return(realWord);
}
	//bool binarySearch(std::vector<std::string> lines, std::string input, int vectorMiddle, int stringIndexToCheck);
	//bool realWord = (binarySearch(lines, input, (lines.size() / 2), 0));
	
	
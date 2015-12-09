#include <iostream>
#include <string>
#include <vector>

bool binarySearch(std::vector<std::string> lines, std::string input){

	unsigned int vMid = lines.size() / 2;
	unsigned int vMin = 0;
	unsigned int vMax = lines.size() - 1;
	unsigned int stringI = 0;

		while (vMin < vMax && stringI < input.length()){

			vMid = (vMin + vMax) / 2;

			 if (lines.operator[](vMid).at(stringI) == input.at(stringI)){
				int vMinTemp = vMid;
				int vMaxTemp = vMid;

				while (vMinTemp != 0 &&lines.operator[](vMinTemp).at(stringI) == lines.operator[](vMinTemp - 1).at(stringI)){
					vMinTemp--;
				}

				while (vMaxTemp != lines.size() - 1 && lines.operator[](vMaxTemp).at(stringI) == lines.operator[](vMaxTemp + 1).at(stringI)){
					vMaxTemp++;
				}

				vMin = vMinTemp;
				vMax = vMaxTemp;
				stringI++;
				continue;
			}
			else if (lines.operator[](vMid).at(stringI) < input.at(stringI)){
				vMin = vMid + 1;
			}
			else {
				vMax = vMid - 1;			
			}
		}

			if (input.compare(lines.operator[](vMin)) == 0){
				return(true);
			}
			else {
				return (false);
			}

}
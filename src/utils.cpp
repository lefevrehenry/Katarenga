#include "utils.hpp"
#include <string>

// Generates 4 different random values between 0 and 7.
void pickRand(int *a, int *b, int *c, int *d){
	srand (time(NULL));
	*a = rand()%8;
	do {
		*b = rand()%8;
	} while(*b == *a);
	do {
		*c = rand()%8;
	} while(*c == *a || *c == *b);
	do {
		*d = rand()%8;
	} while(*d == *a || *d == *b || *d == *c);
}

// Generates a Sring containing the cell types of the board, row by row.
std::string generateBoardString(std::string & board_configuration)
{
	int a,b,c,d;

    if (board_configuration.size() == 4)
    {
        a = stoi(board_configuration.substr(0,1));
        b = stoi(board_configuration.substr(0,2));
        c = stoi(board_configuration.substr(0,3));
        d = stoi(board_configuration.substr(0,4));
    }
    else
    {
        pickRand(&a, &b, &c, &d);
        board_configuration = std::to_string(a) + std::to_string(b) + std::to_string(c) + std::to_string(d);
    }

	std::string s1 = "";
	std::string s2 = "";
	for(int i = 0; i<4; ++i){
		for(int j = 0; j<4; ++j){
			s1+= bb[a][i][j];
			s2+= bb[c][i][j];
		}
		for(int j = 0; j<4; ++j){
			s1+= bb[b][i][j];
			s2+= bb[d][i][j];
		}
	}
	return (s1+s2);
}



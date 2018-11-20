#include "utils.hpp"

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
std::string generateBoardString()
{
	int a,b,c,d;
    pickRand(&a, &b, &c, &d);

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



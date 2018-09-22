#include "Board.hpp"

#include<iostream>

int main()
{
	Board board;
	board.print();

	std::vector<move> list = board.getAllMoves(1,4);
	for(move m: list){
		std::cout << m.first << m.second << std::endl;
	}

	return 0;
}

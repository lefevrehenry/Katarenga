#include "Board.hpp"

#include<iostream>

int main()
{
	Board board;
	board.print();

    //board.doTest();
    board.main_loop();

	return 0;
}

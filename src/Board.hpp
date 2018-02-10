#ifndef BOARD_HPP
#define BOARD_HPP

#include "Cell.hpp"
#include "Piece.hpp"

#include <vector>

class Board
{
public:
	Board();
	~Board();

	void print();

private:
	BoardCell _plateau[8][8];			// The Board
	CampCell _campW[2];					// The 2 CampCells of White
	std::vector<Piece*> _piecesW;		// The list of White Pieces

	CampCell _campB[2];					// The 2 CampCells of Black
	std::vector<Piece*> _piecesB;		// The list of Black Pieces

	int _currentPlayer = 1;					// 1 if White, -1 if Black
	int _isWin = 0;							// 0 if not win, 1 if White wins, -1 if Black wins
};

#endif // BOARD_HPP

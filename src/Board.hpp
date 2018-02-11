#ifndef BOARD_HPP
#define BOARD_HPP

#include "Cell.hpp"
#include "Piece.hpp"

#include <vector>
#include <utility>

class Board
{
public:
	Board();
	~Board();

	bool movePiece(int s_row, int s_col, int e_row, int e_col);

	bool isValidMove(int s_row, int s_col, int e_row, int e_col);

	std::vector<std::pair<int,int>> getAllMoves(int row, int col);


	void print();

private:
	Cell *** _plateau;			// The Board containing the 8*8 BoardCells and the 4 CampCells
	//CampCell _campW[2];					// The 2 CampCells of White
	std::vector<Piece*> _piecesW;		// The list of White Pieces

	//CampCell _campB[2];					// The 2 CampCells of Black
	std::vector<Piece*> _piecesB;		// The list of Black Pieces

	int _currentPlayer = 1;					// 1 if White, -1 if Black
	int _isWin = 0;							// 0 if not win, 1 if White wins, -1 if Black wins
};

#endif // BOARD_HPP

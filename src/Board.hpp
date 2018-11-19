#ifndef BOARD_HPP
#define BOARD_HPP

#include "utils.hpp"
#include "Cell.hpp"
#include "Piece.hpp"

#include <vector>
#include <utility>

class Board
{
public:
	Board();
	~Board();

    void doTest(); //TODO REMOVE THIS

    bool movePiece(move move);

    bool isValidMove(move m, int current_player);

    move askNextValidMove();

    void doMove(move);

    void fillAllMoves(int row, int col, std::vector<move> * moveList);

    bool gameFinished();

    void main_loop();
	void print();

private:
    void removePiece(Piece * p);
    bool checkCellAddMove(BoardCell* src_cell, int row, int col, int player, CellType type, std::vector<move>* plist);

    Cell *** _plateau;                  // The Board containing the 8*8 BoardCells and the 4 CampCells
	std::vector<Piece*> _piecesW;		// The list of White Pieces
	std::vector<Piece*> _piecesB;		// The list of Black Pieces

    int _currentPlayer = 1;				// 1 if White, -1 if Black
    int _isWin = 0;						// 0 if not win, 1 if White wins, -1 if Black wins
};

#endif // BOARD_HPP

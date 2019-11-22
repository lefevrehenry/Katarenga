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
    Board(std::string & board_configuration, bool verbose);
	~Board();

    bool isValidMove(Move & m, int player);
    bool isValidMove(std::string & move_str, int player);

    Move askNextValidMove();

    void playMove(Move & move);
    void playMove(std::string & move_str);

    void fillAllMoves(int row, int col, std::vector<Move> * moveList);

    // Return 0 if not finished, 1 if White won, -1 if Black won.
    int gameFinished();

    int getCurrentPlayer(){ return _currentPlayer; }

    void main_loop();
	void print();

private:
    void removePiece(Piece * p);
    Cell * indexToCell(int cell_index);
    bool checkCellAddMove(BoardCell* src_cell, int row, int col, int player, CellType type, std::vector<Move>* plist);

    // Converts a string representation of a Move to a Move
    Move stringToMove(std::string & move_str);
    // Converts a Move to a string representation of that Move
    std::string moveToString(Move & move);

    Cell *** _plateau;                  // The Board containing the 8*8 BoardCells and the 4 CampCells
	std::vector<Piece*> _piecesW;		// The list of White Pieces
	std::vector<Piece*> _piecesB;		// The list of Black Pieces

    bool _verbose;                       // Whether to talk a lot or not
    int _currentPlayer = 1;				// 1 if White, -1 if Black
    int _isWin = 0;						// 0 if not win, 1 if White wins, -1 if Black wins
};

#endif // BOARD_HPP

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

public:
    std::string getBoard() const;
    void setBoard(const std::string& boardString);

public:
    bool isValidMove(const Move& m, int player);
    bool isValidMove(const std::string& move_str, int player);

    Move askNextValidMove();

    void playMove(const Move& move);
    void playMove(const std::string& move_str);

    // Fills the list with possible Moves of the Piece in the cell (row,col)
    void fillAllMoves(int row, int col, std::vector<Move>* moveList);

public:
    // Return 0 if not finished, 1 if White won, -1 if Black won.
    int gameFinished();
    // 1 if White, -1 if Black
    int getCurrentPlayer();

public:
    void main_loop();
	void print();

private:
    void removePiece(Piece* p);
    Cell* indexToCell(int cell_index);
    // Internal function to populate the vector of Moves, called in getAllMoves.
    // Returns false if the while loop calling this function has to break.
    bool checkCellAddMove(BoardCell* src_cell, int row, int col, int player, CellType type, std::vector<Move>* plist);

    // Converts a Move to a string representation of that Move
    std::string moveToString(const Move& move);
    // Converts a string representation of a Move to a Move
    Move stringToMove(const std::string& move_str);

private:
    Cell *** _plateau;                  // The Board containing the 8*8 BoardCells and the 4 CampCells
	std::vector<Piece*> _piecesW;		// The list of White Pieces
	std::vector<Piece*> _piecesB;		// The list of Black Pieces

//    bool _verbose;                      // Whether to talk a lot or not
    int _currentPlayer = 1;				// 1 if White, -1 if Black
    int _isWin = 0;						// 0 if not win, 1 if White wins, -1 if Black wins

};

#endif // BOARD_HPP

#ifndef BOARD_HPP
#define BOARD_HPP

#include "server_utils.hpp"
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
    const std::string getBoardConfiguration() const;
    void setBoardCellTypes(const std::string& boardString);

public:
    bool isValidMove(const Move& m, int player) const;
    bool isValidMove(const std::string& move_str, int player) const;
    bool isValidMove(int src, int dest, int player) const;

    Move askNextValidMove() const;

    bool playMove(const Move& move);
    bool playMove(const std::string& move_str);
    bool playMove(int src, int dest);

    // Fills the list with possible Moves of the Piece in the cell (row,col)
    void fillAllMoves(int row, int col, std::vector<Move>* moveList) const;

public:
    // Return 0 if not finished, 1 if White won, -1 if Black won.
    int whoWon() const;
    bool isGameFinished() const;
    void updateGameFinished();
    int getCurrentPlayer() const;

public:
    //void main_loop();
    void print() const;

private:
    void removePiece(Piece* p);
    Cell* indexToCell(int cell_index) const;
    // Internal function to populate the vector of Moves, called in getAllMoves.
    // Returns false if the while loop calling this function has to break.
    bool checkCellAddMove(BoardCell* src_cell, int row, int col, int player, CellType type, std::vector<Move>* plist) const;

    // Converts a Move to a string representation of that Move
    std::string moveToString(const Move& move) const;
    // Converts a string representation of a Move to a Move
    Move stringToMove(const std::string& move_str) const;

    Move indicesToMove(int src, int dest) const;

private:
    Cell *** _plateau;                  // The Board containing the 8*8 BoardCells and the 4 CampCells
	std::vector<Piece*> _piecesW;		// The list of White Pieces
	std::vector<Piece*> _piecesB;		// The list of Black Pieces

//    bool _verbose;                      // Whether to talk a lot or not
    int _currentPlayer = 1;			      // 1 if White, -1 if Black
    int _winningPlayer = 0;				  // 0 if not win, 1 if White wins, -1 if Black wins
    bool _gameFinished = false;

};

#endif // BOARD_HPP

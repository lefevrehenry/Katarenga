#ifndef PIECE_HPP
#define PIECE_HPP

#include "utils.hpp"
#include <vector>

class Cell;

class Piece
{
public:
	Piece() {}
    Piece(int player,
          Cell * cell,
          std::vector<Move> * moveList):
	    _player(player),
        _moveList(moveList),
        _cell(cell) { _cell->setPiece(this);}
    ~Piece() {delete _moveList; }

    int getPlayer() const { return _player; }
    std::vector<Move>* getMoveList() { return _moveList; }
	Cell * getCell() const { return _cell; }
	void setCell(Cell * cell) { _cell = cell; }



private:
    int _player;
	Cell * _cell;
    std::vector<Move> * _moveList;
};

#endif // PIECE_HPP

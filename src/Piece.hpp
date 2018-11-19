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
          std::vector<move> * moveList):
	    _player(player),
        _moveList(moveList),
        _cell(cell) { _cell->setPiece(this);}
    ~Piece() {delete _moveList; }

    int getPlayer() const { return _player; }
    std::vector<move>* getMoveList() { return _moveList; }
	Cell * getCell() const { return _cell; }
	void setCell(Cell * cell) { _cell = cell; }



private:
    int _player;
	Cell * _cell;
    std::vector<move> * _moveList;
};

#endif // PIECE_HPP

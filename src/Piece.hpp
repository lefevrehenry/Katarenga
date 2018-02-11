#ifndef PIECE_HPP
#define PIECE_HPP

//#include "Cell.hpp"

class Cell;

class Piece
{
public:
	Piece() {}
	Piece(int player,
	      Cell * cell):
	    _player(player),
	    _cell(cell) { _cell->setPiece(this); }

	int getPlayer() const { return _player; }
	Cell * getCell() const { return _cell; }
	void setCell(Cell * cell) { _cell = cell; }


private:
	int _player;
	Cell * _cell;
};

#endif // PIECE_HPP

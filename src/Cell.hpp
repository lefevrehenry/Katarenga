#ifndef CELL_HPP
#define CELL_HPP

//#include "Piece.hpp"

class Piece;


enum class CellType
{
	KING, ROCK, BISHOP, KNIGHT
};


class Cell
{
public:
	Cell() {}
	virtual ~Cell() {}

	bool isEmpty() const { return _piece == nullptr; } // Tells whether the cell is empty or not

	Piece * getPiece() const { return _piece; }
	void setPiece(Piece *piece) { _piece = piece; }

	virtual bool isCampCell() = 0;
protected:
	Piece * _piece = nullptr; // Points to the Piece on this Cell, or nullptr
};



// Basic cell of the board.
class BoardCell : public Cell
{
public:
	BoardCell() {}
	/*BoardCell(CellType type,	// Not used if we declare the 'plateau' as a static table
		 int row,
		 int column):
		_type(type),
		_row(row),
		_column(column),
		_index(8*row+column){}*/

	void init(CellType type,
	          int row,
	          int column)
	{
		_type = type;
		_row = row;
		_column = column;
		_index = 8*row * column;
	}

	CellType getType() const { return _type; }

	int getIndex() const { return _index; }

	bool isCampCell() { return false; }

private:
	CellType _type;				// Type of the BoardCell
	int _row;					// Row of this BoardCell on the board
	int _column;				// Column of this BoardCell on the board
	int _index;					// Index of this BoardCell, equals to 8*row + column
};


// Special cell of the board, for the camp cells of the players
class CampCell : public Cell
{
public:
	CampCell() {}
	/*CampCell(int index):	// Not used if we declare the CampCells statically in Board constructor
		_index(index){}*/

	void init(int index){ _index = index; }

	bool isEmpty(){ return _piece == nullptr; }	// Tells whether the CampCell is occupied or not

	bool isCampCell() { return true; }

private:
	int _index;					// The index of the CampCell. Either 1 or 2 for White player; -1 or -2 for Black player.
};

#endif // CELL_HPP

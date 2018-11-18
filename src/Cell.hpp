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
    Cell(int row, int column):
        _row(row),
        _column(column){}
	virtual ~Cell() {}

	bool isEmpty() const { return _piece == nullptr; } // Tells whether the cell is empty or not

	Piece * getPiece() const { return _piece; }
	void setPiece(Piece *piece) { _piece = piece; }

    int getIndex() const { return _index; }
    int getRow() const { return _row; }
    int getColumn() const { return _column; }

	virtual bool isCampCell() = 0;
protected:
	Piece * _piece = nullptr; // Points to the Piece on this Cell, or nullptr
    int _row;       // Row of this Cell
    int _column;    // Column of this Cell
    int _index;     // Index of this Cell, equals to 8*row+column-1 if BoardCell
                    // 1 or 2 for a White CampCell, -1 or -2 for a Black CampCell
};



// Basic cell of the board.
class BoardCell : public Cell
{
public:
	BoardCell() {}
    BoardCell(CellType type, int row, int column):
        Cell(row,column),
        _type(type)
        {_index = 8*row+column-1;}

	CellType getType() const { return _type; }

	bool isCampCell() { return false; }

private:
	CellType _type;				// Type of the BoardCell
};


// Special cell of the board, for the camp cells of the players
class CampCell : public Cell
{
public:
	CampCell() {}
    CampCell(int row, int column, int index):
        Cell(row, column){_index = index;}

	bool isEmpty(){ return _piece == nullptr; }	// Tells whether the CampCell is occupied or not

	bool isCampCell() { return true; }
};

#endif // CELL_HPP

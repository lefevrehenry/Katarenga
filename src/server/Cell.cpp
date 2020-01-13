#include "Cell.hpp"

Cell::Cell(int row, int column) :
    _row(row),
    _column(column)
{
}

Cell::~Cell()
{
}

bool Cell::isEmpty() const
{
    return _piece == nullptr;
}

Piece *Cell::getPiece() const
{
    return _piece;
}

void Cell::setPiece(Piece* piece)
{
    _piece = piece;
}

int Cell::getIndex() const
{
    return _index;
}

int Cell::getRow() const
{
    return _row;
}

int Cell::getColumn() const
{
    return _column;
}

BoardCell::BoardCell(CellType type, int row, int column) :
    Cell(row,column),
    _type(type)
{
    _index = 8 * row + column - 1;
}

CellType BoardCell::getType() const
{
    return _type;
}

void BoardCell::setType(CellType type)
{
    _type = type;
}

bool BoardCell::isCampCell()
{
    return false;
}

CampCell::CampCell(int row, int column, int index) :
    Cell(row, column)
{
    _index = index;
}

bool CampCell::isCampCell()
{
    return true;
}

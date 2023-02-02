#include "Cell.hpp"
#include "Piece.hpp"

Cell::Cell() :
    type(),
    piece(nullptr),
    row(),
    column(),
    index()
{
}

Cell::Cell(Type _type, int _row, int _column) :
    type(_type),
    piece(nullptr),
    row(_row),
    column(_column),
    index((row * 8) + column)
{
}

bool Cell::isEmpty() const
{
    return piece == nullptr;
}

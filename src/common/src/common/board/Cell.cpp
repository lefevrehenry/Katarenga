#include "Cell.hpp"

Cell::Cell(Type _type, int _row, int _column) :
    type(_type),
    player(GameActor::None),
    row(_row),
    column(_column),
    index((row * 8) + column)
{
}

bool Cell::isEmpty() const
{
    return player == GameActor::None;
}

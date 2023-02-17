#include "Cell.hpp"

Cell::Cell(Common::Case _c, Type _type) :
    c(_c),
    type(_type),
    actor(GameActor::None)
//    row(_row),
//    column(_column),
//    index((row * 8) + column)
{
}

bool Cell::isEmpty() const
{
    return actor == GameActor::None;
}

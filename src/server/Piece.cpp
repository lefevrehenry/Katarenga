#include "Piece.hpp"
#include "Cell.hpp"

Piece::Piece(int player, Cell* cell) :
    _player(player),
    _cell(cell)
{
    _cell->setPiece(this);
}

Piece::~Piece()
{

}

int Piece::getPlayer() const
{
    return _player;
}

Cell* Piece::getCell() const
{
    return _cell;
}

void Piece::setCell(Cell* cell)
{
    _cell = cell;
}

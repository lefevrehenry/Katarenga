#include "Piece.hpp"
#include "Cell.hpp"

Piece::Piece(int player, Cell* cell) :
    _player(player),
    _cell(cell),
    _moveList(new std::vector<Move>())
{
    _cell->setPiece(this);
}

Piece::~Piece()
{
    delete _moveList;
}

int Piece::getPlayer() const
{
    return _player;
}

std::vector<Move>* Piece::getMoveList()
{
    return _moveList;
}

Cell* Piece::getCell() const
{
    return _cell;
}

void Piece::setCell(Cell* cell)
{
    _cell = cell;
}

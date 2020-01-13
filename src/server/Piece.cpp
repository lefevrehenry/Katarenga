#include "Piece.hpp"
#include "Cell.hpp"

Piece::Piece()
{
}

Piece::Piece(int player, Cell* cell, std::vector<Move>* moveList) :
    _player(player),
    _cell(cell),
    _moveList(moveList)
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

std::vector<Move> *Piece::getMoveList()
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

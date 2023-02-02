#include "Piece.hpp"

// Katarenga
#include <common/board/Cell.hpp>

Piece::Piece(Common::GameActor _player, Cell* _cell) :
    player(_player),
    cell(_cell)
{
    _cell->piece = this;
}

#ifndef KATARENGA_COMMON_BOARD_PIECE_HPP
#define KATARENGA_COMMON_BOARD_PIECE_HPP

// Katarenga
#include <common/board/board_utils.hpp>
#include <common/common_utils.hpp>

class Cell;

/**
 * @brief The Piece struct
 */
struct Piece
{

public:
    Piece(Common::GameActor player, Cell* cell);

public:
    Common::GameActor player;
    Cell* cell;

};

#endif // KATARENGA_COMMON_BOARD_PIECE_HPP

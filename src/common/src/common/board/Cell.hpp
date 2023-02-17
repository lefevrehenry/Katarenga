#ifndef KATARENGA_COMMON_BOARD_CELL_HPP
#define KATARENGA_COMMON_BOARD_CELL_HPP

// Katarenga
#include <common/common_utils.hpp>

/**
 * @brief The Cell class
 */
struct Cell
{
    using GameActor = Common::GameActor;

public:
    using Type = Common::CellType;

public:
    Cell() = default;
    Cell(Common::Case c, Type type);

public:
    // Tells whether the cell is is occupied by a player or not
    bool isEmpty() const;

public:
    bool operator==(const Cell& other) const { return c == other.c; }
    operator bool() const { return !isEmpty(); }

public:
    bool isCampCell() const { return !(case_index(c) >= 0 && case_index(c) < 64); }

public:
    Common::Case c;     // Case of the Cell
    Type type;         // Type of the Cell
    GameActor actor;   // Points to the Piece on this Cell, or nullptr
//    int row;           // Row of this Cell
//    int column;        // Column of this Cell
//    int index;         // Index of this Cell, equals to (8 * row) + column

};

#endif // KATARENGA_COMMON_BOARD_CELL_HPP

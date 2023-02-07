#ifndef KATARENGA_COMMON_BOARD_CELL_HPP
#define KATARENGA_COMMON_BOARD_CELL_HPP

// Katarenga
#include <common/common_utils.hpp>

// Base class for all cells of the Board
struct Cell
{
    using GameActor = Common::GameActor;

public:
    enum class Type
    {
        None,
        King,
        Rook,
        Bishop,
        Knight,
        CampCell
    };

public:
    Cell() = default;
    Cell(Type type, int row, int column);

public:
    // Tells whether the cell is is occupied by a player or not
    bool isEmpty() const;

public:
    bool operator==(const Cell& other) const { return row == other.row && column == other.column; }
    operator bool() const { return !isEmpty(); }

public:
    bool isCampCell() const { return !(index >= 0 && index < 64); }

public:
    Type type;         // Type of the Cell
    GameActor player;  // Points to the Piece on this Cell, or nullptr
    int row;           // Row of this Cell
    int column;        // Column of this Cell
    int index;         // Index of this Cell, equals to (8 * row) + column

};

#endif // KATARENGA_COMMON_BOARD_CELL_HPP

#ifndef BOARD_UTILS_HPP
#define BOARD_UTILS_HPP

#include "Cell.hpp"

#include <string>

using Move = std::pair<const Cell&,const Cell&>;

class Board;

enum class BoardPlayer {
    None,
    White,
    Black
};

BoardPlayer otherPlayer(const BoardPlayer &player);

std::string to_string(const BoardPlayer &player);

// Generates a Sring containing the cell types of the board, row by row.
std::string generateBoardCellTypes();

std::string format_board(const std::string& board_configuration);

void print_board(const Board* board);

#endif // BOARD_UTILS_HPP

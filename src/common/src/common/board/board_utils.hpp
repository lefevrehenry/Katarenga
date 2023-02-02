#ifndef KATARENGA_COMMON_BOARD_BOARD_UTILS_HPP
#define KATARENGA_COMMON_BOARD_BOARD_UTILS_HPP

// Katarenga
#include <common/common_utils.hpp>

// Standard Library
#include <string>

//using Move = std::pair<const Cell&,const Cell&>;

class Board;

Common::GameActor otherPlayer(const Common::GameActor& player);

std::string to_string(const Common::GameActor& player);

// Generates a Sring containing the cell types of the board, row by row.
std::string generateBoardCellTypes();

std::string format_board(const std::string& board_configuration);

#endif // KATARENGA_COMMON_BOARD_BOARD_UTILS_HPP

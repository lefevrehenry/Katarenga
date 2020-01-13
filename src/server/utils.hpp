#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

class Board;
class Cell;

typedef std::pair<Cell*,Cell*> Move;

// Generates a Sring containing the cell types of the board, row by row.
void generateBoard(Board* board);

#endif // UTILS_HPP

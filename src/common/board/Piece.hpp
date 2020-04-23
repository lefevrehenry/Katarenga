#ifndef PIECE_HPP
#define PIECE_HPP

#include "board_utils.hpp"

class Cell;

class Piece
{

public:
    Piece(BoardPlayer player, Cell* cell);
    virtual ~Piece();

public:
    BoardPlayer getPlayer() const;

    Cell* getCell() const;
    void setCell(Cell* cell);

private:
    BoardPlayer _player;
    Cell* _cell;

};

#endif // PIECE_HPP

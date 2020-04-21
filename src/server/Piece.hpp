#ifndef PIECE_HPP
#define PIECE_HPP

#include <vector>

class Cell;

class Piece
{

public:
    Piece(int player, Cell* cell);
    virtual ~Piece();

public:
    int getPlayer() const;

    Cell* getCell() const;
    void setCell(Cell* cell);

private:
    int _player;
    Cell* _cell;

};

#endif // PIECE_HPP

#ifndef KATARENGA_COMMON_BOARD_BOARD_HPP
#define KATARENGA_COMMON_BOARD_BOARD_HPP

//Katarenga
#include <common/board/board_utils.hpp>
#include <common/board/Cell.hpp>
#include <common/common_utils.hpp>

// Standard Library
#include <string>
#include <vector>
#include <map>

class Piece;

/**
 * @brief The Board class
 */
class Board
{
    using BoardMove = std::pair<Cell*,Cell*>;

public:
    Board();
    virtual ~Board();

public:
    const std::string getBoardConfiguration() const;
    void setBoardCellTypes(const std::string& boardString);

public:
    bool isValidMove(const Common::Move& m, Common::GameActor player) const;

    bool playMove(const Common::Move& move);

    // Fills the list with possible Moves of the Piece in the cell (row,col)
    std::vector<Common::Move> findAllMoves(int row, int col) const;

public:
    Common::GameActor getCurrentPlayer() const;
    void updateGameFinished();
    bool isGameFinished() const;
    Common::GameActor whoWon() const;

private:
    void nextPlayer();
    void removePiece(Piece* p);
    const Cell& indexToCell(int cell_index) const;
    Cell& indexToCell(int cell_index);
//    const CampCell& indexToCampCell(int campcell_index) const;
//    CampCell& indexToCampCell(int campcell_index);

private:
    BoardMove boardMoveFromMove(const Common::Move& move);

private:
    Cell                _plateau[8][8];
    std::array<Cell,4>  _campCell;

    std::vector<Piece*> _piecesW;       // The list of White Pieces
    std::vector<Piece*> _piecesB;		// The list of Black Pieces

    bool _gameFinished;

    Common::GameActor _currentPlayer;
    Common::GameActor _winningPlayer;
};

#endif // KATARENGA_COMMON_BOARD_BOARD_HPP

#ifndef KATARENGA_COMMON_BOARD_BOARD_HPP
#define KATARENGA_COMMON_BOARD_BOARD_HPP

// Katarenga
#include <common/board/board_utils.hpp>
#include <common/board/Cell.hpp>
#include <common/common_utils.hpp>

// Standard Library
#include <string>
#include <vector>
#include <map>

/**
 * @brief The Board class
 */
class Board
{

public:
    Board();

public:
    const Common::Position getBoardConfiguration() const;
    void setFromBoardConfiguration(const Common::Position& configuration);

    void setBoardCellTypes(const std::string& boardString);
    void clear();

public:
    bool isValidMove(const Common::Move& m, Common::GameActor player) const;

    bool playMove(const Common::Move& move);

private:
    // Fills the list with possible Moves of the Piece in the cell (row,col)
    std::vector<Common::Move> findAllMoves(Common::Case c) const;

public:
    Common::GameActor getCurrentPlayer() const;
    void updateGameFinished();
    bool isGameFinished() const;
    Common::GameActor whoWon() const;

private:
    void nextPlayer();
    void removePiece(const Cell& cell);

    Cell& indexToCell(Common::Case c);
    const Cell& indexToCell(Common::Case c) const;

private:
    Cell               _plateau[8][8];
    std::array<Cell,4> _campCell;

    std::vector<Cell*> _piecesW;        // The list of white cells
    std::vector<Cell*> _piecesB;        // The list of black cells

    Common::GameActor _currentPlayer;
    Common::GameActor _winningPlayer;
};

#endif // KATARENGA_COMMON_BOARD_BOARD_HPP

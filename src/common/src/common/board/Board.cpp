﻿#include "Board.hpp"

// Standard Library
#include <algorithm>
#include <iostream>

using Case = Common::Case;
using Move = Common::Move;
using GameActor = Common::GameActor;

Board::Board() :
    _plateau(),
    _campCell(),
    _piecesW(),
    _piecesB(),
    _currentPlayer(GameActor::White),
    _winningPlayer(GameActor::None)
{
    // Initialization of the Cells
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            _plateau[row][col] = Cell(new_case(row,col), Cell::Type::None);
        }
    }

    // Initialization of the CampCells
    Cell::Type type = Cell::Type::CampCell;
    _campCell = { Cell(new_case(-1,0),type), Cell(new_case(-1,7),type), Cell(new_case(8,0),type), Cell(new_case(8,7),type) };

    // Initialization of Black and White Pieces
    for(int col = 0; col < 8; ++col)
    {
        // For White
        _plateau[0][col].actor = GameActor::White;
        _piecesW.push_back(&_plateau[0][col]);

        // For Black
        _plateau[7][col].actor = GameActor::Black;
        _piecesB.push_back(&_plateau[7][col]);
    }
}

/* Returns the current configuration of the board with for each cell
 * its type and whether there is a piece on that cell,
 * for each camp cell whether there is a piece on that cell,
 * the current player and whether the game is finished or not
 */
const Common::Position Board::getBoardConfiguration() const
{
    Common::Position s = "";

    // Populate the cells of the board
    for(int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            const Cell& cell = _plateau[row][col];

            char c = from_cell_type(cell.type);

            s += (c != char() ? c : ' ');

            if(cell)
                s += (cell.actor == GameActor::White ? "+" : "-");
            else
                s += " ";
        }

        s += "\n";
    }

    const Cell& whiteLeft = _campCell[0];
    const Cell& whiteRight = _campCell[1];
    const Cell& blackLeft = _campCell[2];
    const Cell& blackRight = _campCell[3];

    // Populate the camp cells
    s += (whiteLeft ? "-" : " ");
    s += (whiteRight ? "-" : " ");
    s += (blackLeft ? "+" : " ");
    s += (blackRight ? "+" : " ");

    // Then put the current player and whether the game is finished
    s += (_currentPlayer == GameActor::White ? "+" : "-");

    if(isGameFinished())
        s += (whoWon() == GameActor::White ? "+" : "-");
    else
        s += " ";

    return s;
}

void Board::setFromBoardConfiguration(const Common::Position& position)
{
    clear();

    if(!has_valid_format(position))
        throw std::runtime_error("bad position format");

    // Plateau
    {
        yield_cases(position, [&](Case c, Cell::Type type, GameActor actor) {
            Cell& cell = indexToCell(c);

            cell.type = type;
            cell.actor = actor;

            switch(actor)
            {
            case GameActor::White:
                _piecesW.push_back(&cell);
                break;
            case GameActor::Black:
                _piecesB.push_back(&cell);
                break;
            default:
                break;
            }
        });
    }

    // Camp Cells
    {
        yield_camp_cells(position, [&](Case c, GameActor actor) {
            Cell& cell = indexToCell(c);

            cell.actor = actor;
        });
    }

    // Current Player
    {
        yield_current_player(position, [&](GameActor actor) {
            _currentPlayer = actor;
        });
    }

    // Winning Player
    {
        yield_winning_player(position, [&](GameActor actor) {
            _winningPlayer = actor;
        });
    }
}

void Board::setBoardCellTypes(const std::string& boardString)
{
    int index = 0;

    // Iterates over rows
    for(int row = 0; row < 8; ++row)
    {
        // Iterates over columns
        for(int col = 0; col < 8; ++col)
        {
            Cell& cell = _plateau[row][col];
            const char charType = boardString.at(index);

            switch (charType)
            {
            case 'K':
                cell.type = Cell::Type::King;
                break;
            case 'R':
                cell.type = Cell::Type::Rook;
                break;
            case 'B':
                cell.type = Cell::Type::Bishop;
                break;
            case 'N':
                cell.type = Cell::Type::Knight;
                break;
            default:
                cell.type = Cell::Type::None;
            }
            index++;
        }
    }

    updateGameFinished();
}

void Board::clear()
{
    _piecesW.clear();
    _piecesB.clear();

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            Cell& cell = _plateau[row][col];
            cell.actor = GameActor::None;
        }
    }

    Cell& whiteLeft = _campCell[0];
    Cell& whiteRight = _campCell[1];
    Cell& blackLeft = _campCell[2];
    Cell& blackRight = _campCell[3];

    whiteLeft.actor = GameActor::None;
    whiteRight.actor = GameActor::None;
    blackLeft.actor = GameActor::None;
    blackRight.actor = GameActor::None;
}

bool Board::isValidMove(const Move& m, GameActor player) const
{
    Case from = move_from(m);
    Case to = move_to(m);

    const Cell& from_cell = indexToCell(from);
    const Cell& to_cell = indexToCell(to);

    if (from_cell.actor != player)
        return false;

    if (to_cell.actor == player)
        return false;

    std::vector<Move> moves = findAllMoves(from);

    for (const Move& move: moves) {
        Case _to = move_to(move);
        const Cell& target_cell = indexToCell(_to);
        if (target_cell == to_cell)
            return true;
    }

    return false;
}

bool Board::playMove(const Move& move)
{
    if(isGameFinished())
        return false;

    if (!isValidMove(move, getCurrentPlayer()))
        return false;

    Case from = move_from(move);
    Case to = move_to(move);

    Cell& src_cell = indexToCell(from);
    Cell& dst_cell = indexToCell(to);
    GameActor player = src_cell.actor;

    if(dst_cell)
        removePiece(dst_cell);

    dst_cell.actor = player;
    src_cell.actor = GameActor::None;

    nextPlayer();

    updateGameFinished();

    return true;
}

std::vector<Move> Board::findAllMoves(Case c) const
{
    std::vector<Move> moves;

    int row = case_row(c);
    int col = case_col(c);

    const Cell& cell = _plateau[row][col];

    if (!cell || cell.isCampCell())
        return moves;

    GameActor player = cell.actor;

    const Cell& whiteLeft = _campCell[0];
    const Cell& whiteRight = _campCell[1];
    const Cell& blackLeft = _campCell[2];
    const Cell& blackRight = _campCell[3];

    // Check if the piece can go to an adversary camp
    if (row == 0 && player == GameActor::Black)
    {
        if(!whiteLeft)
            moves.push_back(Move(cell.c, whiteLeft.c));

        if(!whiteRight)
            moves.push_back(Move(cell.c, whiteRight.c));
    }
    if (row == 7 && player == GameActor::White)
    {
        if(!blackLeft)
            moves.push_back(Move(cell.c, blackLeft.c));

        if(!blackRight)
            moves.push_back(Move(cell.c, blackRight.c));
    }

    switch(cell.type)
    {
    case Cell::Type::King:
    {
        // all deltas move from a King cell (check cells directly around)
        static const std::vector< std::pair<int,int> > deltas = {
            {-1,-1}, {-1, 0}, {-1, 1}, { 0,-1}, { 0, 1}, { 1,-1}, { 1, 0}, { 1, 1}
        };

        for (const std::pair<int,int>& delta : deltas)
        {
            int new_row = row + delta.first;
            int new_col = col + delta.second;

            if((new_row < 0) || (new_row > 7) || (new_col < 0) || (new_col > 7))
                continue;

            const Cell& new_cell = _plateau[new_row][new_col];
            if(!new_cell || new_cell.actor == otherPlayer(player))
                moves.push_back(Move(cell.c, new_cell.c));
        }
        break;
    }

    case Cell::Type::Bishop:
    {
        // all direction move from a Bishop cell (check cells in diagonal until next edge or Bishop cell or oponnent)
        static const std::vector< std::pair<int,int> > directions = {
            {-1,-1}, {-1, 1}, { 1,-1}, { 1, 1}
        };

        for (const std::pair<int,int> direction : directions)
        {
            int delta_row = direction.first;
            int delta_col = direction.second;

            int new_row = row + delta_row;
            int new_col = col + delta_col;

            while(new_row >= 0 && new_row <= 7 && new_col >= 0 && new_col <= 7) {
                const Cell& new_cell = _plateau[new_row][new_col];

                if(!new_cell)
                {
                    moves.push_back(Move(cell.c, new_cell.c));
                    if(new_cell.type == Cell::Type::Bishop)
                        break;
                }
                else
                {
                    if(new_cell.actor == otherPlayer(player))
                        moves.push_back(Move(cell.c, new_cell.c));

                    break;
                }

                new_row += delta_row;
                new_col += delta_col;
            }
        }
        break;
    }

    case Cell::Type::Rook:
    {
        // all direction move from a Rook cell (check cells in diagonal until next edge or Bishop cell or oponnent)
        static const std::vector< std::pair<int,int> > directions = {
            {-1, 0}, { 1, 0}, { 0,-1}, { 0, 1}
        };

        for (const std::pair<int,int> direction : directions)
        {
            int delta_row = direction.first;
            int delta_col = direction.second;

            int new_row = row + delta_row;
            int new_col = col + delta_col;

            while(new_row >= 0 && new_row <= 7 && new_col >= 0 && new_col <= 7) {
                const Cell& new_cell = _plateau[new_row][new_col];

                if(!new_cell)
                {
                    moves.push_back(Move(cell.c, new_cell.c));
                    if(new_cell.type == Cell::Type::Rook)
                        break;
                }
                else
                {
                    if(new_cell.actor == otherPlayer(player))
                        moves.push_back(Move(cell.c, new_cell.c));

                    break;
                }

                new_row += delta_row;
                new_col += delta_col;
            }
        }
        break;
    }

    case Cell::Type::Knight:
    {
        // all deltas move from a Knight cell
        static const std::vector< std::pair<int,int> > deltas = {
            {-1,-2}, { 1,-2}, {-1, 2}, { 1, 2}, {-2,-1}, { 2,-1}, {-2, 1}, { 2, 1}
        };

        for (const std::pair<int,int>& delta : deltas)
        {
            int new_row = row + delta.first;
            int new_col = col + delta.second;

            if((new_row < 0) || (new_row > 7) || (new_col < 0) || (new_col > 7))
                continue;

            const Cell& new_cell = _plateau[new_row][new_col];
            if(!new_cell || new_cell.actor == otherPlayer(player))
                moves.push_back(Move(cell.c, new_cell.c));
        }
        break;
    }

    default:
        break;
    }

    return moves;
}

void Board::updateGameFinished()
{
    const Cell& whiteLeft = _campCell[0];
    const Cell& whiteRight = _campCell[1];
    const Cell& blackLeft = _campCell[2];
    const Cell& blackRight = _campCell[3];

    // Check if White won
    if ((blackLeft && blackRight) || _piecesB.size() == 0)
    {
        _winningPlayer = GameActor::White;
    }

    // Check if Black won
    else if ((whiteLeft && whiteRight) || _piecesW.size() == 0)
    {
        _winningPlayer = GameActor::Black;
    }
}

bool Board::isGameFinished() const
{
    return _winningPlayer != GameActor::None;
}

GameActor Board::whoWon() const
{
    return _winningPlayer;
}

GameActor Board::getCurrentPlayer() const
{
    return _currentPlayer;
}

void Board::nextPlayer()
{
    _currentPlayer = (_currentPlayer == GameActor::White) ? GameActor::Black : GameActor::White;
}

void Board::removePiece(const Cell& cell)
{
    std::vector<Cell*> * pieces = cell.actor == GameActor::White ? &_piecesW : &_piecesB;

    auto it = std::find(pieces->begin(), pieces->end(), &cell);
    if (it != pieces->end())
        pieces->erase(it);
}

Cell& Board::indexToCell(Case c)
{
    int index = case_index(c);

    if(index >= 0 || index < 64)
    {
        int row = case_row(c);
        int col = case_col(c);

        return _plateau[row][col];
    }

    if(index == -8)
        return _campCell[0];
    else if(index == -1)
        return _campCell[1];
    else if(index == 64)
        return _campCell[2];
    else if(index == 71)
        return _campCell[3];

    throw std::runtime_error("no Cell defined for index " + std::to_string(index));
}

const Cell& Board::indexToCell(Case c) const
{
    int index = case_index(c);

    if(index >= 0 || index < 64)
    {
        int row = case_row(c);
        int col = case_col(c);

        return _plateau[row][col];
    }

    if(index == -8)
        return _campCell[0];
    else if(index == -1)
        return _campCell[1];
    else if(index == 64)
        return _campCell[2];
    else if(index == 71)
        return _campCell[3];

    throw std::runtime_error("no Cell defined for index " + std::to_string(index));
}

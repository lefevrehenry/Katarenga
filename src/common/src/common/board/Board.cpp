#include "Board.hpp"
#include "Piece.hpp"

#include <iostream>
#include <algorithm>

using Move = Common::Move;
using GameActor = Common::GameActor;

Board::Board() :
    _plateau(),
    _campCell(),
    _piecesW(8),
    _piecesB(8),
    _gameFinished(false),
    _currentPlayer(GameActor::White),
    _winningPlayer(GameActor::None)
{
    // Initialization of the Cells
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            _plateau[row][col] = Cell(Cell::Type::None,row,col);
        }
    }

    // Initialization of the CampCells
    Cell::Type type = Cell::Type::CampCell;
    _campCell = { Cell(type,-1,0), Cell(type,-1,7), Cell(type,8,0), Cell(type,8,7) };

	// Initialization of Black and White Pieces
    for(int col = 0; col < 8; ++col)
	{
        // For White
        Piece* pieceW = new Piece(GameActor::White, &_plateau[0][col]);
		_piecesW.push_back(pieceW);

        // For Black
        Piece* pieceB = new Piece(GameActor::Black, &_plateau[7][col]);
		_piecesB.push_back(pieceB);
	}
}

Board::~Board()
{
    for(size_t i = 0; i < _piecesW.size(); ++i)
	{
        delete _piecesW[i];
		_piecesW[i] = nullptr;
	}
    _piecesW.clear();
    for(size_t i = 0; i < _piecesB.size(); ++i)
	{
        delete _piecesB[i];
		_piecesB[i] = nullptr;
	}
    _piecesB.clear();
}

/* Returns the current configuration of the board with for each cell
 * its type and whether there is a piece on that cell,
 * for each camp cell whether there is a piece on that cell,
 * the current player and whether the game is finished or not
 */
const std::string Board::getBoardConfiguration() const
{
    std::string s = "";

    // Populate the cells of the board
    for(int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            const Cell& cell = _plateau[row][col];
            switch(cell.type)
            {
            case Cell::Type::King:
                s += "K";
                break;
            case Cell::Type::Bishop:
                s += "B";
                break;
            case Cell::Type::Knight:
                s += "N";
                break;
            case Cell::Type::Rook:
                s += "R";
                break;
            default:
                s += " ";
            }

            Piece* piece = cell.piece;
            if(piece)
                s += (piece->player == GameActor::White ? "+" : "-");
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
    s += (whiteLeft.isEmpty() ? " " : "-");
    s += (whiteRight.isEmpty() ? " " : "-");
    s += (blackLeft.isEmpty() ? " " : "+");
    s += (blackRight.isEmpty() ? " " : "+");

    // Then put the current player and whether the game is finished
    s += (_currentPlayer == GameActor::White ? "+" : "-");

    if(isGameFinished())
        s += (whoWon() == GameActor::White ? "+" : "-");
    else
        s += " ";

    return s;
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

bool Board::isValidMove(const Move& m, GameActor player) const
{
    int from = std::get<0>(m);
    int to = std::get<1>(m);

    const Cell& from_cell = indexToCell(from);
    const Cell& to_cell = indexToCell(to);

    if (from_cell.isEmpty() || from_cell.piece->player != player)
        return false;

    if (!to_cell.isEmpty() && to_cell.piece->player == player)
        return false;

    int src_row = from_cell.row;
    int src_col = from_cell.column;

    std::vector<Move> moves = findAllMoves(src_row, src_col);

    for (const Move& move: moves) {
        int _to = std::get<1>(move);
        const Cell& target_cell = indexToCell(_to);
        if (target_cell == to_cell)
            return true;
    }

    return false;
}

bool Board::playMove(const Move& move)
{
    if (!isValidMove(move, getCurrentPlayer()))
        return false;

    int from = std::get<0>(move);
    int to = std::get<1>(move);

    Cell& src_cell = indexToCell(from);
    Cell& dst_cell = indexToCell(to);
    Piece * piece = src_cell.piece;

    if(!dst_cell.isEmpty())
        removePiece(dst_cell.piece);

    dst_cell.piece = piece;
    src_cell.piece = nullptr;
    piece->cell = &dst_cell;
    nextPlayer();

    updateGameFinished();

    return true;
}

std::vector<Move> Board::findAllMoves(int row, int col) const
{
    std::vector<Move> moves;

    const Cell& cell = _plateau[row][col];

    if (cell.isEmpty() || cell.isCampCell())
        return moves;

    GameActor player = cell.piece->player;

    const Cell& whiteLeft = _campCell[0];
    const Cell& whiteRight = _campCell[1];
    const Cell& blackLeft = _campCell[2];
    const Cell& blackRight = _campCell[3];

    // Check if the piece can go to an adversary camp
    if (row == 0 && player == GameActor::Black)
    {
        if(whiteLeft.isEmpty())
            moves.push_back(Move(cell.index,whiteLeft.index));

        if(whiteRight.isEmpty())
            moves.push_back(Move(cell.index,whiteRight.index));
    }
    if (row == 7 && player == GameActor::White)
    {
        if(blackLeft.isEmpty())
            moves.push_back(Move(cell.index,blackLeft.index));

        if(blackRight.isEmpty())
            moves.push_back(Move(cell.index,blackRight.index));
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

            if(new_row < 0 || new_row > 7 || new_col < 0 || new_col > 7)
                continue;

            const Cell& new_cell = _plateau[new_row][new_col];
            if(new_cell.isEmpty() || new_cell.piece->player == otherPlayer(player))
                moves.push_back(Move(cell,new_cell));
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

                if(new_cell.isEmpty())
                {
                    moves.push_back(Move(cell,new_cell));
                    if(new_cell.type == Cell::Type::Bishop)
                        break;
                }
                else
                {
                    if(new_cell.piece->player == otherPlayer(player))
                        moves.push_back(Move(cell,new_cell));

                    break;
                }

                new_row += delta_row;
                new_col += delta_col;
            }
        }
        break;
    }

    case Cell::Type::Rook: // Check cells in straight line until Rook cell or edge or oponnent
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

                if(new_cell.isEmpty())
                {
                    moves.push_back(Move(cell,new_cell));
                    if(new_cell.type == Cell::Type::Rook)
                        break;
                }
                else
                {
                    if(new_cell.piece->player == otherPlayer(player))
                        moves.push_back(Move(cell,new_cell));

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

            if(new_row < 0 || new_row > 7 || new_col < 0 || new_col > 7)
                continue;

            const Cell& new_cell = _plateau[new_row][new_col];
            if(new_cell.isEmpty() || new_cell.piece->player == otherPlayer(player))
                moves.push_back(Move(cell,new_cell));
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
    if (!blackLeft.isEmpty() && !blackRight.isEmpty())
    {
        _winningPlayer = GameActor::White;
        _gameFinished = true;
    }

    // Check if Black won
    else if (!whiteLeft.isEmpty() && !whiteRight.isEmpty())
    {
        _winningPlayer = GameActor::Black;
        _gameFinished = true;
    }
}

bool Board::isGameFinished() const
{
    return _gameFinished;
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

void Board::removePiece(Piece* p)
{
    std::cout << "Outch! " << ((p->player == GameActor::White) ? "White" : "Black") << " just lost a Piece!" << std::endl;
    std::vector<Piece*> * list = p->player == GameActor::White ? &_piecesW : &_piecesB;

    auto it = std::find(list->begin(), list->end(), p);
    if (it != list->end())
        list->erase(it);

    delete p;
}

Cell& Board::indexToCell(int index)
{
    return indexToCell(index);
}

const Cell& Board::indexToCell(int index) const
{
    if(index >= 0 || index < 64)
    {
        int row = index / 8;
        int col = index % 8;

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

Board::BoardMove Board::boardMoveFromMove(const Common::Move& move)
{
    Cell* first = nullptr;
    Cell* second = nullptr;

    return {first, second};
}

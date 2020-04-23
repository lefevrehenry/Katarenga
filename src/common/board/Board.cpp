#include "Board.hpp"
#include "Piece.hpp"

#include <iostream>
#include <algorithm>

Board::Board() :
    _plateau(),
    _campCell(),
    _piecesW(8),
    _piecesB(8),
    _currentPlayer(BoardPlayer::White),
    _winningPlayer(BoardPlayer::None),
    _gameFinished(false)
{
    // Initialization of the BoardCells
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            _plateau[row][col] = BoardCell(CellType::NONE,row,col);
        }
    }

    // Initialization of the CampCells
    _campCell[CampCell::Type::WhiteLeft] = CampCell(-1,-1,64);
    _campCell[CampCell::Type::WhiteRight] = CampCell(-1,9,65);
    _campCell[CampCell::Type::BlackLeft] = CampCell(9,-1,66);
    _campCell[CampCell::Type::BlackRight] = CampCell(9,9,67);

	// Initialization of Black and White Pieces
    for(int col = 0; col < 8; ++col)
	{
        // For White
        Piece* pieceW = new Piece(BoardPlayer::White, &_plateau[0][col]);
		_piecesW.push_back(pieceW);

        // For Black
        Piece* pieceB = new Piece(BoardPlayer::Black, &_plateau[7][col]);
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
            const BoardCell& cell = _plateau[row][col];
            switch(cell.getType())
            {
            case CellType::KING:
                s += "K";
                break;
            case CellType::BISHOP:
                s += "B";
                break;
            case CellType::KNIGHT:
                s += "N";
                break;
            case CellType::ROOK:
                s += "R";
                break;
            default:
                s += " ";
            }

            Piece* piece = cell.getPiece();
            if(piece)
                s += (piece->getPlayer() == BoardPlayer::White ? "+" : "-");
            else
                s += " ";
        }
    }

    // Populate the camp cells
    s += (_campCell.at(CampCell::Type::WhiteLeft).isEmpty() ? " " : "-");
    s += (_campCell.at(CampCell::Type::WhiteRight).isEmpty() ? " " : "-");
    s += (_campCell.at(CampCell::Type::BlackLeft).isEmpty() ? " " : "+");
    s += (_campCell.at(CampCell::Type::BlackRight).isEmpty() ? " " : "+");

    // Then put the current player and whether the game is finished
    s += (_currentPlayer == BoardPlayer::White ? "+" : "-");

    if(isGameFinished())
        s += (whoWon() == BoardPlayer::White ? "+" : "-");
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
            BoardCell& cell = _plateau[row][col];
            const char charType = boardString.at(index);

            switch (charType)
            {
            case 'K':
                cell.setType(CellType::KING);
                break;
            case 'R':
                cell.setType(CellType::ROOK);
                break;
            case 'B':
                cell.setType(CellType::BISHOP);
                break;
            case 'N':
                cell.setType(CellType::KNIGHT);
                break;
            default:
                cell.setType(CellType::NONE);
            }
            index++;
        }
    }

    updateGameFinished();
}

bool Board::isValidMove(const Move& m, BoardPlayer player) const
{
    if (m.first.isEmpty() || m.first.getPiece()->getPlayer() != player)
        return false;

    if (!m.second.isEmpty() && m.second.getPiece()->getPlayer() == player)
        return false;

    int src_row = m.first.getRow();
    int src_col = m.first.getColumn();

    std::vector<Move> moves;
    fillAllMoves(src_row, src_col, &moves);

    for (const Move& move: moves) {
        if (move.second == m.second)
            return true;
    }

    return false;
}

bool Board::isValidMove(const std::string& move_str, BoardPlayer player) const
{
    Move m = stringToMove(move_str);
    return isValidMove(m, player);
}

bool Board::isValidMove(int src, int dest, BoardPlayer player) const
{
    Move m = indicesToMove(src, dest);
    return isValidMove(m, player);
}

bool Board::playMove(const Move& move)
{
    if (!isValidMove(move, getCurrentPlayer()))
        return false;

    Cell& src_cell = indexToBoardCell(move.first.getIndex());
    Cell& dst_cell = indexToBoardCell(move.second.getIndex());
    Piece * piece = src_cell.getPiece();

    if(!dst_cell.isEmpty())
        removePiece(dst_cell.getPiece());

    dst_cell.setPiece(piece);
    src_cell.setPiece(nullptr);
    piece->setCell(&dst_cell);
    nextPlayer();

    updateGameFinished();

    return true;
}

bool Board::playMove(const std::string& move_str)
{
    Move m = stringToMove(move_str);
    return playMove(m);
}

bool Board::playMove(int src, int dest)
{
    Move m = indicesToMove(src, dest);
    return playMove(m);
}

void Board::fillAllMoves(int row, int col, std::vector<Move>* list) const
{
    const BoardCell& cell = _plateau[row][col];

    if (cell.isEmpty() || cell.isCampCell())
        return;

    BoardPlayer player = cell.getPiece()->getPlayer();

    // Check if the piece can go to an adversary camp
    if (row == 0 && player == BoardPlayer::Black)
    {
        if(_campCell.at(CampCell::WhiteLeft).isEmpty())
            list->push_back(Move(cell,_campCell.at(CampCell::WhiteLeft)));

        if(_campCell.at(CampCell::WhiteRight).isEmpty())
            list->push_back(Move(cell,_campCell.at(CampCell::WhiteRight)));
    }
    if (row == 7 && player == BoardPlayer::White)
    {
        if(_campCell.at(CampCell::BlackLeft).isEmpty())
            list->push_back(Move(cell,_campCell.at(CampCell::BlackLeft)));

        if(_campCell.at(CampCell::BlackRight).isEmpty())
            list->push_back(Move(cell,_campCell.at(CampCell::BlackRight)));
    }

    switch(cell.getType())
    {
    case CellType::KING:
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

            const BoardCell& new_cell = _plateau[new_row][new_col];
            if(new_cell.isEmpty() || new_cell.getPiece()->getPlayer() == otherPlayer(player))
                list->push_back(Move(cell,new_cell));
        }
        break;
    }

    case CellType::BISHOP:
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
                const BoardCell& new_cell = _plateau[new_row][new_col];

                if(new_cell.isEmpty())
                {
                    list->push_back(Move(cell,new_cell));
                    if(new_cell.getType() == CellType::BISHOP)
                        break;
                }
                else
                {
                    if(new_cell.getPiece()->getPlayer() == otherPlayer(player))
                        list->push_back(Move(cell,new_cell));

                    break;
                }

                new_row += delta_row;
                new_col += delta_col;
            }
        }
        break;
    }

    case CellType::ROOK: // Check cells in straight line until Rook cell or edge or oponnent
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
                const BoardCell& new_cell = _plateau[new_row][new_col];

                if(new_cell.isEmpty())
                {
                    list->push_back(Move(cell,new_cell));
                    if(new_cell.getType() == CellType::BISHOP)
                        break;
                }
                else
                {
                    if(new_cell.getPiece()->getPlayer() == otherPlayer(player))
                        list->push_back(Move(cell,new_cell));

                    break;
                }

                new_row += delta_row;
                new_col += delta_col;
            }
        }
        break;
    }

    case CellType::KNIGHT:
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

            const BoardCell& new_cell = _plateau[new_row][new_col];
            if(new_cell.isEmpty() || new_cell.getPiece()->getPlayer() == otherPlayer(player))
                list->push_back(Move(cell,new_cell));
        }
        break;
    }

    default:
        break;
    }
}

void Board::updateGameFinished()
{
    // Check if White won
    if (!_campCell.at(CampCell::Type::BlackLeft).isEmpty() && !_campCell.at(CampCell::Type::BlackRight).isEmpty())
    {
        _winningPlayer = BoardPlayer::White;
        _gameFinished = true;
    }
    // Check if Black won
    else if (!_campCell.at(CampCell::Type::WhiteLeft).isEmpty() && !_campCell.at(CampCell::Type::WhiteRight).isEmpty())
    {
        _winningPlayer = BoardPlayer::Black;
        _gameFinished = true;
    }
}

bool Board::isGameFinished() const
{
    return _gameFinished;
}

BoardPlayer Board::whoWon() const
{
    return _winningPlayer;
}

BoardPlayer Board::getCurrentPlayer() const
{
    return _currentPlayer;
}

void Board::nextPlayer()
{
    _currentPlayer = (_currentPlayer == BoardPlayer::White) ? BoardPlayer::Black : BoardPlayer::White;
}


void Board::removePiece(Piece* p)
{
    std::cout << "Outch! " << ((p->getPlayer() == 1) ? "White" : "Black") << " just lost a Piece!" << std::endl;
    std::vector<Piece*> * list = p->getPlayer()==1 ? &_piecesW : &_piecesB;

    auto it = std::find(list->begin(), list->end(), p);
    if (it != list->end())
        list->erase(it);

    delete p;
}

BoardCell& Board::indexToBoardCell(int cell_index)
{
    if(cell_index < 0 || cell_index > 63)
        throw std::runtime_error("no BoardCell defined for index " + std::to_string(cell_index));

    int row = cell_index / 8;
    int col = cell_index % 8;

    return _plateau[row][col];
}

const BoardCell& Board::indexToBoardCell(int cell_index) const
{
    if(cell_index < 0 || cell_index > 63)
        throw std::runtime_error("no BoardCell defined for index " + std::to_string(cell_index));

    int row = cell_index / 8;
    int col = cell_index % 8;

    return _plateau[row][col];
}

CampCell& Board::indexToCampCell(int campcell_index)
{
    switch(campcell_index)
    {
    case 64:
        return _campCell.at(CampCell::Type::WhiteLeft);
        break;
    case 65:
        return _campCell.at(CampCell::Type::WhiteRight);
        break;
    case 66:
        return _campCell.at(CampCell::Type::BlackLeft);
        break;
    case 67:
        return _campCell.at(CampCell::Type::BlackRight);
        break;
    default:
        throw std::runtime_error("no CampCell defined for index " + std::to_string(campcell_index));
    }
}

const CampCell& Board::indexToCampCell(int campcell_index) const
{
    switch(campcell_index)
    {
    case 64:
        return _campCell.at(CampCell::Type::WhiteLeft);
        break;
    case 65:
        return _campCell.at(CampCell::Type::WhiteRight);
        break;
    case 66:
        return _campCell.at(CampCell::Type::BlackLeft);
        break;
    case 67:
        return _campCell.at(CampCell::Type::BlackRight);
        break;
    default:
        throw std::runtime_error("no CampCell defined for index " + std::to_string(campcell_index));
    }
}

// Converts a Move to a string representation of that Move
std::string Board::moveToString(const Move& move) const
{
    int src_index = move.first.getIndex();
    int dest_index = move.second.getIndex();
    std::string s;

    // TODO add the current player at the beginning of this move string

    if (src_index == 64 || src_index == 65)
    {
        s = "+" + std::to_string(src_index); // A CampCell
    }
    else if (src_index == 66 || src_index == 67)
    {
        s = std::to_string(src_index); // A CampCell
    }
    else if (src_index < 10)
    {
        s = "0" + std::to_string(src_index);
    }
    else
    {
        s = std::to_string(src_index);
    }
    s += ":";
    if (dest_index == 64 || dest_index == 65)
    {
        s += "+" + std::to_string(dest_index); // A CampCell
    }
    else if (dest_index == 66 || dest_index == 67)
    {
        s += std::to_string(dest_index); // A CampCell
    }
    else if (dest_index < 10)
    {
        s += "0" + std::to_string(dest_index);
    }
    else
    {
        s += std::to_string(dest_index);
    }

    if (s.size() != 5)
    {
        std::cout << s << " " << s.size() << std::endl;
        std::cout << "ERROR while transcripting a Move ("<< src_index << ":" << dest_index << ") to a string." << std::endl;
        //TODO RAISE ERROR
        return std::string();
    }
    return s;
}

// Converts a string representation of a Move to a Move
Move Board::stringToMove(const std::string& move_str) const
{
    /* A string move is of the form:
     * m<src_cell_index>:<dest_cell_index>
     * Where the index is between 0 and 63 for a Board cell
     * and 64/65/66/67 for a Camp cell. */
    if (move_str.size() != 5)
        throw std::runtime_error("error while retrieving a Move from a string (" + move_str + ").");

    std::string src_str = move_str.substr(0, 2);
    std::string dest_str = move_str.substr(3, 2);

    return Move(indexToBoardCell(stoi(src_str)),indexToBoardCell(stoi(dest_str)));
}

Move Board::indicesToMove(int src, int dest) const
{
    return Move(indexToBoardCell(src), indexToBoardCell(dest));
}

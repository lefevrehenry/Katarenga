#include "Board.hpp"
#include "server.hpp"
#include "Piece.hpp"

#include <iostream>
#include <algorithm>

Board::Board() :
    _plateau(),
    _campCell(),
    _piecesW(8),
    _piecesB(8),
    _currentPlayer(1),
    _winningPlayer(0),
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
    _campCell[CampCell::Type::WhiteRight] = CampCell(-1,-1,65);
    _campCell[CampCell::Type::BlackLeft] = CampCell(-1,-1,66);
    _campCell[CampCell::Type::BlackRight] = CampCell(-1,-1,67);

	// Initialization of Black and White Pieces
    for(int col = 0; col < 8; ++col)
	{
        // For White
        Piece* pieceW = new Piece(1, &_plateau[0][col]);
		_piecesW.push_back(pieceW);

        // For Black
        Piece* pieceB = new Piece(-1, &_plateau[7][col]);
		_piecesB.push_back(pieceB);
	}

//    if(_verbose)
//    {
//        std::cout << "Board successfully initialized, now is time to play!" << std::endl;
//    }
}

Board::~Board()
{
    for(size_t i = 0; i < _piecesW.size(); ++i)
	{
        delete _piecesW[i];
		_piecesW[i] = nullptr;
	}
    for(size_t i = 0; i < _piecesB.size(); ++i)
	{
        delete _piecesB[i];
		_piecesB[i] = nullptr;
	}
//	for(int i = 1; i<=8; ++i)
//	{
//		for(int j = 1; j<=8; ++j)
//		{
//			delete _plateau[i][j];
//		}
//		delete _plateau[i];
//	}
//    delete _plateau[0][0]; // Isn't
//    delete _plateau[0][1]; // it
//    delete _plateau[9][0]; // too
//    delete _plateau[9][1]; // ugly
//    delete _plateau;       // like
//    _plateau = nullptr;   // that?
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
        for (int col = 1; col < 8; ++col)
        {
            const BoardCell& cell = _plateau[row][col];
            switch(cell.getType())
            {
            case CellType::KING:
                s+= "K";
                break;
            case CellType::BISHOP:
                s+="B";
                break;
            case CellType::KNIGHT:
                s+="N";
                break;
            case CellType::ROCK:
                s+="R";
                break;
            default:
                s+=" ";
            }

            Piece* piece = cell.getPiece();
            if (piece)
            {
                if (piece->getPlayer() == 1)
                {
                    s+="+";
                }
                else
                {
                    s+="-";
                }
            }
            else
            {
                s+=" ";
            }
        }
    }

    // Populate the camp cells
    s+= (_campCell.at(CampCell::Type::WhiteLeft).isEmpty() ? " " : "-"); // White
    s+= (_campCell.at(CampCell::Type::WhiteRight).isEmpty() ? " " : "-");
    s+= (_campCell.at(CampCell::Type::BlackLeft).isEmpty() ? " " : "+"); // Black
    s+= (_campCell.at(CampCell::Type::BlackRight).isEmpty() ? " " : "+");

    // Then put the current player and whether the game is finished
    s+= (_currentPlayer == 1 ? "+" : "-");

    if (isGameFinished())
    {
        s+= (whoWon() == 1 ? "+" : "-");
    }
    else
    {
        s+=" ";
    }

    return s;
}

void Board::setBoardCellTypes(const std::string& boardString)
{
    int index = 0;

    // Iterates over rows
    for(int i = 1; i <= 8; ++i)
    {
        // Iterates over columns
        for(int j = 1; j <= 8; ++j)
        {
            BoardCell& cell = _plateau[i][j];
            const char charType = boardString.at(index);

            switch (charType)
            {
            case 'K':
                cell.setType(CellType::KING);
                break;
            case 'R':
                cell.setType(CellType::ROCK);
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

bool Board::isValidMove(const Move& m, int player) const
{
//    if(_verbose)
//    {
//        std::cout << "Is this move valid? " << m.first->getIndex() << " -> " << m.second->getIndex() << std::endl;
//    }

    if (m.first.isEmpty() || m.first.getPiece()->getPlayer() != player)
    {
        return false;
    }
    if (!m.second.isEmpty() && m.second.getPiece()->getPlayer() == player)
    {
        return false;
    }

//    for (const Move& move : *(m.first.getPiece()->getMoveList()))
//    {
//        if (move.second == m.second)
//        {
//            return true;
//        }
//    }

    return false;
}

bool Board::isValidMove(const std::string& move_str, int player) const
{
    Move m = stringToMove(move_str);
    return isValidMove(m, player);
}

bool Board::isValidMove(int src, int dest, int player) const
{
    Move m = indicesToMove(src, dest);
    return isValidMove(m, player);
}

//Board::Move Board::askNextValidMove() const
//{
//    int srci, desti;
//    bool flag = true;
//    Move m;
//    std::cout << (this->_currentPlayer == 1 ? "It's White's (+)" : "Black's (-)") << " turn, what is your move?" << std::endl;
//    while(flag)
//    {
//        std::cout << "Source index: ";
//        std::cin >> srci;
//        std::cout << "Destination index: ";
//        std::cin >> desti;

//        m = Move(indexToBoardCell(srci),indexToBoardCell(desti));

//        if(!isValidMove(m, _currentPlayer))
//        {
//            std::cout << "Invalid move... what is your move?" << std::endl;
//        }
//        else
//        {
//            flag = false;
//        }
//    }
//    return m;
//}

bool Board::playMove(const Move& move)
{
    if (!isValidMove(move, getCurrentPlayer()))
    {
        // TODO throw an error
        //std::cout << "ERROR: Trying to play an invalid move" << std::endl;
        return false;
    }

    Cell& src_cell = indexToBoardCell(move.first.getIndex());
    Cell& dst_cell = indexToBoardCell(move.second.getIndex());
    Piece * piece = src_cell.getPiece();

    if(!dst_cell.isEmpty())
    {
        removePiece(dst_cell.getPiece());
    }
    dst_cell.setPiece(piece);
    src_cell.setPiece(nullptr);
    piece->setCell(&dst_cell);
    _currentPlayer = -_currentPlayer;

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
    list->clear();
    const BoardCell& cell = _plateau[row][col];

    if (cell.isEmpty() || cell.isCampCell())
        return;

    int player = cell.getPiece()->getPlayer();

    // Check if the piece can go to an adversary camp
    if (row == 0 && player == -1)
    {
        if(_plateau[0][0].isEmpty())
        {
            list->push_back(Move(cell,_campCell.at(CampCell::Type::WhiteLeft)));
        }
        if(_plateau[0][7].isEmpty())
        {
            list->push_back(Move(cell,_campCell.at(CampCell::Type::WhiteRight)));
        }
    }
    if (row == 7 && player == 1)
    {
        if(_plateau[7][0].isEmpty())
        {
            list->push_back(Move(cell,_campCell.at(CampCell::Type::BlackLeft)));
        }
        if(_plateau[7][0].isEmpty())
        {
            list->push_back(Move(cell,_campCell.at(CampCell::Type::BlackRight)));
        }
    }

    const BoardCell* new_cell;

    switch(cell.getType())
    {
    case CellType::KING: // Check cells directly around
    {
        for(int i = -1; i <= 1; ++i)
        {
            for(int j = -1; j <= 1; ++j)
            {
                if(i == 0 && j == 0)
                    continue;

                if((row+i < 0) || (row+i > 7) || (col+j < 0) || (col+j > 7))
                    continue;

                new_cell = &_plateau[row+i][col+j];
                if(new_cell->isEmpty() || new_cell->getPiece()->getPlayer() == -player)
                    list->push_back(Move(cell,_plateau[row+i][col+j]));
            }
        }
        break; //Switch break;
    }

    case CellType::BISHOP: // Check cells in diagonal until next edge or Bishop cell or oponnent
    {
        // Top left direction
        int i = 1;
        int j = 1;
        while((row-i) >= 0 && (col-j) >= 0)
        {
            if(!checkCellAddMove(cell, row-i, col-j, player, CellType::BISHOP, list))
            {
                break;
            }
            ++i;
            ++j;
        }

        // Top right direction
        i = 1;
        j = 1;
        while((row-i) >= 0 && (col+j) <= 7)
        {
            if(!checkCellAddMove(cell, row-i, col+j, player, CellType::BISHOP, list))
            {
                break;
            }
            ++i;
            ++j;
        }

        // Bottom left direction
        i = 1;
        j = 1;
        while((row+i) <= 7 && (col-j) >= 0)
        {
            if(!checkCellAddMove(cell, row+i, col-j, player, CellType::BISHOP, list))
            {
                break;
            }
            ++i;
            ++j;
        }

        // Bottom right direction
        i = 1;
        j = 1;
        while((row+i) <= 7 && (col+j) <= 7)
        {
            if(!checkCellAddMove(cell, row+i, col+j, player, CellType::BISHOP, list))
            {
                break;
            }
            ++i;
            ++j;
        }
        break; //Switch break;
    }

    case CellType::ROCK: // Check cells in straight line until Rock cell or edge or oponnent
    {
        int i = 1;
        while((row-i) >= 0) // Top line
        {
            if(!checkCellAddMove(cell, row-i, col, player, CellType::ROCK, list))
            {
                break;
            }
            ++i;
        }

        i = 1;
        while((row+i) <= 7) // Bottom line
        {
            if(!checkCellAddMove(cell, row+i, col, player, CellType::ROCK, list))
            {
                break;
            }
            ++i;
        }

        i = 1;
        while((col-i) >= 0) // Left line
        {
            if(!checkCellAddMove(cell, row, col-i, player, CellType::ROCK, list))
            {
                break;
            }
            ++i;
        }

        i = 1;
        while((col+i) <= 0) // Right line
        {
            if(!checkCellAddMove(cell, row, col+i, player, CellType::ROCK, list))
            {
                break;
            }
            ++i;
        }
        break; //Switch break
    }

    case CellType::KNIGHT:
    {
        //Left top and bottom
        if((col-2) >= 0)
        {
            if((row-1) >= 0)
            {
                new_cell = &_plateau[row-1][col-2];
                if(new_cell->isEmpty() || new_cell->getPiece()->getPlayer() == -player)
                {
                    list->push_back(Move(cell,_plateau[row-1][col-2]));
                }
            }
            if((row+1) <= 7)
            {
                new_cell = &_plateau[row+1][col-2];
                if(new_cell->isEmpty() || new_cell->getPiece()->getPlayer() == -player)
                {
                    list->push_back(Move(cell,_plateau[row+1][col-2]));
                }
            }
        }
        //Right top and bottom
        if((col+2) <= 7)
        {
            if((row-1) >= 0)
            {
                new_cell = &_plateau[row-1][col+2];
                if(new_cell->isEmpty() || new_cell->getPiece()->getPlayer() == -player)
                {
                    list->push_back(Move(cell,_plateau[row-1][col+2]));
                }
            }
            if((row+1) <= 7)
            {
                new_cell = &_plateau[row+1][col+2];
                if(new_cell->isEmpty() || new_cell->getPiece()->getPlayer() == -player)
                {
                    list->push_back(Move(cell,_plateau[row+1][col+2]));
                }
            }
        }

        //Top left and right
        if((row-2) >= 0)
        {
            if((col-1) >= 0)
            {
                new_cell = &_plateau[row-2][col-1];
                if(new_cell->isEmpty() || new_cell->getPiece()->getPlayer() == -player)
                {
                    list->push_back(Move(cell,_plateau[row-2][col-1]));
                }
            }
            if((col+1) <= 7)
            {
                new_cell = &_plateau[row-2][col+1];
                if(new_cell->isEmpty() || new_cell->getPiece()->getPlayer() == -player)
                {
                    list->push_back(Move(cell,_plateau[row-2][col+1]));
                }
            }
        }

        //Bottom left and right
        if((row+2) <= 7)
        {
            if((col-1) >= 0)
            {
                new_cell = &_plateau[row+2][col-1];
                if(new_cell->isEmpty() || new_cell->getPiece()->getPlayer() == -player)
                {
                    list->push_back(Move(cell,_plateau[row+2][col-1]));
                }
            }
            if((col+1) <= 7)
            {
                new_cell = &_plateau[row+2][col+1];
                if(new_cell->isEmpty() || new_cell->getPiece()->getPlayer() == -player)
                {
                    list->push_back(Move(cell,_plateau[row+2][col+1]));
                }
            }
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
        _winningPlayer = 1;
        _gameFinished = true;
    }
    // Check if Black won
    else if (!_campCell.at(CampCell::Type::WhiteLeft).isEmpty() && !_campCell.at(CampCell::Type::WhiteRight).isEmpty())
    {
        _winningPlayer = -1;
        _gameFinished = true;
    }
}

bool Board::isGameFinished() const
{
    return _gameFinished;
}

int Board::whoWon() const
{
    return _winningPlayer;
}

int Board::getCurrentPlayer() const
{
    return _currentPlayer;
}

/*void Board::main_loop()
{
    Move move;
    print();
    while(true)
    {
        move = askNextValidMove();

        playMove(move);

        print();

        if(isGameFinished())
        {
            std::cout << "Congrats! " << ((_winningPlayer==1)?"White (+)":"Black (-)") << " player won the game!" << std::endl;
            break;
        }
    }
}*/

void Board::print() const
{
    std::string s = "   1    2    3    4    5    6    7    8\n";
    s += (_campCell.at(CampCell::Type::WhiteLeft).isEmpty() ? " " : "X");
    s += "              White              ";
    s += (_campCell.at(CampCell::Type::WhiteRight).isEmpty() ? " " : "X");
    s += "\n";
    for(int row = 0; row <= 7; ++row)   // Iterates over rows
    {
        s+= std::to_string(row) + " ";
        for(int col = 0; col <= 7; ++col)	// Iterates over columns
        {
            const BoardCell& cell = _plateau[row][col];

            int id = cell.getIndex();
            s+= (id >= 10 ? "" : " ") + std::to_string(cell.getIndex());

            switch(cell.getType())
            {
                case CellType::KING:
                    s+= "K";
                    break;
                case CellType::BISHOP:
                    s+= "B";
                    break;
                case CellType::KNIGHT:
                    s+= "N";
                    break;
                case CellType::ROCK:
                    s+= "R";
                    break;
                case CellType::NONE:
                    s += "-";
                    break;
            }
            if(cell.isEmpty())
            {
                s+= "  ";
            }
            else
            {
                s+= cell.getPiece()->getPlayer() == 1 ? "+ " : "- ";
            }
        }
        s += "\n";
    }
    s += (_campCell.at(CampCell::Type::BlackLeft).isEmpty() ? " " : "X");
    s += "              Black              ";
    s += (_campCell.at(CampCell::Type::BlackRight).isEmpty() ? " " : "X");
    std::cout << s << std::endl;
}

void Board::removePiece(Piece* p)
{
    std::cout << "Outch! " << ((p->getPlayer() == 1) ? "White" : "Black") << " just lost a Piece!" << std::endl;
    std::vector<Piece*> * list = p->getPlayer()==1 ? &_piecesW : &_piecesB;

    auto it = std::find(list->begin(), list->end(), p);
    if (it != list->end())
    {
        list->erase(it);
    }

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

bool Board::checkCellAddMove(const BoardCell& src_cell, int row, int col, int player, CellType type, std::vector<Move>* plist) const
{
    const BoardCell& dst_cell = _plateau[row][col];
    if(dst_cell.isEmpty())
    {
        plist->push_back(Move(src_cell,dst_cell));
        if(dst_cell.getType() == type)
        {
            return false;
        }
    }
    else
    {
        if(dst_cell.getPiece()->getPlayer() == -player)
        {
            plist->push_back(Move(src_cell, dst_cell));
        }
        return false;
    }
    return true;
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

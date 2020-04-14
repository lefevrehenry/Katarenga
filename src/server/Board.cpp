#include "Board.hpp"
#include "server.hpp"

#include <iostream>
#include <algorithm>

Board::Board()
{
	// Initialization of the BoardCells
    _plateau = new Cell**[10];

    if(!_plateau)
        throw std::runtime_error("cannot create Board");

//    if(verbose){ std::cout << "String used for the board generation: " << board_configuration << std::endl; }

	// Init BoardCells
    for(int i = 1; i <= 8; ++i)     // Iterates over rows
	{
        _plateau[i] = new Cell*[8];
        for(int j = 1; j <= 8; ++j)     // Iterates over columns
		{
            _plateau[i][j] = new BoardCell(CellType::NONE, i, j);
		}
	}

    // Initialization of the CampCells
    _plateau[0] = new Cell*[2];
    _plateau[9] = new Cell*[2];
    _plateau[0][0] = new CampCell(0,0,1);
    _plateau[0][1] = new CampCell(0,1,2);
    _plateau[9][0] = new CampCell(9,0,-1);
    _plateau[9][1] = new CampCell(9,1,-2);

	// Initialization of Black and White Pieces
	_piecesW.reserve(8);
	_piecesB.reserve(8);
    for(int j = 1; j <= 8; ++j)
	{
        // For White
        Piece* pieceW = new Piece(1, _plateau[1][j]);
		_piecesW.push_back(pieceW);

        // For Black
        Piece* pieceB = new Piece(-1, _plateau[8][j]);
		_piecesB.push_back(pieceB);
	}

    // Initalization of the list of possible Moves for every Piece
    for(int j = 1; j <= 8; ++j)
    {
        fillAllMoves(1, j, _piecesW[j-1]->getMoveList());
        fillAllMoves(8, j, _piecesB[j-1]->getMoveList());
    }

//    if(_verbose)
//    {
//        std::cout << "Board successfully initialized, now is time to play!" << std::endl;
//    }
}

Board::~Board()
{
    Piece * p;
	for(size_t i = 0; i<_piecesW.size(); ++i)
	{
        p = _piecesW[i];
        //delete p->getMoveList();
        delete p;
		_piecesW[i] = nullptr;
	}
	for(size_t i = 0; i<_piecesB.size(); ++i)
	{
        p = _piecesB[i];
        //delete p->getMoveList();
        delete p;
		_piecesB[i] = nullptr;
	}
	for(int i = 1; i<=8; ++i)
	{
		for(int j = 1; j<=8; ++j)
		{
			delete _plateau[i][j];
		}
		delete _plateau[i];
	}
    delete _plateau[0][0]; // Isn't
    delete _plateau[0][1]; // it
    delete _plateau[9][0]; // too
    delete _plateau[9][1]; // ugly
    delete _plateau;       // like
    _plateau = nullptr;   // that?
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
    for(int i = 1; i <= 8; ++i)
    {
        for (int j = 1; j <= 8; ++j)
        {
            BoardCell* cell = dynamic_cast<BoardCell*>(_plateau[i][j]);
            switch(cell->getType())
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

            Piece* piece = cell->getPiece();
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
    s+= (_plateau[0][0]->isEmpty() ? " " : "-"); // White
    s+= (_plateau[0][1]->isEmpty() ? " " : "-");
    s+= (_plateau[9][0]->isEmpty() ? " " : "+"); // Black
    s+= (_plateau[9][1]->isEmpty() ? " " : "+");

    // Then put the current player and whether the game is finished
    s+= (_currentPlayer == 1 ? "+" : "-");

    int finished = whoWon();
    if (finished)
    {
        s+= (finished == 1 ? "+" : "-");
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
            BoardCell* cell = dynamic_cast<BoardCell*>(_plateau[i][j]);
            const char charType = boardString.at(index);

            switch (charType)
            {
            case 'K':
                cell->setType(CellType::KING);
                break;
            case 'R':
                cell->setType(CellType::ROCK);
                break;
            case 'B':
                cell->setType(CellType::BISHOP);
                break;
            case 'N':
                cell->setType(CellType::KNIGHT);
                break;
            default:
                cell->setType(CellType::NONE);
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

    if (m.first->isEmpty() || m.first->getPiece()->getPlayer() != player)
    {
        return false;
    }
    if (!m.second->isEmpty() && m.second->getPiece()->getPlayer() == player)
    {
        return false;
    }

    for (Move move : *(m.first->getPiece()->getMoveList()))
    {
        if (move.second == m.second)
        {
            return true;
        }
    }

    return false;
}

bool Board::isValidMove(const std::string& move_str, int player) const
{
    Move m = stringToMove(move_str);
    return isValidMove(m, player);
}

Move Board::askNextValidMove() const
{
    int srci, desti;
    bool flag = true;
    Move m;
    std::cout << (this->_currentPlayer == 1 ? "It's White's (+)" : "Black's (-)") << " turn, what is your move?" << std::endl;
    while(flag)
    {
        std::cout << "Source index: ";
        std::cin >> srci;
        std::cout << "Destination index: ";
        std::cin >> desti;

        m = Move(indexToCell(srci),indexToCell(desti));

        if(!isValidMove(m, _currentPlayer))
        {
            std::cout << "Invalid move... what is your move?" << std::endl;
        }
        else
        {
            flag = false;
        }
    }
    return m;
}

void Board::playMove(const Move& move)
{
    if (!isValidMove(move, getCurrentPlayer()))
    {
        // TODO throw an error
        std::cout << "ERROR: Trying to play an invalid move" << std::endl;
        return;
    }

    BoardCell * src_cell = (BoardCell *)move.first;
    Cell * dst_cell = move.second;
    Piece * piece = src_cell->getPiece();

    if(!dst_cell->isEmpty()){
        removePiece(dst_cell->getPiece());
    }
    dst_cell->setPiece(piece);
    src_cell->setPiece(nullptr);
    piece->setCell(dst_cell);
    fillAllMoves(dst_cell->getRow(), dst_cell->getColumn(), piece->getMoveList());
    _currentPlayer = -_currentPlayer;

    updateGameFinished();
}

void Board::playMove(const std::string& move_str)
{
    Move m = stringToMove(move_str);
    playMove(m);
}

void Board::fillAllMoves(int row, int col, std::vector<Move>* list) const
{
    list->clear();
    Cell* cell = _plateau[row][col];
    if (cell->isEmpty() || cell->isCampCell()) // If CampCell, the piece cannot move anymore
    {
        return;
    }
    int i,j;
    int player = cell->getPiece()->getPlayer();


    // Check if the piece can go to an adversary camp
    if (row == 1 && player == -1){
        if(_plateau[0][0]->isEmpty())
        {
            list->push_back(Move(cell,_plateau[0][0]));
        }
        if(_plateau[0][1]->isEmpty())
        {
            list->push_back(Move(cell,_plateau[0][1]));
        }
    }
    if (row == 8 && player == 1){
        if(_plateau[8][0]->isEmpty())
        {
            list->push_back(Move(cell,_plateau[8][0]));
        }
        if(_plateau[8][1]->isEmpty())
        {
            list->push_back(Move(cell,_plateau[8][1]));
        }
    }
    BoardCell* new_cell;
    switch(((BoardCell*)cell)->getType())
    {
    case CellType::KING: // Check cells directly around
        for(i = -1; i<=1; ++i){
            for(j=-1; j<=1; ++j)
            {
                if(i == 0 && j == 0)
                {
                    continue;
                }
                if( (row+i < 1) || (row+i > 8) || (col+j < 1) || (col+j > 8))
                {
                    continue;
                }

                new_cell = (BoardCell*)_plateau[row+i][col+j];
                if(new_cell->isEmpty() || new_cell->getPiece()->getPlayer() == -player)
                {
                    list->push_back(Move(cell,_plateau[row+i][col+j]));
                }
            }
        }
        break; //Switch break;


    case CellType::BISHOP: // Check cells in diagonal until next edge or Bishop cell or oponnent
        // Top left direction
        i = 1;
        j = 1;
        while((row-i)>= 1 && (col-j) >= 1)
        {
            if(! checkCellAddMove((BoardCell*) cell, row-i, col-j, player, CellType::BISHOP, list))
            {
                break;
            }
            ++i;
            ++j;
        }

        // Top right direction
        i = 1;
        j = 1;
        while((row-i)>= 1 && (col+j) <= 8)
        {
            if(! checkCellAddMove((BoardCell*) cell, row-i, col+j, player, CellType::BISHOP, list))
            {
                break;
            }
            ++i;
            ++j;
        }

        // Bottom left direction
        i = 1;
        j = 1;
        while((row+i) <= 8 && (col-j) >= 1)
        {
            if(! checkCellAddMove((BoardCell*) cell, row+i, col-j, player, CellType::BISHOP, list))
            {
                break;
            }
            ++i;
            ++j;
        }

        // Bottom right direction
        i = 1;
        j = 1;
        while((row+i) <= 8 && (col+j) <= 8)
        {
            if(! checkCellAddMove((BoardCell*) cell, row+i, col+j, player, CellType::BISHOP, list))
            {
                break;
            }
            ++i;
            ++j;
        }
        break; //Switch break;


    case CellType::ROCK: // Check cells in straight line until Rock cell or edge or oponnent
        i = 1;
        while((row-i) >= 1) // Top line
        {
            if(! checkCellAddMove((BoardCell*) cell, row-i, col, player, CellType::ROCK, list))
            {
                break;
            }
            ++i;
        }

        i = 1;
        while((row+i) <= 8) // Bottom line
        {
            if(! checkCellAddMove((BoardCell*) cell, row+i, col, player, CellType::ROCK, list))
            {
                break;
            }
            ++i;
        }

        i = 1;
        while((col-i) >= 1) // Left line
        {
            if(! checkCellAddMove((BoardCell*) cell, row, col-i, player, CellType::ROCK, list))
            {
                break;
            }
            ++i;
        }

        i = 1;
        while((col+i) <= 8) // Right line
        {
            if(! checkCellAddMove((BoardCell*) cell, row, col+i, player, CellType::ROCK, list))
            {
                break;
            }
            ++i;
        }

        break; //Switch break


    case CellType::KNIGHT:
        //Left top and bottom
        if((col-2) >= 1)
        {
            if((row-1) >= 1)
            {
                new_cell = (BoardCell*) _plateau[row-1][col-2];
                if(new_cell->isEmpty() || new_cell->getPiece()->getPlayer() == -player)
                {
                    list->push_back(Move(cell,_plateau[row-1][col-2]));
                }
            }
            if((row+1) <= 8)
            {
                new_cell = (BoardCell*) _plateau[row+1][col-2];
                if(new_cell->isEmpty() || new_cell->getPiece()->getPlayer() == -player)
                {
                    list->push_back(Move(cell,_plateau[row+1][col-2]));
                }
            }
        }
        //Right top and bottom
        if((col+2) <= 8)
        {
            if((row-1) >= 1)
            {
                new_cell = (BoardCell*) _plateau[row-1][col+2];
                if(new_cell->isEmpty() || new_cell->getPiece()->getPlayer() == -player)
                {
                    list->push_back(Move(cell,_plateau[row-1][col+2]));
                }
            }
            if((row+1) <= 8)
            {
                new_cell = (BoardCell*) _plateau[row+1][col+2];
                if(new_cell->isEmpty() || new_cell->getPiece()->getPlayer() == -player)
                {
                    list->push_back(Move(cell,_plateau[row+1][col+2]));
                }
            }
        }

        //Top left and right
        if((row-2) >= 1)
        {
            if((col-1) >= 1)
            {
                new_cell = (BoardCell*) _plateau[row-2][col-1];
                if(new_cell->isEmpty() || new_cell->getPiece()->getPlayer() == -player)
                {
                    list->push_back(Move(cell,_plateau[row-2][col-1]));
                }
            }
            if((col+1) <= 8)
            {
                new_cell = (BoardCell*) _plateau[row-2][col+1];
                if(new_cell->isEmpty() || new_cell->getPiece()->getPlayer() == -player)
                {
                    list->push_back(Move(cell,_plateau[row-2][col+1]));
                }
            }
        }

        //Bottom left and right
        if((row+2) <= 8)
        {
            if((col-1) >= 1)
            {
                new_cell = (BoardCell*) _plateau[row+2][col-1];
                if(new_cell->isEmpty() || new_cell->getPiece()->getPlayer() == -player)
                {
                    list->push_back(Move(cell,_plateau[row+2][col-1]));
                }
            }
            if((col+1) <= 8)
            {
                new_cell = (BoardCell*) _plateau[row+2][col+1];
                if(new_cell->isEmpty() || new_cell->getPiece()->getPlayer() == -player)
                {
                    list->push_back(Move(cell,_plateau[row+2][col+1]));
                }
            }
        }
        break;
    default:
        break;
    }
}


void Board::updateGameFinished()
{
    // Check if White won
    if (!_plateau[9][0]->isEmpty() && !_plateau[9][1]->isEmpty())
    {
        _winningPlayer = 1;
        _gameFinished = true;
    }
    // Check if Black won
    else if (!_plateau[0][0]->isEmpty() && !_plateau[0][1]->isEmpty())
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
    s+= (_plateau[0][0]->isEmpty() ? " " : "X");
    s+= "              White              ";
    s+= (_plateau[0][1]->isEmpty() ? " " : "X");
    s+= "\n";
    for(int i = 1; i<=8; ++i)		// Iterates over rows
    {
        s+= std::to_string(i) + " ";
        for(int j = 1; j<=8; ++j)	// Iterates over columns
        {
            BoardCell * cell = (BoardCell*) _plateau[i][j];

            int id = cell->getIndex();
            s+= (id>=10?"":" ") + std::to_string(cell->getIndex());

            switch(cell->getType())
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
            if(cell->isEmpty())
            {
                s+= "  ";
            }
            else
            {
                s+= cell->getPiece()->getPlayer() == 1 ? "+ " : "- ";
            }
        }
        s+="\n";
    }
    s+= (_plateau[9][0]->isEmpty() ? " " : "X");
    s+= "              Black              ";
    s+= (_plateau[9][1]->isEmpty() ? " " : "X");
    //s+= "\n";
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

Cell* Board::indexToCell(int cell_index) const
{
    switch(cell_index)
    {
    case 1:
        return _plateau[0][0];
        break;
    case 2:
        return _plateau[0][1];
        break;
    case -1:
        return _plateau[9][0];
        break;
    case -2:
        return _plateau[9][1];
        break;
    default:
        int row = cell_index/8;
        int col = (cell_index % 8) + 1;
        return _plateau[row][col];
    }
}

bool Board::checkCellAddMove(BoardCell* src_cell, int row, int col, int player, CellType type, std::vector<Move>* plist) const
{
    BoardCell* dst_cell = (BoardCell*)_plateau[row][col];
    if(dst_cell->isEmpty())
    {
        plist->push_back(Move(src_cell,dst_cell));
        if(dst_cell->getType() == type)
        {
            return false;
        }
    }
    else
    {
        if(dst_cell->getPiece()->getPlayer() == -player)
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
    int src_index = move.first->getIndex();
    int dest_index = move.second->getIndex();
    std::string s;

    // TODO add the current player at the beginning of this move string

    if (src_index == 1 || src_index == 2)
    {
        s = "+" + std::to_string(src_index); // A CampCell
    }
    else if (src_index == -1 || src_index == -2)
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
    if (dest_index == 1 || dest_index == 2)
    {
        s += "+" + std::to_string(dest_index); // A CampCell
    }
    else if (dest_index == -1 || dest_index == -2)
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
     * Where the index is between 08 and 72 for a Board cell
     * and -1/-2/+1/+2 for a Camp cell. */
    if (move_str.size() != 6)
    {
        std::cout << "ERROR while retrieving a Move from a string (" << move_str << ")." << std::endl;
        // TODO RAISE ERROR
        return Move();
    }

    std::string src_str = move_str.substr(1, 2);
    std::string dest_str = move_str.substr(4, 2);

    /*int srci = stoi(src_str);
    int desti = stoi(dest_str);
    std::cout << srci << " " << desti << std::endl;*/

    return Move(indexToCell(stoi(src_str)),indexToCell(stoi(dest_str)));
}

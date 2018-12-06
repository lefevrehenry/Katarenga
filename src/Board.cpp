#include "Board.hpp"

#include <iostream>
#include <algorithm>

Board::Board(bool verbose)
{
    _verbose = verbose;
	// Initialization of the BoardCells
	_plateau = (Cell***) malloc(10 * sizeof(Cell**));
	if(!_plateau){
		std::cout << "TODO RAISE ERROR" << std::endl;
		return;
	}


	std::string boardString = generateBoardString();
	int index = 0;


	boardString.at(1);
	// Init BoardCells
	for(int i = 1; i<=8; ++i)		// Iterates over rows
	{
		_plateau[i] = (Cell**) malloc(8 * sizeof(BoardCell*));
		for(int j = 1; j<=8; ++j)	// Iterates over columns
		{
			if(boardString.at(index) == 'K'){
				_plateau[i][j] = new BoardCell(CellType::KING, i, j);
			}
			else if (boardString.at(index) == 'R'){
				_plateau[i][j] = new BoardCell(CellType::ROCK, i, j);
			}
			else if (boardString.at(index) == 'B'){
				_plateau[i][j] = new BoardCell(CellType::BISHOP, i, j);
			}
			else if (boardString.at(index) == 'N'){
				_plateau[i][j] = new BoardCell(CellType::KNIGHT, i, j);
			}
			else{
				std::cout << "TODO RAISE ERROR" << std::endl;
				return;
			}
			index++;
		}
	}

    // Initialization of the CampCells
	_plateau[0] = (Cell**) malloc(2 * sizeof(CampCell*));
	_plateau[9] = (Cell**) malloc(2 * sizeof(CampCell*));
    _plateau[0][0] = new CampCell(0,0,1);
    _plateau[0][1] = new CampCell(0,1,2);
    _plateau[9][0] = new CampCell(9,0,-1);
    _plateau[9][1] = new CampCell(9,1,-2);


	// Initialization of Black and White Pieces
	_piecesW.reserve(8);
	_piecesB.reserve(8);
	for(int j = 1; j<=8; ++j)
	{
		// For White
        std::vector<move>* moveListW = new std::vector<move>();
        Piece * pieceW = new Piece(1, _plateau[1][j],moveListW);
		_piecesW.push_back(pieceW);

		// For Black
        std::vector<move>* moveListB = new std::vector<move>();
        Piece * pieceB = new Piece(-1, _plateau[8][j], moveListB);
		_piecesB.push_back(pieceB);
	}

    // Initalization of the list of possible moves for every Piece
    for(int j = 1; j<=8; ++j)
    {
        fillAllMoves(1, j, _piecesW[j-1]->getMoveList());
        fillAllMoves(8, j, _piecesB[j-1]->getMoveList());
    }

    if(_verbose)
    {
        std::cout << "Board successfully initialized, now is time to play!" << std::endl;
    }
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

void
Board::main_loop()
{
    int n = 0;
    move move
            ;
    print();
    while(true)
    {
        move = askNextValidMove();

        doMove(move);

        print();

        if(gameFinished())
        {
            std::cout << "Congrats! " << (_currentPlayer==1?"White (+)":"Black (-)") << " player won the game!" << std::endl;
            break;
        }
        _currentPlayer = -_currentPlayer;
    }
}


void
Board::doMove(move move)
{
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
}


move
Board::askNextValidMove()
{
    int srci, desti;
    bool flag = true;
    move m;
    std::cout << (this->_currentPlayer == 1 ? "It's White's (+)" : "Black's (-)") << " turn, what is your move?" << std::endl;
    while(flag)
    {
        std::cout << "Source index: ";
        std::cin >> srci;
        std::cout << "Destination index: ";
        std::cin >> desti;

        m = move(indexToCell(srci),indexToCell(desti));

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


bool Board::isValidMove(move m, int current_player)
{
    //std::cout << "Is this move valid? " << m.first->getIndex() << " -> " << m.second->getIndex() << std::endl;

    if (m.first->isEmpty() || m.first->getPiece()->getPlayer() != current_player)
    {
        return false;
    }
    if (!m.second->isEmpty() && m.second->getPiece()->getPlayer() == current_player)
    {
        return false;
    }

    for (move move : *(m.first->getPiece()->getMoveList()))
    {
        //std::cout << "Testing " << move.first->getIndex() << " -> " << move.second->getIndex() << std::endl;
        if (move.second == m.second)
        {
            return true;
        }
        /*else
        {
            std::cout << "Cell " << move.second->getIndex() << " and " << m.second->getIndex() << " does not match" << std::endl;
        }*/
    }

    return false;
}

// Fills the list with possible moves of the Piece in the cell (row,col)
void Board::fillAllMoves(int row, int col, std::vector<move>* list)
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
            list->push_back(move(cell,_plateau[0][0]));
		}
		if(_plateau[0][1]->isEmpty())
		{
            list->push_back(move(cell,_plateau[0][1]));
		}
	}
	if (row == 8 && player == 1){
		if(_plateau[8][0]->isEmpty())
		{
            list->push_back(move(cell,_plateau[8][0]));
		}
		if(_plateau[8][1]->isEmpty())
		{
            list->push_back(move(cell,_plateau[8][1]));
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
                    list->push_back(move(cell,_plateau[row+i][col+j]));
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
                    list->push_back(move(cell,_plateau[row-1][col-2]));
				}
			}
			if((row+1) <= 8)
			{
				new_cell = (BoardCell*) _plateau[row+1][col-2];
				if(new_cell->isEmpty() || new_cell->getPiece()->getPlayer() == -player)
				{
                    list->push_back(move(cell,_plateau[row+1][col-2]));
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
                    list->push_back(move(cell,_plateau[row-1][col+2]));
				}
			}
			if((row+1) <= 8)
			{
				new_cell = (BoardCell*) _plateau[row+1][col+2];
				if(new_cell->isEmpty() || new_cell->getPiece()->getPlayer() == -player)
				{
                    list->push_back(move(cell,_plateau[row+1][col+2]));
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
                    list->push_back(move(cell,_plateau[row-2][col-1]));
				}
			}
			if((col+1) <= 8)
			{
				new_cell = (BoardCell*) _plateau[row-2][col+1];
				if(new_cell->isEmpty() || new_cell->getPiece()->getPlayer() == -player)
				{
                    list->push_back(move(cell,_plateau[row-2][col+1]));
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
                    list->push_back(move(cell,_plateau[row+2][col-1]));
				}
			}
			if((col+1) <= 8)
			{
				new_cell = (BoardCell*) _plateau[row+2][col+1];
				if(new_cell->isEmpty() || new_cell->getPiece()->getPlayer() == -player)
				{
                    list->push_back(move(cell,_plateau[row+2][col+1]));
				}
			}
		}
		break;
	default:
		break;
	}
}

// Internal function to populate the vector of moves, called in getAllMoves.
// Returns false if the while loop calling this function has to break.
bool Board::checkCellAddMove(BoardCell* src_cell, int row, int col, int player, CellType type, std::vector<move> * plist)
{
    BoardCell * dst_cell = (BoardCell*)_plateau[row][col];
    if(dst_cell->isEmpty())
	{
        plist->push_back(move(src_cell,dst_cell));
        if(dst_cell->getType() == type)
		{
			return false;
		}
	}
	else
	{
        if(dst_cell->getPiece()->getPlayer() == -player)
		{
            plist->push_back(move(src_cell, dst_cell));
		}
		return false;
	}
	return true;
}


bool
Board::gameFinished()
{
    // Check if White won
    if (!_plateau[9][0]->isEmpty() && !_plateau[9][1]->isEmpty())
    {
        return true;
    }
    // Check if Black won
    if (!_plateau[0][0]->isEmpty() && !_plateau[0][1]->isEmpty())
    {
        return true;
    }
    return false;
}

Cell *
Board::indexToCell(int cell_index)
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

// Converts a string representation of a move to a move
move Board::stringToMove(std::string & move_str)
{
    /* A string move is of the form:
     * src_cell_index:dest_cell_index
     * Where the index is between 08 and 72 for a Board cell
     * and -1/-2/+1/+2 for a Camp cell. */
    if (move_str.size() != 5)
    {
        std::cout << "ERROR while retrieving a move from a string (" << move_str << ")." << std::endl;
        // TODO RAISE ERROR
        return move();
    }

    int src_index, dest_index;
    std::string src_str = move_str.substr(0, 2);
    std::string dest_str = move_str.substr(3, 2);

    int i = stoi(src_str);
    int j = stoi(dest_str);
    std::cout << i << " " << j << std::endl;

    return move(indexToCell(stoi(src_str)),indexToCell(stoi(dest_str)));
}

// Converts a move to a string representation of that move
std::string Board::moveToString(move move)
{
    int src_index = move.first->getIndex();
    int dest_index = move.second->getIndex();
    std::string s;

    if (src_index == 1 || src_index == 2)
    {
        s = "+" + std::to_string(src_index);
    }
    else if (src_index == -1 || src_index == -2)
    {
        s = "-" + std::to_string(src_index);
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
        s += "+" + std::to_string(dest_index);
    }
    else if (dest_index == -1 || dest_index == -2)
    {
        s += std::to_string(dest_index);
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
        std::cout << "ERROR while transcripting a move ("<< src_index << ":" << dest_index << ") to a string." << std::endl;
        //TODO RAISE ERROR
        return std::string();
    }
    return s;
}

void
Board::removePiece(Piece * p)
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

void
Board::print()
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

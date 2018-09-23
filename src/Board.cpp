#include "Board.hpp"

#include <iostream>

Board::Board()
{
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

	// Init the CampCells
	_plateau[0] = (Cell**) malloc(2 * sizeof(CampCell*));
	_plateau[9] = (Cell**) malloc(2 * sizeof(CampCell*));
	_plateau[0][0] = new CampCell(1);
	_plateau[0][1] = new CampCell(2);
	_plateau[9][0] = new CampCell(-1);
	_plateau[9][1] = new CampCell(-2);


	// Initialization of Black and White Pieces
	_piecesW.reserve(8);
	_piecesB.reserve(8);
	for(int j = 1; j<=8; ++j)
	{
		// For White
		Piece * pieceW = new Piece(1, _plateau[1][j]);
		_piecesW.push_back(pieceW);

		// For Black
		Piece * pieceB = new Piece(-1, _plateau[8][j]);
		_piecesB.push_back(pieceB);
	}

	//For tests
	//Piece * pp = new Piece(-1, _plateau[4][5]);

}

Board::~Board()
{
	for(size_t i = 0; i<_piecesW.size(); ++i)
	{
		delete _piecesW[i];
		_piecesW[i] = nullptr;
	}
	for(size_t i = 0; i<_piecesB.size(); ++i)
	{
		delete _piecesB[i];
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
	delete _plateau;	 // C'est correct
	 _plateau = nullptr; // comme ca ??
}


std::vector<move> Board::getAllMoves(int row, int col)
{
	std::vector<move> list;
	Cell* cell = _plateau[row][col];
	if (cell->isEmpty() || cell->isCampCell()) // If CampCell, the piece cannot move anymore
	{
		return list;
	}

	int i,j;
	int player = cell->getPiece()->getPlayer();


	// Check if the piece can go to an adversary camp
	if (row == 1 && player == -1){
		if(_plateau[0][0]->isEmpty())
		{
			list.push_back(move(0,0));
		}
		if(_plateau[0][1]->isEmpty())
		{
			list.push_back(move(0,1));
		}
	}
	if (row == 8 && player == 1){
		if(_plateau[8][0]->isEmpty())
		{
			list.push_back(move(8,0));
		}
		if(_plateau[8][1]->isEmpty())
		{
			list.push_back(move(8,1));
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
					list.push_back(move(row+i, col+j));
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
			if(! checkCellAddMove(row-i, col-j, player, CellType::BISHOP, &list))
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
			if(! checkCellAddMove(row-i, col+j, player, CellType::BISHOP, &list))
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
			if(! checkCellAddMove(row+i, col-j, player, CellType::BISHOP, &list))
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
			if(! checkCellAddMove(row+i, col+j, player, CellType::BISHOP, &list))
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
			if(! checkCellAddMove(row-i, col, player, CellType::ROCK, &list))
			{
				break;
			}
			++i;
		}

		i = 1;
		while((row+i) <= 8) // Bottom line
		{
			if(! checkCellAddMove(row+i, col, player, CellType::ROCK, &list))
			{
				break;
			}
			++i;
		}

		i = 1;
		while((col-i) >= 1) // Left line
		{
			if(! checkCellAddMove(row, col-i, player, CellType::ROCK, &list))
			{
				break;
			}
			++i;
		}

		i = 1;
		while((col+i) <= 8) // Right line
		{
			if(! checkCellAddMove(row, col+i, player, CellType::ROCK, &list))
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
					list.push_back(move(row-1, col-2));
				}
			}
			if((row+1) <= 8)
			{
				new_cell = (BoardCell*) _plateau[row+1][col-2];
				if(new_cell->isEmpty() || new_cell->getPiece()->getPlayer() == -player)
				{
					list.push_back(move(row+1, col-2));
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
					list.push_back(move(row-1, col+2));
				}
			}
			if((row+1) <= 8)
			{
				new_cell = (BoardCell*) _plateau[row+1][col+2];
				if(new_cell->isEmpty() || new_cell->getPiece()->getPlayer() == -player)
				{
					list.push_back(move(row+1, col+2));
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
					list.push_back(move(row-2, col-1));
				}
			}
			if((col+1) <= 8)
			{
				new_cell = (BoardCell*) _plateau[row-2][col+1];
				if(new_cell->isEmpty() || new_cell->getPiece()->getPlayer() == -player)
				{
					list.push_back(move(row-2, col+1));
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
					list.push_back(move(row+2, col-1));
				}
			}
			if((col+1) <= 8)
			{
				new_cell = (BoardCell*) _plateau[row+2][col+1];
				if(new_cell->isEmpty() || new_cell->getPiece()->getPlayer() == -player)
				{
					list.push_back(move(row+2, col+1));
				}
			}
		}
		break;
	default:
		break;
	}
	return list;
}

// Internal function to populate the vector of moves, called in getAllMoves.
// Returns false if the while loop calling this function has to break.
bool Board::checkCellAddMove(int row, int col, int player, CellType type, std::vector<move> *plist)
{
	BoardCell* new_cell = (BoardCell*) _plateau[row][col];
	if(new_cell->isEmpty())
	{
		plist->push_back(move(row,col));
		if(new_cell->getType() == type)
		{
			return false;
		}
	}
	else
	{
		if(new_cell->getPiece()->getPlayer() == -player)
		{
			plist->push_back(move(row,col));
		}
		return false;
	}
	return true;
}





void
Board::print()
{
	std::string s = "  1  2  3  4  5  6  7  8\n";
	s+= (_plateau[0][0]->isEmpty() ? " " : "X");
	s+= "        White        ";
	s+= (_plateau[0][1]->isEmpty() ? " " : "X");
	s+= "\n";
	for(int i = 1; i<=8; ++i)		// Iterates over rows
	{
		s+= std::to_string(i) + " ";
		for(int j = 1; j<=8; ++j)	// Iterates over columns
		{
			BoardCell * cell = (BoardCell*) _plateau[i][j];
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
	s+= "        Black        ";
	s+= (_plateau[9][1]->isEmpty() ? " " : "X");
	//s+= "\n";
	std::cout << s << std::endl;
}

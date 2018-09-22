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

	switch(((BoardCell*)cell)->getType())
	{
	case CellType::KING:
		for(int i = -1; i<=1; ++i){
			for(int j=-1; j<=1; ++j)
			{
				if(i == 0 && j == 0)
					continue;
				if( (row+i < 1) || (row+i > 8) || (col+j < 1) || (col+j > 8))
					continue;
				Cell* new_cell = _plateau[row+i][col+j];
				if(new_cell->isEmpty() || new_cell->getPiece()->getPlayer() == -player)
				{
					list.push_back(move(row+i, col+j));
				}
			}
		}
		break;
	default:
		break;
	}
	return list;
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

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

	// Init BoardCells
	for(int i = 1; i<=8; ++i)		// Iterates over rows
	{
		_plateau[i] = (Cell**) malloc(8 * sizeof(BoardCell*));
		for(int j = 1; j<=8; ++j)	// Iterates over columns
		{
			_plateau[i][j] = new BoardCell(CellType::KING, i, j);
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










void
Board::print()
{
	std::string s = "";
	s+= (_plateau[0][0]->isEmpty() ? " " : "X");
	s+= "        White        ";
	s+= (_plateau[0][1]->isEmpty() ? " " : "X");
	s+= "\n";
	for(int i = 1; i<=8; ++i)		// Iterates over rows
	{
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
	s+= "\n";
	std::cout << s << std::endl;
}

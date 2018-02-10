#include "Board.hpp"

#include <string>
#include <iostream>

Board::Board()
{
	// Initialization of the BoardCells
	for(int i = 0; i<8; ++i)		// Iterates over rows
	{
		for(int j = 0; j<8; ++j)	// Iterates over columns
		{
			_plateau[i][j].init(CellType::KING, i, j);
		}
	}

	// Initialization of Black and White Pieces
	_piecesW.reserve(8);
	_piecesB.reserve(8);
	for(int j = 0; j<8; ++j)
	{
		// For White
		Piece * pieceW = new Piece(1, &(_plateau[0][j]));
		_piecesW.push_back(pieceW);

		// For Black
		Piece * pieceB = new Piece(-1, &(_plateau[7][j]));
		_piecesB.push_back(pieceB);
	}

	_campW[0].init(1);
	_campW[1].init(2);
	_campB[0].init(-1);
	_campB[1].init(-2);
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
}

void
Board::print()
{
	std::string s = "";
	for(int i = 0; i<8; ++i)		// Iterates over rows
	{
		for(int j = 0; j<8; ++j)	// Iterates over columns
		{
			CellType type = _plateau[i][j].getType();
			if(type == CellType::KING){
				s+= "K ";
			}
		}
		s+="\n";
	}
	std::cout << s << std::endl;
}

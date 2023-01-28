#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "ChessMain.h"

#ifndef BOARD_HEADER
#define BOARD_HEADER


class Board
{
public:
	
	void movePiece(int x1, int y1, int x2, int y2);
	void movePiece(std::bitset<10> piece, int x, int y);
	
	std::bitset<10> getPiece(int x, int y);
	std::vector<std::bitset<10>> getPieces();

	void addPiece(bool color, int type, int x, int y);
	void setPiecePosition(std::bitset<10> piece, int x, int y);
	void setBoardByFEN(std::string FEN);

	std::vector<SDL_Point> AvailableMoves(std::bitset<10> piece);

	static Board* Get();

private:
	Board();
	static Board* instance;
	std::vector<std::bitset<10>> pieces;

};
#endif // !BOARD_HEADER

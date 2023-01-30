#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "ChessMain.h"
#include <map>

#ifndef BOARD_HEADER
#define BOARD_HEADER


class Board
{
public:
	
	void movePiece(int x1, int y1, int x2, int y2);
	void movePiece(bitset<10> piece, int x, int y);
	
	void removePiece(bitset<10> piece);
	bitset<10> getPiece(int x, int y);
	vector<bitset<10>> getPieces();

	void addPiece(bool color, int type, int x, int y);
	void addPiece(bitset<10> piece);
	void setPiecePosition(bitset<10> piece, int x, int y);
	void setBoardByFEN(string FEN);

	bool canPieceEat(int x, int y, bitset<10> piece);
	bool canPieceMove(int x, int y, bitset<10> piece);

	map<int, SDL_Texture*> getTextureMap();

	vector<SDL_Point> AvailableMoves(bitset<10> piece);

	static Board* Get();

private:
	Board();
	static Board* instance;
	vector<bitset<10>> pieces;

	map<int, SDL_Texture*> textureMap;

};
#endif // !BOARD_HEADER

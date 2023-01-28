#include "ChessMain.h"

SDL_Texture* ChessMain::wPawnTexture = nullptr;
SDL_Texture* ChessMain::wKnightTexture = nullptr;
SDL_Texture* ChessMain::wBishopTexture = nullptr;
SDL_Texture* ChessMain::wRookTexture = nullptr;
SDL_Texture* ChessMain::wQueenTexture = nullptr;
SDL_Texture* ChessMain::wKingTexture = nullptr;
SDL_Texture* ChessMain::bPawnTexture = nullptr;
SDL_Texture* ChessMain::bKnightTexture = nullptr;
SDL_Texture* ChessMain::bBishopTexture = nullptr;
SDL_Texture* ChessMain::bRookTexture = nullptr;
SDL_Texture* ChessMain::bQueenTexture = nullptr;
SDL_Texture* ChessMain::bKingTexture = nullptr;

void ChessMain::Initailize(SDL_Renderer* renderer)
{
	wPawnTexture = IMG_LoadTexture(renderer, "Images/White PAWN.png");
	wKnightTexture = IMG_LoadTexture(renderer, "Images/White KNIGHT.png");
	wBishopTexture = IMG_LoadTexture(renderer, "Images/White BISHOP.png");
	wRookTexture = IMG_LoadTexture(renderer, "Images/White ROOK.png");
	wQueenTexture = IMG_LoadTexture(renderer, "Images/White QUEEN.png");
	wKingTexture = IMG_LoadTexture(renderer, "Images/White KING.png");

	bPawnTexture = IMG_LoadTexture(renderer, "Images/Black PAWN.png");
	bKnightTexture = IMG_LoadTexture(renderer, "Images/Black KNIGHT.png");
	bBishopTexture = IMG_LoadTexture(renderer, "Images/Black BISHOP.png");
	bRookTexture = IMG_LoadTexture(renderer, "Images/Black ROOK.png");
	bQueenTexture = IMG_LoadTexture(renderer, "Images/Black QUEEN.png");
	bKingTexture = IMG_LoadTexture(renderer, "Images/Black KING.png");
}
/*
* binary numbers:
001 = 1
010 = 2
011 = 3
100 = 4
101 = 5
110 = 6
111 = 7


1 001-white pawn   (1)
1 010-white knight (2)
1 011-white bishop (3)
1 100-white rook   (4)
1 101-white queen  (5)
1 110-white king   (6)

0 001-black pawn   (1)
0 010-black knight (2)
0 011-black bishop (3)
0 100-black rook   (4)
0 101-black queen  (5)
0 110-black king   (6)
*/
int ChessMain::StrToBinary(const char* pieceType)
{
	if (pieceType == "P")
		return 1001;
	if (pieceType == "K")
		return 1010;
	if (pieceType == "B")
		return 1011;
	if (pieceType == "R")
		return 1100;
	if (pieceType == "Q")
		return 1101;
	if (pieceType == "K")
		return 1110;

	if (pieceType == "p")
		return    1;
	if (pieceType == "k")
		return   10;
	if (pieceType == "b")
		return   11;
	if (pieceType == "r")
		return  100;
	if (pieceType == "q")
		return  101;
	if (pieceType == "k")
		return  110;


	// Invalid
	return 0x000;
}

SDL_Texture* ChessMain::TextureByIndex(int type)
{
	switch (type) {
	case 1001:
		return wPawnTexture;
	case 1010:
		return wKnightTexture;
	case 1011:
		return wBishopTexture;
	case 1100:
		return wRookTexture;
	case 1101:
		return wQueenTexture;
	case 1110:
		return wKingTexture;

	case 1:
		return bPawnTexture;
	case 10:
		return bKnightTexture;
	case 11:
		return bBishopTexture;
	case 100:
		return bRookTexture;
	case 101:
		return bQueenTexture;
	case 110:
		return bKingTexture;


	default:
		return wKingTexture;
	}
}

bool ChessMain::IncludesPoint(vector<SDL_Point> list, SDL_Point point)
{
	for (SDL_Point p : list) {
		if (p.x == point.x && p.y == point.y) {
			return true;
		}
	}
	return false;
}

bool ChessMain::PieceWhite(bitset<10> piece)
{
	return piece[0];
}

bitset<3> ChessMain::PieceType(bitset<10> piece)
{
	int IntType = (piece.to_ulong() & 448);

	bitset<3> result(IntType);

	return result;
}


bitset<3> ChessMain::PieceX(bitset<10> piece)
{
	int IntType = (piece.to_ulong() & 56);

	bitset<3> result(IntType);

	return result;
}

bitset<3> ChessMain::PieceY(bitset<10> piece)
{
	int IntType = (piece.to_ulong() & 7);

	bitset<3> result(IntType);

	return result;
}

void PieceSetPos(std::vector<std::bitset<10>>& pieces, bitset<10>& piece, int x, int y) {
	int index = std::find(pieces.begin(), pieces.end(), piece) - pieces.begin();

	piece <<= 3;
	piece |= x;

	piece <<= 3;
	piece |= x;

	pieces[index] = piece;
}
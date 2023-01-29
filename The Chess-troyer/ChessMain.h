#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include<bitset>
#include <map>


#ifndef CHESSMAIN_HEADER
#define CHESSMAIN_HEADER

using namespace std;

static class ChessMain
{
public:
	static SDL_Texture* wPawnTexture;
	static SDL_Texture* wKnightTexture;
	static SDL_Texture* wBishopTexture;
	static SDL_Texture* wRookTexture;
	static SDL_Texture* wQueenTexture;
	static SDL_Texture* wKingTexture;
	
	static SDL_Texture* bPawnTexture;
	static SDL_Texture* bKnightTexture;
	static SDL_Texture* bBishopTexture;
	static SDL_Texture* bRookTexture;
	static SDL_Texture* bQueenTexture;
	static SDL_Texture* bKingTexture;

	static void Initailize(SDL_Renderer* renderer);

	static SDL_Texture* TextureByIndex(int type);

	static bool IncludesPoint(vector<SDL_Point> list, SDL_Point point);

	static bool PieceWhite(bitset<10> piece);

	static bitset<3> PieceType(bitset<10> piece);
	
	static bitset<3> PieceX(bitset<10> piece);
	
	static bitset<3> PieceY(bitset<10> piece);

	static void PieceSetPos(std::vector<std::bitset<10>>& pieces, bitset<10>& piece, int x, int y);

	static void PieceSetPos(bitset<10>& piece, int x, int y);
};
#endif // !CHESSMAIN_HEADER
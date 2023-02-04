
#include "SDL.h"
#include "Board.h"
#include "ChessMain.h"

using namespace std;

class GameManager
{
public:
	GameManager(SDL_Color movesColor);

	void SetMousePos(int x, int y);

	void OnPress();

	void Draw(SDL_Renderer* renderer);

	void OnRelease();

	void OnLeftMButton();

	int GetX();
	int GetY();

	void updateEnPassantVar(bitset<4>& enPassantVar, SDL_Point move);
	void updateCastlingMoves(bitset<3>& blackCastleState, bitset<3>& whiteCastleState);


private:

	int mouseX = 0, mouseY = 0;
	bool clicking;
	bitset<10> clickedPiece;
	SDL_Color movesColor;
	vector<SDL_Point> moves;
};


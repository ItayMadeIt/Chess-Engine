
#include "SDL.h"
#include "Board.h"
#include "ChessMain.h"

using namespace std;

class GameManager
{
public:
	GameManager();

	void SetMousePos(int x, int y);

	void OnPress();

	void Draw(SDL_Renderer* renderer);

	void OnRelease();

	void OnLeftMButton();

	int GetX();
	int GetY();

private:

	int mouseX = 0, mouseY = 0;
	bool clicking;
	bitset<10> clickedPiece;
};


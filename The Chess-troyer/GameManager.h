
#include "SDL.h"
#include "Board.h"
#include "ChessMain.h"

class GameManager
{
public:
	void SetMousePos(int x, int y);

	void OnPress();

	void OnRelease();

private:
	int mouseX = 0, mouseY = 0;
};


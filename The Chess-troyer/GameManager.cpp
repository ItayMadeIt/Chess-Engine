#include "GameManager.h"

void GameManager::SetMousePos(int x, int y)
{
	mouseX = x;
	mouseY = y;
}

void GameManager::OnPress()
{

	Board::Get()->getPiece(mouseX / 100, mouseY / 100);
}

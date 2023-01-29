#include "GameManager.h"

GameManager::GameManager() {
	
}

void GameManager::SetMousePos(int x, int y)
{
	mouseX = x;
	mouseY = y;
}

void GameManager::OnPress()
{
	// Will be called only if we dont hold a piece

	clickedPiece = Board::Get()->getPiece(mouseX / 100, mouseY / 100);

	Board::Get()->removePiece(clickedPiece);
}

void GameManager::Draw(SDL_Renderer* renderer) {

	SDL_Rect rect = { 0, 0, 100, 100 };

	for (bitset<10> piece : Board::Get()->getPieces()) {
		int key = ChessMain::PieceType(piece).to_ulong() * (ChessMain::PieceWhite(piece) ? 1 : -1);


		rect.x = ChessMain::PieceX(piece).to_ulong() * 100;
		rect.y = ChessMain::PieceY(piece).to_ulong() * 100;
		SDL_RenderCopy(renderer, Board::Get()->getTextureMap()[key], NULL, &rect);
	}

	int key = ChessMain::PieceType(clickedPiece).to_ulong() * (ChessMain::PieceWhite(clickedPiece) ? 1 : -1);

	rect.x = mouseX - rect.w / 2;
	rect.y = mouseY - rect.h / 2;
	SDL_RenderCopy(renderer, Board::Get()->getTextureMap()[key], NULL, &rect);
}

void GameManager::OnRelease()
{
	// Checking if the move is legal
	auto moves = Board::Get()->AvailableMoves(clickedPiece);
	SDL_Point move = { mouseX / 100, mouseY / 100 };

	if (ChessMain::IncludesPoint(moves, move)) {
		// The move is legal

		ChessMain::PieceSetPos(clickedPiece, mouseX / 100, mouseY / 100);
	}
	// Either way move the piece to the correct position
	Board::Get()->addPiece(clickedPiece);

	// Removing it from our memory
	clickedPiece.reset();
}

void GameManager::OnLeftMButton()
{
	// Is it empty (we are not dragging any piece
	if (clickedPiece == bitset<10>(0))
		OnPress();
	else
		OnRelease();
	
}

int GameManager::GetX()
{
	return mouseX;
}

int GameManager::GetY()
{
	return mouseY;
}

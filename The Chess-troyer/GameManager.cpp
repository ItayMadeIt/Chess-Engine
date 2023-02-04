#include "GameManager.h"

GameManager::GameManager(SDL_Color movesColor) {
	clicking = false;
	this->movesColor = movesColor;
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
	moves = Board::Get()->AvailableMoves(clickedPiece);
}

void GameManager::Draw(SDL_Renderer* renderer) {

	SDL_Rect rect = { 0, 0, 100, 100 };

	for (bitset<10> piece : Board::Get()->getPieces()) {
		int key = ChessMain::PieceType(piece).to_ulong() * (ChessMain::PieceWhite(piece) ? 1 : -1);


		rect.x = ChessMain::PieceX(piece).to_ulong() * 100;
		rect.y = ChessMain::PieceY(piece).to_ulong() * 100;
		SDL_RenderCopy(renderer, Board::Get()->getTextureMap()[key], NULL, &rect);
	}

	if (clickedPiece != bitset<10>(0)) {;
		
		rect.w = 90;
		rect.h = 90;
		for (SDL_Point p : moves) {

			rect.x = p.x * 100 + (100-rect.w)/2;
			rect.y = p.y * 100 + (100-rect.h)/2;


			SDL_RenderCopy(renderer, ChessMain::circleTexture, NULL, &rect);
		}

		rect.w = 100;
		rect.h = 100;
	}
	int key = ChessMain::PieceType(clickedPiece).to_ulong() * (ChessMain::PieceWhite(clickedPiece) ? 1 : -1);

	rect.x = mouseX - rect.w / 2;
	rect.y = mouseY - rect.h / 2;
	SDL_RenderCopy(renderer, Board::Get()->getTextureMap()[key], NULL, &rect);
}

void GameManager::OnRelease()
{
	// Checking if the move is legal
	SDL_Point move = { mouseX / 100, mouseY / 100 };

	// If the move is legal
	if (ChessMain::IncludesPoint(moves, move)) {


		// If king (move rooks if castled)
		if (ChessMain::PieceType(clickedPiece) == 6) {
			// Get distance between the king to the position the player want to move it to
			int distance = ChessMain::PieceX(clickedPiece).to_ulong() - mouseX / 100;

			int x = ChessMain::PieceX(clickedPiece).to_ulong();
			int y = ChessMain::PieceY(clickedPiece).to_ulong();
			// If we are clicking on the rook side castling
			if (abs(distance) == 2 && y == mouseY / 100) {

				// Getting the rook starting position
				bool QueenSide = 4 > (mouseX / 100);
				int rookX = QueenSide ? 0 : 7;
				int newRookX = QueenSide ? 3 : 5;

				bitset<10> rook = Board::Get()->getPiece(rookX, y);
				Board::Get()->removePiece(rook);

				ChessMain::PieceSetPos(rook, newRookX, y);
				Board::Get()->addPiece(rook);

			}
		}

		// If is pawn and ate something (eat as en passant)
		if (ChessMain::PieceType(clickedPiece) == 1 && move.x != ChessMain::PieceX(clickedPiece).to_ulong()) {
			int forward = ChessMain::PieceWhite(clickedPiece) ? -1 : 1;

			// Checks if pawn ate the lastEnPassant (last double pawn move) 
			if (ChessMain::PieceMatchesEnPassant(Board::Get()->getPiece(move.x, move.y - forward), Board::Get()->lastEnPassant)) {

				// Removes this pawn from the board
				Board::Get()->removePiece(Board::Get()->getPiece(move.x, move.y - forward));
			}
		}




		// Checking if En Passant Variable needs to be changed
		updateEnPassantVar(Board::Get()->lastEnPassant, move);

		// Checking if castling is not allowed anymore
		updateCastlingMoves(Board::Get()->blackCastleState, Board::Get()->whiteCastleState);



		ChessMain::PieceSetPos(clickedPiece, mouseX / 100, mouseY / 100);

		Board::Get()->removePiece(Board::Get()->getPiece(move.x, move.y));

		// Do something like changing turn



	}
	// Either way move the piece to the correct position
	Board::Get()->addPiece(clickedPiece);

	// Removing it from our memory
	clickedPiece.reset();
}
void GameManager::updateEnPassantVar(bitset<4>& enPassantVar, SDL_Point move) {
	
	enPassantVar.reset();

	if (ChessMain::PieceX(clickedPiece).to_ulong() == move.x && (ChessMain::PieceY(clickedPiece).to_ulong() - 2 == move.y || ChessMain::PieceY(clickedPiece).to_ulong() + 2 == move.y)) {
		enPassantVar = bitset<4>(ChessMain::PieceX(clickedPiece).to_ulong());
		enPassantVar[3] = ChessMain::PieceWhite(clickedPiece);
	}

}

void GameManager::OnLeftMButton() {
	// Is it empty (we are not dragging any piece
	if (clickedPiece == bitset<10>(0))
		OnPress();
	else
		OnRelease();
	
}

int GameManager::GetX() {
	return mouseX;
}

int GameManager::GetY()
{
	return mouseY;
}


void GameManager::updateCastlingMoves(bitset<3>& blackCastleState, bitset<3>& whiteCastleState)
{
	int type = ChessMain::PieceType(clickedPiece).to_ulong();
	bool isWhite = ChessMain::PieceWhite(clickedPiece);

	// If rook

	if (type == 4) {

		// Is White? and if so does we have castling right on at least one of the sides
		if (isWhite && (whiteCastleState[0] || whiteCastleState[2])) {
			
			int X = ChessMain::PieceX(clickedPiece).to_ulong();
			// If the white rook position is in one of the starting positions
			if ((X == 0 || X == 7) && ChessMain::PieceY(clickedPiece) == 7) {

				// Get the index
				int LeftOrRight = (X == 0) ? 0 : 2;
				whiteCastleState[LeftOrRight] = false;
			}


		}
		// Is black? and if so does we have castling right on at least one of the sides
		else if (!isWhite && (blackCastleState[0] || blackCastleState[2])) {

			int X = ChessMain::PieceX(clickedPiece).to_ulong();
			// If the black rook position is in one of the starting positions
			if ((X == 0 || X == 7) && ChessMain::PieceY(clickedPiece) == 0) {

				// Get the index
				int LeftOrRight = (X == 0) ? 0 : 2;
				blackCastleState[LeftOrRight] = false;
			}
		}

	}

	// If king

	if (type == 6) {

		// If white and white did not move the king
		if (isWhite && whiteCastleState[1])
			// Cant castle
			whiteCastleState.reset();

		// If black and black did not move the king
		if (!isWhite && blackCastleState[1])
			blackCastleState.reset();

	}
}

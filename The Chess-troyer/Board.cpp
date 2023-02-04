#include "Board.h"

Board::Board() {
	textureMap[1] = ChessMain::wPawnTexture;
	textureMap[2] = ChessMain::wKnightTexture;
	textureMap[3] = ChessMain::wBishopTexture;
	textureMap[4] = ChessMain::wRookTexture;
	textureMap[5] = ChessMain::wQueenTexture;
	textureMap[6] = ChessMain::wKingTexture;
	
	textureMap[-1] = ChessMain::bPawnTexture;
	textureMap[-2] = ChessMain::bKnightTexture;
	textureMap[-3] = ChessMain::bBishopTexture;
	textureMap[-4] = ChessMain::bRookTexture;
	textureMap[-5] = ChessMain::bQueenTexture;
	textureMap[-6] = ChessMain::bKingTexture;

	whiteCastleState.set();
	blackCastleState.set();
}

void Board::movePiece(int x1, int y1, int x2, int y2)
{
	std::bitset<10> piece = getPiece(x1, y1);

	if (piece != std::bitset<10>(0)) {
		ChessMain::PieceSetPos(pieces, piece, x2, y2);
	}
}

void Board::movePiece(std::bitset<10> piece, int x, int y)
{
	if (piece != std::bitset<10>(0)) {
		ChessMain::PieceSetPos(pieces, piece, x, y);
	}
}

void Board::removePiece(bitset<10> piece)
{
	if (piece != bitset<10>(0)) {
		auto it = std::find(pieces.begin(), pieces.end(), piece);

		if (it != pieces.end()) {
			int key = ChessMain::PieceType(*it).to_ulong() * (int)(2 * ((unsigned int)ChessMain::PieceWhite(*it) - 0.5));
			
			// If rook
			if (ChessMain::PieceType(piece).to_ulong() == 4){
				int X = ChessMain::PieceX(piece).to_ulong();
				int Y = ChessMain::PieceY(piece).to_ulong();
				// If white rook and we can castle and the rook is in the starting position
				if (ChessMain::PieceWhite(piece) && whiteCastleState[1] && Y == 7) {
					int LeftOrRight = (X == 0) ? 0 : 2;
					whiteCastleState[LeftOrRight] = false;
				}
				// If black rook and we can castle and the rook is in the starting position
				if (!ChessMain::PieceWhite(piece) && blackCastleState[1] && Y == 0){
					int LeftOrRight = (X == 0) ? 0 : 2;
					blackCastleState[LeftOrRight] = false;
				}
			}

			pieces.erase(it);
		}
	}
}
bitset<10> Board::getPiece(int x, int y)
{
	auto it = std::find_if(pieces.begin(), pieces.end(), [x, y](bitset<10> piece) {

		int x_ = (int)(piece.to_ulong() >> 3) & 7;
		int y_ = (int)(piece.to_ulong() & 7);

		return x_ == x && y_ == y;
	});

	if (it != pieces.end())
		return *it;
	else
		return std::bitset<10>(0);

}
vector<bitset<10>> Board::getPieces() {
	return pieces;
}
vector<SDL_Point> Board::AvailableMoves(std::bitset<10> piece)
{
	int pieceType_ = ChessMain::PieceType(piece).to_ulong();

	// If we are white its -1 if we are black its 1 (because in SDL y is upside down)
	int forward = ChessMain::PieceWhite(piece) ? -1 : 1;

	vector<SDL_Point> moves;
	int x = ChessMain::PieceX(piece).to_ulong();
	int y = ChessMain::PieceY(piece).to_ulong();

	int knightX[] = { 2,  2,  1,  1, -1, -1, -2, -2 };
	int knightY[] = { 1, -1,  2, -2,  2, -2,  1, -1 };


	int kingX[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
	int kingY[] = { -1, 0, 1, -1, 1, -1, 0, 1 };


	switch (pieceType_) {
	case 1:
		// PAWN

		// Maybe promotion code here
		if (y + forward < 0 || y + forward > 8)
			break;

		// Move forward one square if there is no one there
		if (getPiece(x, y + forward) == bitset<10>(0))
			moves.push_back({ x, y + forward });

		// Move two squares if there is no one there and you are in your starting position
		if (((y == 1 && forward == 1) || (y == 6 && forward == -1)) && getPiece(x, y + forward * 2) == bitset<10>(0))
			moves.push_back({ x, y + forward * 2 });
		
		// En Passant
		if (lastEnPassant != bitset<4>(0)) {

			// Is White
			if (ChessMain::PieceWhite(piece) && y == 3) {
				if (ChessMain::PieceMatchesEnPassant(getPiece(x - 1, y), lastEnPassant)) {
					moves.push_back({ x - 1, y + forward });
				}
				if (ChessMain::PieceMatchesEnPassant(getPiece(x + 1, y), lastEnPassant)) {
					moves.push_back({ x + 1, y + forward });
				}
			}
			

			// Is Black
			if (!ChessMain::PieceWhite(piece) && y == 4) {
				if (ChessMain::PieceMatchesEnPassant(getPiece(x - 1, y), lastEnPassant) && getPiece(x - 1, y + forward) == bitset<10>(0)) {
					moves.push_back({ x - 1, y + forward });
				}

				if (ChessMain::PieceMatchesEnPassant(getPiece(x + 1, y), lastEnPassant) && getPiece(x + 1, y + forward) == bitset<10>(0)) {
					moves.push_back({ x + 1, y + forward });
				}
			}

		}


		// Move a square (left or right) and forward (eat it) if there is the opposite color.
		if (canPieceEat(x - 1, y + forward, piece))
			moves.push_back({ x - 1, y + forward });

		if (canPieceEat(x + 1, y + forward, piece))
			moves.push_back({ x + 1, y + forward });

		break;

	case 2:
		// KNIGHT

		for (int i = 0; i < 8; i++) {
			int newX = x + knightX[i];
			int newY = y + knightY[i];

			if (canPieceMove(newX, newY, piece))
				moves.push_back({ newX, newY });
		}
		break;

	case 3:
		// BISHOP

		for (int TR = 1; TR < 8; TR++)
		{
			// Go Up and Right until you hit a wall (A piece that you don't own)

			// If there is nothing
			if (getPiece(x + TR, y + TR) == bitset<10>(0))
				moves.push_back({ x + TR, y + TR });
			else
			{
				// If opposite colors you can eat it but you cannot go further
				if (ChessMain::PieceWhite(getPiece(x + TR, y + TR)) != ChessMain::PieceWhite(piece))
					moves.push_back({ x + TR, y + TR });

				TR = 8;
			}
		}

		for (int TL = 1; TL < 8; TL++)
		{
			// Go Up and Left until you hit a wall (A piece that you don't own)

			// If there is nothing
			if (getPiece(x - TL, y + TL) == bitset<10>(0))
				moves.push_back({ x - TL, y + TL });
			else {
				// If opposite colors you can eat it but you cannot go further
				if (ChessMain::PieceWhite(getPiece(x - TL, y + TL)) != ChessMain::PieceWhite(piece))
					moves.push_back({ x - TL, y + TL });

				TL = 8;

			}
		}

		for (int BL = 1; BL < 8; BL++)
		{
			// Go Down and Left until you hit a wall (A piece that you don't own)

			// If there is nothing
			if (getPiece(x - BL, y - BL) == bitset<10>(0))
				moves.push_back({ x - BL, y - BL });
			else {
				// If opposite colors you can eat it but you cannot go further
				if (ChessMain::PieceWhite(getPiece(x - BL, y - BL)) != ChessMain::PieceWhite(piece))
					moves.push_back({ x - BL, y - BL });

				BL = 8;
			}
		}

		for (int BR = 1; BR < 8; BR++)
		{
			// Go Down and Right until you hit a wall (A piece that you don't own)

			// If there is nothing
			if (getPiece(x + BR, y - BR) == bitset<10>(0))
				moves.push_back({ x + BR, y - BR });
			else {
				// If opposite colors you can eat it but you cannot go further
				if (ChessMain::PieceWhite(getPiece(x + BR, y - BR)) != ChessMain::PieceWhite(piece))
					moves.push_back({ x + BR, y - BR });

				BR = 8;
			}
		}


		break;

	case 4:
		// ROOK

		for (int R = 1; R < 8; R++)
		{
			// Go Up and Right until you hit a wall (A piece that you don't own)

			// If there is nothing
			if (getPiece(x + R, y) == bitset<10>(0))
				moves.push_back({ x + R, y });
			else
			{
				// If opposite colors you can eat it but you cannot go further
				if (ChessMain::PieceWhite(getPiece(x + R, y)) != ChessMain::PieceWhite(piece))
					moves.push_back({ x + R, y });

				R = 8;
			}
		}

		for (int L = 1; L < 8; L++)
		{
			// Go Up and Right until you hit a wall (A piece that you don't own)

			// If there is nothing
			if (getPiece(x - L, y) == bitset<10>(0))
				moves.push_back({ x - L, y });
			else
			{
				// If opposite colors you can eat it but you cannot go further
				if (ChessMain::PieceWhite(getPiece(x - L, y)) != ChessMain::PieceWhite(piece))
					moves.push_back({ x - L, y });

				L = 8;
			}
		}

		for (int T = 1; T < 8; T++)
		{
			// Go Up and Right until you hit a wall (A piece that you don't own)

			// If there is nothing
			if (getPiece(x, y + T) == bitset<10>(0))
				moves.push_back({ x,  y + T });
			else
			{
				// If opposite colors you can eat it but you cannot go further
				if (ChessMain::PieceWhite(getPiece(x, y + T)) != ChessMain::PieceWhite(piece))
					moves.push_back({ x, y + T });

				T = 8;
			}
		}

		for (int B = 1; B < 8; B++)
		{
			// Go Up and Right until you hit a wall (A piece that you don't own)

			// If there is nothing
			if (getPiece(x, y - B) == bitset<10>(0))
				moves.push_back({ x,  y - B });
			else
			{
				// If opposite colors you can eat it but you cannot go further
				if (ChessMain::PieceWhite(getPiece(x, y - B)) != ChessMain::PieceWhite(piece))
					moves.push_back({ x, y - B });

				B = 8;
			}
		}

		break;

	case 5:
		// QUEEN

		for (int TR = 1; TR < 8; TR++)
		{
			// Go Up and Right until you hit a wall (A piece that you don't own)

			// If there is nothing
			if (getPiece(x + TR, y + TR) == bitset<10>(0))
				moves.push_back({ x + TR, y + TR });
			else
			{
				// If opposite colors you can eat it but you cannot go further
				if (ChessMain::PieceWhite(getPiece(x + TR, y + TR)) != ChessMain::PieceWhite(piece))
					moves.push_back({ x + TR, y + TR });

				TR = 8;
			}
		}

		for (int TL = 1; TL < 8; TL++)
		{
			// Go Up and Left until you hit a wall (A piece that you don't own)

			// If there is nothing
			if (getPiece(x - TL, y + TL) == bitset<10>(0))
				moves.push_back({ x - TL, y + TL });
			else {
				// If opposite colors you can eat it but you cannot go further
				if (ChessMain::PieceWhite(getPiece(x - TL, y + TL)) != ChessMain::PieceWhite(piece))
					moves.push_back({ x - TL, y + TL });

				TL = 8;

			}
		}

		for (int BL = 1; BL < 8; BL++)
		{
			// Go Down and Left until you hit a wall (A piece that you don't own)

			// If there is nothing
			if (getPiece(x - BL, y - BL) == bitset<10>(0))
				moves.push_back({ x - BL, y - BL });
			else {
				// If opposite colors you can eat it but you cannot go further
				if (ChessMain::PieceWhite(getPiece(x - BL, y - BL)) != ChessMain::PieceWhite(piece))
					moves.push_back({ x - BL, y - BL });

				BL = 8;
			}
		}

		for (int BR = 1; BR < 8; BR++)
		{
			// Go Down and Right until you hit a wall (A piece that you don't own)

			// If there is nothing
			if (getPiece(x + BR, y - BR) == bitset<10>(0))
				moves.push_back({ x + BR, y - BR });
			else {
				// If opposite colors you can eat it but you cannot go further
				if (ChessMain::PieceWhite(getPiece(x + BR, y - BR)) != ChessMain::PieceWhite(piece))
					moves.push_back({ x + BR, y - BR });

				BR = 8;
			}
		}


		for (int R = 1; R < 8; R++)
		{
			// Go Up and Right until you hit a wall (A piece that you don't own)

			// If there is nothing
			if (getPiece(x + R, y) == bitset<10>(0))
				moves.push_back({ x + R, y });
			else
			{
				// If opposite colors you can eat it but you cannot go further
				if (ChessMain::PieceWhite(getPiece(x + R, y)) != ChessMain::PieceWhite(piece))
					moves.push_back({ x + R, y });

				R = 8;
			}
		}

		for (int L = 1; L < 8; L++)
		{
			// Go Up and Right until you hit a wall (A piece that you don't own)

			// If there is nothing
			if (getPiece(x - L, y) == bitset<10>(0))
				moves.push_back({ x - L, y });
			else
			{
				// If opposite colors you can eat it but you cannot go further
				if (ChessMain::PieceWhite(getPiece(x - L, y)) != ChessMain::PieceWhite(piece))
					moves.push_back({ x - L, y });

				L = 8;
			}
		}

		for (int T = 1; T < 8; T++)
		{
			// Go Up and Right until you hit a wall (A piece that you don't own)

			// If there is nothing
			if (getPiece(x, y + T) == bitset<10>(0))
				moves.push_back({ x,  y + T });
			else
			{
				// If opposite colors you can eat it but you cannot go further
				if (ChessMain::PieceWhite(getPiece(x, y + T)) != ChessMain::PieceWhite(piece))
					moves.push_back({ x, y + T });

				T = 8;
			}
		}

		for (int B = 1; B < 8; B++)
		{
			// Go Up and Right until you hit a wall (A piece that you don't own)

			// If there is nothing
			if (getPiece(x, y - B) == bitset<10>(0))
				moves.push_back({ x,  y - B });
			else
			{
				// If opposite colors you can eat it but you cannot go further
				if (ChessMain::PieceWhite(getPiece(x, y - B)) != ChessMain::PieceWhite(piece))
					moves.push_back({ x, y - B });

				B = 8;
			}
		}

		break;

	case 6:
		// KING

		for (int i = 0; i < 8; i++) {
			int _x = x + kingX[i];
			int _y = y + kingY[i];
			if ((getPiece(_x, _y) != piece) && canPieceMove(_x, _y, piece))
				moves.push_back({ _x, _y });
		}

		if (ChessMain::PieceWhite(piece) && whiteCastleState[1]) {

			// if can castle right (king side) and there is nothing between the king to the rook
			if (whiteCastleState[2] && getPiece(x + 1, y) == bitset<10>(0) && getPiece(x + 2, y) == bitset<10>(0))
				moves.push_back({ x + 2, y });

			// if can castle left (queen side) and there is nothing between the king to the rook
			if (whiteCastleState[0] && getPiece(x - 1, y) == bitset<10>(0) && getPiece(x - 2, y) == bitset<10>(0) && getPiece(x - 3, y) == bitset<10>(0))
				moves.push_back({ x - 2, y });
		}
		if (!ChessMain::PieceWhite(piece) && blackCastleState[1]) {

			// if can castle right (king side) and there is nothing between the king to the rook
			if (blackCastleState[2] && getPiece(x + 1, y) == bitset<10>(0) && getPiece(x + 2, y) == bitset<10>(0))
				moves.push_back({ x + 2, y });

			// if can castle left (queen side) and there is nothing between the king to the rook
			if (blackCastleState[0] && getPiece(x - 1, y) == bitset<10>(0) && getPiece(x - 2, y) == bitset<10>(0) && getPiece(x - 3, y) == bitset<10>(0))
				moves.push_back({ x - 2, y });
		}
		break;
	}
	return moves;
}

bool Board::canPieceEat(int x, int y, bitset<10> piece)
{
	bool inBounds = x >= 0 && x < 8 && y >= 0 && y < 8;

	bool notNull = getPiece(x, y) != bitset<10>(0);

	bool IsOppositeColor = ChessMain::PieceWhite(getPiece(x, y)) != ChessMain::PieceWhite(piece);
	

	return inBounds && notNull && IsOppositeColor;
}

bool Board::canPieceMove(int x, int y, bitset<10> piece)
{
	bool inBounds = x >= 0 && x < 8 && y >= 0 && y < 8;

	bool isNull = getPiece(x, y) == bitset<10>(0);

	bool isOppositeColor = ChessMain::PieceWhite(getPiece(x, y)) != ChessMain::PieceWhite(piece);

	return inBounds && (isOppositeColor || isNull);
}

void Board::addPiece(bool color, int type, int x, int y)
{
	bitset<10> piece(0);

	piece[9] = color;

	piece |= bitset<10>(type) << 6;

	piece |= bitset<10>(x) << 3;

	piece |= bitset<10>(y) << 0;

	pieces.push_back(piece);
}
void Board::addPiece(bitset<10> piece)
{
	pieces.push_back(piece);
}



void Board::setPiecePosition(bitset<10> piece, int x, int y)
{
	// Seeing if we can eat the piece we go to.
	bitset<10> pieceToRemove = getPiece(x, y);

	// If we can, we do
	if (pieceToRemove != piece)
		removePiece(pieceToRemove);

	// Actually setting the piece position to the new position
	ChessMain::PieceSetPos(pieces, piece,x, y);
}

void Board::setBoardByFEN(std::string FEN)
{
	std::vector<std::string> lines;
	std::istringstream fenStream(FEN);
	std::string line;
	
	while (std::getline(fenStream, line, '/')) {
		lines.push_back(line);
	}

	for (std::string i : lines) {
		const char* chars = i.c_str();
		int x = 0;
		for (int k = 0; k < i.size() ; k++)
		{
			if (std::isdigit(chars[k])) {
				x += (chars[k] - 48);
				continue;
			}

			int y = -1;

			std::string key = i;

			for (int l = 0; l < lines.size(); l++)
			{
				if (lines[l] == key)
				{
					y = l;
					break;
				}
			}

			bool IsUpper = isupper(chars[k]);

			switch (tolower(chars[k])) {
			case'p':
				addPiece(IsUpper, 1, x, y);
				break;
			case'n':
				addPiece(IsUpper, 2, x, y);
				break;
			case'b':
				addPiece(IsUpper, 3, x, y);
				break;						
			case'r':						
				addPiece(IsUpper, 4, x, y);
				break;						
			case'q':						
				addPiece(IsUpper, 5, x, y);
				break;						
			case'k':						
				addPiece(IsUpper, 6, x, y);
				break;
			}
			x++;
		}
	}
}
map<int, SDL_Texture*> Board::getTextureMap() {
	return textureMap;
}

Board* Board::instance = nullptr;

Board* Board::Get()
{
	if (!instance)
		instance = new Board();
	return instance;
}
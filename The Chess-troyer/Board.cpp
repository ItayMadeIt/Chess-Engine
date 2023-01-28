#include "Board.h"

Board::Board() {

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
	int pieceType_ = ChessMain::PieceType(piece).to_ulong() % 1000;

	// If we are white its -1 if we are black its 1 (because in SDL y is upside down)
	int forward = ChessMain::PieceType(piece).to_ulong() >= 1000 ? -1 : 1;

	vector<SDL_Point> moves;
	int x = ChessMain::PieceX(piece).to_ulong();
	int y = ChessMain::PieceY(piece).to_ulong();

	switch (pieceType_) {
	case 1:
		// PAWN

		// Move forward one square if there is no one there
		if (getPiece(x, y + forward) == bitset<10>())
			moves.push_back({ x, y + forward });

		// Move two squares if there is no one there and you are in your starting position
		if (((y == 1 && forward == 1) || (y == 6 && forward == -1)) && getPiece(x, y + forward * 2) == bitset<10>(0))
			moves.push_back({ x, y + forward * 2 });

		// Move a square (left or right) and forward (eat it) if there is the opposite color.
		if (getPiece(x - 1, y + forward) != bitset<10>(0))
			if (ChessMain::PieceWhite(getPiece(x - 1, y + forward)) != ChessMain::PieceWhite(piece)) {
				moves.push_back({ x - 1, y + forward });
			}
		if (getPiece(x + 1, y + forward) != bitset<10>(0))
			if (ChessMain::PieceWhite(getPiece(x + 1, y + forward)) != ChessMain::PieceWhite(piece)) {
				moves.push_back({ x + 1, y + forward });
			}
	}

	return moves;
}

void Board::addPiece(bool color, int type, int x, int y)
{
	bitset<10> piece(0);

	piece[0] = color;
	
	piece |= type << 6;

	piece |= x << 3;
	
	piece |= y << 0;

	pieces.push_back(piece);
}

void Board::setPiecePosition(bitset<10> piece, int x, int y)
{
	// Seeing if we can eat the piece we go to.
	bitset<10> pieceToRemove = getPiece(x, y);

	// If we can, we do
	if (pieceToRemove != bitset<10>(0) && pieceToRemove != piece) {
		auto it = std::find(pieces.begin(), pieces.end(), pieceToRemove);
		int index = it - pieces.begin();

		// We erase it from existence
		pieces.erase(pieces.begin() + index);
	}

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
				x += (int)chars[k];
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

			switch (tolower(chars[k])) {
			case'p':
				addPiece(isupper(chars[k]), 1, x, y);
				break;
			case'n':
				addPiece(isupper(chars[k]), 2, x, y);
				break;
			case'b':
				addPiece(isupper(chars[k]), 3, x, y);
				break;						
			case'r':						
				addPiece(isupper(chars[k]), 4, x, y);
				break;						
			case'q':						
				addPiece(isupper(chars[k]), 5, x, y);
				break;						
			case'k':						
				addPiece(isupper(chars[k]), 6, x, y);
				break;
			}
			x++;
		}
	}


}
Board* Board::instance = nullptr;

Board* Board::Get()
{
	if (!instance)
		instance = new Board();
	return instance;
}
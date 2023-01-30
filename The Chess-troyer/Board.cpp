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
	int pieceType_ = ChessMain::PieceType(piece).to_ulong() % 1000;

	// If we are white its -1 if we are black its 1 (because in SDL y is upside down)
	int forward = ChessMain::PieceWhite(piece) ? -1 : 1;

	vector<SDL_Point> moves;
	int x = ChessMain::PieceX(piece).to_ulong();
	int y = ChessMain::PieceY(piece).to_ulong();

	switch (pieceType_) {
	case 1:
		// PAWN

		// Move forward one square if there is no one there
		if (getPiece(x, y + forward) == bitset<10>(0))
			moves.push_back({ x, y + forward });

		// Move two squares if there is no one there and you are in your starting position
		if (((y == 1 && forward == 1) || (y == 6 && forward == -1)) && getPiece(x, y + forward * 2) == bitset<10>(0))
			moves.push_back({ x, y + forward * 2 });

		// Move a square (left or right) and forward (eat it) if there is the opposite color.
		if (canPieceEat(x - 1, y + forward, piece))
			moves.push_back({ x - 1, y + forward });

		if (canPieceEat(x + 1, y + forward, piece))
			moves.push_back({ x + 1, y + forward });
			

	case 2:
		// KNIGHT

		if (canPieceMove(x + 2, y + 1, piece))
			moves.push_back({ x + 2, y + 1 });


		if (canPieceMove(x + 2, y - 1, piece))
			moves.push_back({ x + 2, y - 1 });


		if (canPieceMove(x + 1, y + 2, piece))
			moves.push_back({ x + 1, y + 2 });


		if (canPieceMove(x + 1, y - 2, piece))
			moves.push_back({ x + 1, y - 2});


		if (canPieceMove(x - 1, y + 2, piece))
			moves.push_back({ x - 1, y + 2 });


		if (canPieceMove(x - 1, y - 2, piece))
			moves.push_back({ x - 1, y - 2 });


		if (canPieceMove(x - 2, y + 1, piece))
			moves.push_back({ x - 2, y + 1 });


		if (canPieceMove(x - 2, y - 1, piece))
			moves.push_back({ x - 2, y - 1 });

	}

	return moves;
}

bool Board::canPieceEat(int x, int y, bitset<10> piece)
{
	bool notNull = getPiece(x, y) != bitset<10>(0);

	bool IsOppositeColor = ChessMain::PieceWhite(getPiece(x, y)) != ChessMain::PieceWhite(piece);

	return notNull && IsOppositeColor;
}

bool Board::canPieceMove(int x, int y, bitset<10> piece)
{
	bool isNull = getPiece(x, y) == bitset<10>(0);

	bool isOppositeColor = ChessMain::PieceWhite(getPiece(x, y)) != ChessMain::PieceWhite(piece);

	return isOppositeColor || isNull;
}

void Board::addPiece(bool color, int type, int x, int y)
{
	bitset<10> piece(0);

	piece[9] = color;

	piece |= bitset<10>(type) << 6;

	piece |= bitset<10>(x) << 3;

	piece |= bitset<10>(y) << 0;

	pieces.push_back(piece);

	cout << "PIECE:" << piece << "C:" << color << "T:" << type << "X:" << x << "Y:" << y << endl;
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
				cout << "Added X:" << (chars[k] - 48) << " Char:" << chars[k] << endl;
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
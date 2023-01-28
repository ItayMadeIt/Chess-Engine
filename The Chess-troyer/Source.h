#ifndef SOURCE_HEADER
#define SOURCE_HEADER

#endif

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <algorithm>

struct Vector2 {
    int x, y;
    Vector2(int x_, int y_) {
        x = x_;
        y = y_;
    }
};

class Source {
public:
    Source(int Width, int Height, int PieceWidth, int PieceHeight);
    

    const int WIDTH, HEIGHT;
    const int PIECE_WIDTH, PIECE_HEIGHT;

    SDL_Window* window;
    SDL_Surface* screenSurface;


    bool init();
    SDL_Surface* loadSurface(std::string path);
    Vector2 getPiecePosByInt(int number);
    Vector2 getPiecePosByMousePos(Vector2 mPos);
    int getIntByPiecePos(Vector2 piecePos);
};

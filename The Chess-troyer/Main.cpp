#include <iostream>
#include <SDL.h>
#include <string>
#include "ChessMain.h"
#include "Board.h"


static int WIDTH = 800, HEIGHT = 800;

static int DESIRED_FPS = 1250;

bool running, fullscreen;

SDL_Renderer* renderer;
SDL_Window* window;

bool clicking;
Piece* clickedPiece;

int mouseX = 0, mouseY = 0;

SDL_Rect basicFrame{ 0, 0, 100, 100 };

SDL_Color darkColor = { 171, 87, 36, 255 };
SDL_Color lightColor = { 237, 167, 26, 255 };



Piece* getClickedPiece(int x, int y) {
    Piece* clickedPiece = Board::Get()->getPiece(x / 100.0f, y / 100.0f);
    if (clickedPiece != nullptr) {
        return clickedPiece;
    }
    return nullptr;
}

void CheckReleasePiece() {
    SDL_Point point = { mouseX / 100, mouseY / 100 };

    if (ChessMain::IncludesPoint(Board::Get()->AvailableMoves(clickedPiece), point)) {
        Board::Get()->setPiecePosition(clickedPiece, mouseX / 100, mouseY / 100);
        clickedPiece->SetPosition(mouseX / 100, mouseY / 100);
        clicking = false;
        clickedPiece = nullptr;
    }
    else {
        Board::Get()->setPiecePosition(clickedPiece, clickedPiece->GetX(), clickedPiece->GetY());
        clickedPiece->SetPosition(clickedPiece->GetX(), clickedPiece->GetY());
        clicking = false;
        clickedPiece = nullptr;
    }
    
}

void handleInput() {
    SDL_Event event;


    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                running = false;
                break;
            case SDLK_f:
                if (!fullscreen) {
                    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                    fullscreen = true;
                }
                else {
                    SDL_SetWindowFullscreen(window, 0);
                    fullscreen = false;
                }
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT && event.button.state == SDL_PRESSED) {
                // Releasing the piece onto "this" square
                if (clicking && clickedPiece != nullptr) {
                    CheckReleasePiece();
                    break;
                }
                // Getting the a new piece if clicked
                clickedPiece = getClickedPiece(mouseX , mouseY);

                if (clickedPiece != nullptr)
                    clicking = true;
                
            }
        case SDL_MOUSEMOTION:
            mouseX = event.motion.x;
            mouseY = event.motion.y;
        }
    }
}
void updateGame() {

}
void drawGraphics() {
    SDL_SetRenderDrawColor(renderer, 194, 100, 33, 255);
    SDL_RenderClear(renderer); //clears the screen

    SDL_SetRenderDrawColor(renderer, 171, 87, 36, 255);

    for (int x = 0; x < 8; x++)
    {
        basicFrame.x = x * 100;
        for (int y = 0; y < 8; y++)
        {
            basicFrame.y = y * 100;
            
            SDL_Color color = (y + x) % 2 ? darkColor : lightColor;
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
            SDL_RenderFillRect(renderer, &basicFrame);
        }
    }
    
    std::vector<Piece*> pieces = Board::Get()->getPieces();
    for (Piece* p : pieces) {
        if (clickedPiece != p)
            p->Draw(renderer);
    }
    if (clickedPiece != nullptr)
        clickedPiece->Draw(renderer, mouseX - 50, mouseY - 50);

    SDL_RenderPresent(renderer);
}

int main(int argc, char** argv) {
    
    running = true;
    fullscreen = false;

    if (SDL_Init(SDL_INIT_EVERYTHING))
        std::cout << "Failed to initialize SDL" << std::endl;

    if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer))
        std::cout << "Failed to create a window" << std::endl;

    ChessMain::Initailize(renderer);

    Board::Get()->setBoardByFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");



    SDL_SetWindowTitle(window, "Chess-troyer");
    SDL_ShowCursor(true);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    
    Uint32 startTime;
    Uint32 endTime;
    float deltaTime = 0;
    float fps = 0;

    while (running) {
        // Getting ticks (FPS)

        startTime = SDL_GetTicks();
        
        updateGame();
        handleInput();
        drawGraphics();

        // Handling FPS (FPS)

        std::string title = "Chess-troyer : " + std::to_string(fps);

        SDL_SetWindowTitle(window, title.c_str());
    
        SDL_Delay( (1000.0f / DESIRED_FPS) - deltaTime);
        endTime = SDL_GetTicks();
        deltaTime = (endTime - startTime) / 1000.0f;
        fps = 1.0f / deltaTime;
    }

    SDL_DestroyTexture(ChessMain::wPawnTexture);
    SDL_DestroyTexture(ChessMain::wKnightTexture);
    SDL_DestroyTexture(ChessMain::wBishopTexture);
    SDL_DestroyTexture(ChessMain::wRookTexture);
    SDL_DestroyTexture(ChessMain::wQueenTexture);
    SDL_DestroyTexture(ChessMain::wKingTexture);

    SDL_DestroyTexture(ChessMain::bPawnTexture);
    SDL_DestroyTexture(ChessMain::bKnightTexture);
    SDL_DestroyTexture(ChessMain::bBishopTexture);
    SDL_DestroyTexture(ChessMain::bRookTexture);
    SDL_DestroyTexture(ChessMain::bQueenTexture);
    SDL_DestroyTexture(ChessMain::bKingTexture);


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

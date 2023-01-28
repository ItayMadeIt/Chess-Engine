#include "Source.h"
#include <string>

    Source::Source(int Width, int Height, int PieceWidth, int PieceHeight) : WIDTH(Width), HEIGHT(Height), PIECE_WIDTH(80), PIECE_HEIGHT(80)
    {
        // variables

        bool quit = false;
        SDL_Event event;
        int x = 0;
        int y = 0;

        // init SDL
        init();
        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

        SDL_Texture* whitePawn = IMG_LoadTexture(renderer, "Images/White PAWN.png");
        SDL_Texture* whiteKnight = IMG_LoadTexture(renderer, "Images/White KNIGHT.png");
        SDL_Texture* whiteBishop = IMG_LoadTexture(renderer, "Images/White BISHOP.png");
        SDL_Texture* whiteRook = IMG_LoadTexture(renderer, "Images/White ROOK.png");
        SDL_Texture* whiteQueen = IMG_LoadTexture(renderer, "Images/White QUEEN.png");
        SDL_Texture* whiteKing = IMG_LoadTexture(renderer, "Images/White KING.png");
        SDL_Texture* blackPawn = IMG_LoadTexture(renderer, "Images/Black PAWN.png");
        SDL_Texture* blackKnight = IMG_LoadTexture(renderer, "Images/Black KNIGHT.png");
        SDL_Texture* blackBishop = IMG_LoadTexture(renderer, "Images/Black BISHOP.png");
        SDL_Texture* blackRook = IMG_LoadTexture(renderer, "Images/Black ROOK.png");
        SDL_Texture* blackQueen = IMG_LoadTexture(renderer, "Images/Black QUEEN.png");
        SDL_Texture* blackKing = IMG_LoadTexture(renderer, "Images/Black KING.png");

        SDL_Texture* boardSprite = IMG_LoadTexture(renderer, "Images/Board.png");

        int index = 0;

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        for (int i = 0; i < 64; i++)
        {
            std::cout << "place:" << i << " piece pos:" << getPiecePosByInt(i).x << ", " << getPiecePosByInt(i).y << std::endl;
        }

        // handle events
        bool followPlayer = true;
        while (!quit)
        {
            Uint64 start = SDL_GetPerformanceCounter();
            while (SDL_PollEvent(&event) != 0)
            {
                switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;

                case SDL_MOUSEMOTION:
                    if (event.type == SDL_MOUSEMOTION) {
                        x = event.motion.x;
                        y = event.motion.y;
                    }
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                       
                    }
                    if (event.button.button == SDL_BUTTON_RIGHT) {

                    }
                }


                SDL_RenderClear(renderer);
                
                SDL_Rect boardRect = { 0, 0, 800, 800 };
                SDL_RenderCopy(renderer, boardSprite, NULL, &boardRect);

                SDL_Rect pieceRect = { x, y, 80, 80 };
                SDL_RenderCopy(renderer, boardSprite, NULL, &pieceRect);


                std::cout << getPiecePosByMousePos(Vector2(x, y)).x << "," << getPiecePosByMousePos(Vector2(x, y)).y << std::endl;

                SDL_RenderPresent(renderer);

                Uint64 end = SDL_GetPerformanceCounter();

                float elapsed = (end - start) / (float)SDL_GetPerformanceFrequency();
                SDL_SetWindowTitle(window, std::to_string(1.0f / elapsed).c_str());
            }
        }
        // cleanup SDL
        SDL_DestroyTexture(whitePawn);
        SDL_DestroyTexture(whiteKnight);
        SDL_DestroyTexture(whiteBishop);
        SDL_DestroyTexture(whiteRook);
        SDL_DestroyTexture(whiteQueen);
        SDL_DestroyTexture(whiteKing);
        SDL_DestroyTexture(blackPawn);
        SDL_DestroyTexture(blackKnight);
        SDL_DestroyTexture(blackBishop);
        SDL_DestroyTexture(blackRook);
        SDL_DestroyTexture(blackQueen);
        SDL_DestroyTexture(blackKing);

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    bool Source::init()
    {
        //Initialization flag
        bool success = true;

        //Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            //Create window
            window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
            if (window == NULL)
            {
                printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
                else
                {
                    //Get window surface
                    screenSurface = SDL_GetWindowSurface(window);
                }
            }
        }

        return success;
    }
    SDL_Surface* Source::loadSurface(std::string path)
    {
        //The final optimized image
        SDL_Surface* optimizedSurface = NULL;

        //Load image at specified path
        SDL_Surface* loadedSurface = IMG_Load(path.c_str());
        if (loadedSurface == NULL)
        {
            printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
        }
        else
        {
            //Convert surface to screen format
            optimizedSurface = SDL_ConvertSurface(loadedSurface, screenSurface->format, 0);
            if (optimizedSurface == NULL)
            {
                printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
            }

            //Get rid of old loaded surface
            SDL_FreeSurface(loadedSurface);
        }

        return optimizedSurface;
    }
    int Source::getIntByPiecePos(Vector2 piecePos) {
        
        return piecePos.x + piecePos.y * 8;
    }
    Vector2 Source::getPiecePosByInt(int number) {
        int row = std::floor(number / 8), column = (number % 8);
        return Vector2(row+1, column+1);
    }
    Vector2 Source::getPiecePosByMousePos(Vector2 mPos)
    {
        int offsetX = 0, offsetY = 0;
        int scaleX = 800, scaleY = 800;
        
        if (mPos.x < 0 || mPos.x > scaleX)
            return Vector2(-1000, -1000);

        if (mPos.y < 0 || mPos.y > scaleY)
            return Vector2(-1000, -1000);

        mPos.y = WIDTH - mPos.y;

        mPos.x -= offsetX;
        mPos.y -= offsetY;

        mPos.x = std::floor(mPos.x / (scaleX / 8));
        mPos.y = std::floor(mPos.y / (scaleY / 8));


        return Vector2(mPos);
    }

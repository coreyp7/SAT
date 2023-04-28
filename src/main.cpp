#include <SDL.h>
#include <stdio.h>

int setupSDL();
void cleanup();
void gameLoop();

const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 500;

SDL_Window* window;
SDL_Renderer* renderer;

typedef struct {
    SDL_Point pos = {250, 250};

    SDL_Point top = {pos.x, pos.y - 50};
    SDL_Point right = {pos.x + 25, pos.y};
    SDL_Point left = {pos.x - 25, pos.y};
} Triangle;

void drawTriangle(Triangle tri){
    SDL_RenderDrawPoint(renderer, tri.pos.x, tri.pos.y);
    SDL_RenderDrawPoint(renderer, tri.top.x, tri.top.y);
    SDL_RenderDrawPoint(renderer, tri.left.x, tri.left.y);
    SDL_RenderDrawPoint(renderer, tri.right.x, tri.right.y);

    SDL_Point points[4];
    points[0] = tri.top;
    points[1] = tri.left;
    points[2] = tri.right;
    points[3] = tri.top;

    // render line going around points
    SDL_RenderDrawLines(renderer, points, 4);
}

void gameLoop(){
    bool quit = false;
    SDL_Event event;

    Uint32 frameStart;
    Uint32 frameFinish;

    float dt;

    Triangle tri;

    while(!quit){
        frameStart = SDL_GetTicks();
        while(SDL_PollEvent(&event) != 0){
            switch(event.type){
                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        drawTriangle(tri);

        SDL_RenderPresent(renderer);

        frameFinish = SDL_GetTicks() - frameStart;
        if((1000/60) > frameFinish){
            SDL_Delay((1000/60) - frameFinish);
        }

    }
}

int main(int argc, char* args[]){
    setupSDL();

    gameLoop();

    cleanup();

    return 0;
}

int setupSDL(){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        return -1;
    }

    window = SDL_CreateWindow("SAT", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 
        SDL_WINDOW_SHOWN);
    if(window == NULL) {
        printf("unsuccessful 2\n");
        return -2;
    }

    renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL){
        printf("unsuccessful 3\n");
        return -3;
    }

    printf("Successfully setup everything.\n");
}

void cleanup(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
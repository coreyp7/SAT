#include <SDL.h>
#include <stdio.h>
#include <vector>

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

    int xvel = 0;
    int yvel = 0;
} Triangle;

enum Direction { UP, DOWN, LEFT, RIGHT };

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

void moveTriangle(Triangle* tri){
    int x = tri->xvel;
    int y = tri->yvel;

    tri->pos.x += x;
    tri->pos.y += y;

    tri->top.x += x;
    tri->top.y += y;
    tri->left.x += x;
    tri->left.y += y;
    tri->right.x += x;
    tri->right.y += y;
}

void gameLoop(){
    bool quit = false;
    SDL_Event event;
    std::vector<SDL_Keycode> keysPressed;

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
                case SDL_KEYDOWN:
                    // keysPressed.push_back(event.key.keysym.sym);
                    // break;
                    switch(event.key.keysym.sym){
                        case SDLK_w:
                            tri.yvel = -5;
                            break;
                        case SDLK_a:
                            tri.xvel = -5;
                            break;
                        case SDLK_s:
                            tri.yvel = 5;
                            break;
                        case SDLK_d:
                            tri.xvel = 5;
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch(event.key.keysym.sym){
                        case SDLK_w:
                            if(tri.yvel != 0){
                                tri.yvel = 0;
                            }
                            break;
                        case SDLK_a:
                            if(tri.xvel != 0){
                                tri.xvel = 0;
                            }
                            break;
                        case SDLK_s:
                            if(tri.yvel != 0){
                                tri.yvel = 0;
                            }
                            break;
                        case SDLK_d:
                            if(tri.xvel != 0){
                                tri.xvel = 0;
                            }
                            break;
                    }
                    break;
            }
        }

        // Update game state
        moveTriangle(&tri);


        // Rendering starts
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
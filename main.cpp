#include <SDL.h>
#include <stdio.h>
#include <vector>
#include <math.h>

//int setupSDL();
//void cleanup();
//void gameLoop();

const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 500;

SDL_Window* window;
SDL_Renderer* renderer;

struct Point {
    double x = 0;
    double y = 0;

    Point(double aX = 0, double aY = 0) {
        x = aX;
        y = aY;
    }

    void translate(double aX, double aY) {
        x += aX;
        y += aY;
    }
    // ANTI-CLOCKWISE ROTATION OF Point ABOUT ORIGIN
    void rotate(double aAngle) {
        double xTemp = x;
        double yTemp = y;

        double theta = aAngle * M_PI / 180;
        double c = cos(theta);
        double s = sin(theta);

        x = xTemp * c - yTemp * s;
        y = xTemp * s + yTemp * c;
    }
};

void drawTriangle(Point triangle[]) {
    SDL_FPoint points[4];
    points[0] = { (float)triangle[0].x, (float)triangle[0].y };
    points[1] = { (float)triangle[1].x, (float)triangle[1].y };
    points[2] = { (float)triangle[2].x, (float)triangle[2].y };
    points[3] = { (float)triangle[0].x, (float)triangle[0].y };
    

    //SDL_RenderDrawPointsF(renderer, points, 3);
    SDL_RenderDrawLinesF(renderer, points, 4);

}

int gameLoop() {
    /*Point triangle[] = { Point(2,1), Point(3,3), Point(4,2) };
    Point geometric_center(0, 0);*/

    //Point triangle[] = { Point(252,251), Point(253,253), Point(254,252) };
    Point triangle[] = { Point(200, 200), Point(270, 190), Point(210, 140) };
    Point geometric_center(0, 0);

    for (auto i : triangle) {
        geometric_center.x += i.x / 3;
        geometric_center.y += i.y / 3;
    }

   /* Point triangle[] = { Point(252,251), Point(253,253), Point(254,252) };
    Point geometric_center(250, 250);*/

    bool quit = false;
    SDL_Event event;
    Uint32 frameStart = SDL_GetTicks();
    Uint32 frameFinish;

    while (!quit) {
        frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_p) {
                    //for (auto i : triangle) {
                    for(int i=0; i<3; i++){
                        printf("(%f, %f) -> ", triangle[i].x, triangle[i].y);
                        triangle[i].translate(-geometric_center.x, -geometric_center.y);
                        triangle[i].rotate(15);
                        triangle[i].translate(geometric_center.x, geometric_center.y);

                        printf("(%f, %f)\n", triangle[i].x, triangle[i].y);
                    }
                }
                break;
            }
        }
        //Update game state


        // Rendering starts
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        drawTriangle(triangle);
        SDL_RenderDrawPointF(renderer, geometric_center.x, geometric_center.y);

        SDL_RenderPresent(renderer);

        frameFinish = SDL_GetTicks() - frameStart;
        if ((1000 / 60) > frameFinish) {
            SDL_Delay((1000 / 60) - frameFinish);
        }

    }

    /*for (auto i : triangle) {
        geometric_center.x += i.x / 3;
        geometric_center.y += i.y / 3;
    }
    printf("Geometric center: (%f, %f)\n", geometric_center.x, geometric_center.y);*/

    /*for (auto i : triangle) {
        printf("(%f, %f) -> ", i.x, i.y);
        i.translate(-geometric_center.x, -geometric_center.y);
        i.rotate(90);
        i.translate(geometric_center.x, geometric_center.y);

        printf("(%f, %f)\n", i.x, i.y);
    }*/
    return 0;
}

void cleanup() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int setupSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return -1;
    }

    window = SDL_CreateWindow("SAT", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("unsuccessful 2\n");
        return -2;
    }

    renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        printf("unsuccessful 3\n");
        return -3;
    }

    printf("Successfully setup everything.\n");
}

int main(int argc, char* args[]) {

    setupSDL();

    gameLoop();

    cleanup();

    return 0;
}

//typedef struct Point {
//    SDL_FPoint point;
//
//    Point(float x, float y) {
//        point.x = x;
//        point.y = y;
//    }
//
//    void translate(float aX, float aY) {
//        point.x += aX;
//        point.y += aY;
//    }
//
//    void rotate(float angle) {
//        float xold = point.x;
//        float yold = point.y;
//
//        double theta = angle * M_PI / 180;
//        double c = cos(theta);
//        double s = sin(theta);
//        
//        point.x = xold * c - yold * s;
//        point.y = xold * s + yold * c;
//
//        printf("(%f, %f) -> (%f, %f)\n", xold, yold, point.x, point.y);
//    }
//} Point;
//
//enum Direction { UP, DOWN, LEFT, RIGHT };
//
//void drawTriangle(Point* triangle[]) {
//    SDL_FPoint points[4];
//    points[0] = triangle[0]->point;
//    points[1] = triangle[1]->point;
//    points[2] = triangle[2]->point;
//    points[3] = triangle[0]->point;
//    
//
//    //SDL_RenderDrawPointsF(renderer, points, 3);
//    SDL_RenderDrawLinesF(renderer, points, 4);
//
//}
//
//
//void gameLoop() {
//    bool quit = false;
//    SDL_Event event;
//    std::vector<SDL_Keycode> keysPressed;
//
//    Uint32 frameStart;
//    Uint32 frameFinish;
//
//    float dt;
//
//    Point* triangle[] = { new Point(240, 250), new Point(260, 250), new Point(250, 240) };
//    Point center(250, 250);
//
//
//    while (!quit) {
//        frameStart = SDL_GetTicks();
//
//        while (SDL_PollEvent(&event) != 0) {
//            switch (event.type) {
//            case SDL_QUIT:
//                quit = true;
//                break;
//            case SDL_KEYDOWN:
//                if (event.key.keysym.sym == SDLK_p) {
//                    for (auto i : triangle) {
//                        i->rotate(90);
//                    }
//                }
//                break;
//            }
//        }
//
//        // Update game state
//        //for (auto i : triangle) {
//            // move center here
//        //}
//        // rotate the triangles points
//        //for (auto i : triangle) {
//        //    i.rotate(90);
//        //}
//
//        // Rendering starts
//        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//        SDL_RenderClear(renderer);
//
//        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
//        drawTriangle(triangle);
//
//        SDL_RenderPresent(renderer);
//
//        frameFinish = SDL_GetTicks() - frameStart;
//        if ((1000 / 60) > frameFinish) {
//            SDL_Delay((1000 / 60) - frameFinish);
//        }
//
//    }
//}
//
//int main(int argc, char* args[]) {
//    setupSDL();
//
//    gameLoop();
//
//    cleanup();
//
//    return 0;
//}
//
//int setupSDL() {
//    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
//        return -1;
//    }
//
//    window = SDL_CreateWindow("SAT", SDL_WINDOWPOS_UNDEFINED,
//        SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT,
//        SDL_WINDOW_SHOWN);
//    if (window == NULL) {
//        printf("unsuccessful 2\n");
//        return -2;
//    }
//
//    renderer = SDL_CreateRenderer(window, -1,
//        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
//    if (renderer == NULL) {
//        printf("unsuccessful 3\n");
//        return -3;
//    }
//
//    printf("Successfully setup everything.\n");
//}
//
//void cleanup() {
//    SDL_DestroyRenderer(renderer);
//    SDL_DestroyWindow(window);
//    SDL_Quit();
//}
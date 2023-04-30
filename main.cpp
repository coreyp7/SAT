#include <SDL.h>
#include <stdio.h>
#include <vector>
#include <math.h>

//int setupSDL();
//void cleanup();
//void gameLoop();

const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 500;
const int ROTATION_ANGLE = 1;

SDL_Window* window;
SDL_Renderer* renderer;


struct Point {
    float x = 0;
    float y = 0;
    float xvel = 0;
    float yvel = 0;

    bool rotatingCW = false;
    bool rotatingCCW = false;

    Point(float aX = 0, float aY = 0) {
        x = aX;
        y = aY;
    }

    void simulate(Point geometric_center) {
        x += xvel;
        y += yvel;

        if (rotatingCW) {
            rotateCW(ROTATION_ANGLE, geometric_center);
        }
        else if (rotatingCCW) {
            rotateCCW(ROTATION_ANGLE, geometric_center);
        }
    }

    void translate(float aX, float aY) {
        x += aX;
        y += aY;
    }
    
    // NOTE: Rotates around origin (0,0).
    void rotateCW(float aAngle, Point geometric_center) {
        translate(-geometric_center.x, -geometric_center.y);

        double xTemp = x;
        double yTemp = y;

        double theta = aAngle * M_PI / 180;
        double c = cos(theta);
        double s = sin(theta);

        x = xTemp * c - yTemp * s;
        y = xTemp * s + yTemp * c;

        translate(geometric_center.x, geometric_center.y);
    }

    // NOTE: Rotates around origin (0,0).
    void rotateCCW(float angle, Point geometric_center) {
        translate(-geometric_center.x, -geometric_center.y);

        double xTemp = x;
        double yTemp = y;

        double theta = angle * M_PI / 180;
        double c = cos(theta);
        double s = sin(theta);

        x = xTemp * c + yTemp * s;
        y = -xTemp * s + yTemp * c;

        translate(geometric_center.x, geometric_center.y);
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

void handleInputs(std::vector<SDL_Keycode> keysDown, std::vector<SDL_Keycode> keysUp,
    Point triangle[], Point* geometric_center) {

    // Handle keys down
    for (int i = 0; i < keysDown.size(); i++) {
        SDL_Keycode key = keysDown[i];
        switch (key) {
        case(SDLK_w):
            geometric_center->yvel = -5;
            for (int j = 0; j < 3; j++) {
                triangle[j].yvel = -5;
            }
            break;
        case(SDLK_a):
            geometric_center->xvel = -5;
            for (int j = 0; j < 3; j++) {
                triangle[j].xvel = -5;
            }
            break;
        case(SDLK_s):
            geometric_center->yvel = 5;
            for (int j = 0; j < 3; j++) {
                triangle[j].yvel = 5;
            }
            break;
        case(SDLK_d):
            geometric_center->xvel = 5;
            for (int j = 0; j < 3; j++) {
                triangle[j].xvel = 5;
            }
            break;
        case(SDLK_p):
            for (int j = 0; j < 3; j++) {
                //triangle[j].translate(-geometric_center->x, -geometric_center->y);
                /*triangle[j].rotateCW(10);*/
                triangle[j].rotatingCW = true;
                //triangle[j].translate(geometric_center->x, geometric_center->y);
            }
            break;
        case(SDLK_o):
            for (int j = 0; j < 3; j++) {
                //triangle[j].translate(-geometric_center->x, -geometric_center->y);
                /*triangle[j].rotateCCW(10);*/
                triangle[j].rotatingCCW = true;
                //triangle[j].translate(geometric_center->x, geometric_center->y);
            }
            break;
        }
    }

    // Handle keys up
    for (int i = 0; i < keysUp.size(); i++) {
        SDL_Keycode key = keysUp[i];
        switch (key) {
        case(SDLK_w):
        case(SDLK_s):
            printf("Cancelled yvel");
            geometric_center->yvel = 0;
            for (int j = 0; j < 3; j++) {
                triangle[j].yvel = 0;
            }
            break;
        case(SDLK_a):
        case(SDLK_d):
            printf("Cancelled xvel");
            geometric_center->xvel = 0;
            for (int j = 0; j < 3; j++) {
                triangle[j].xvel = 0;
            }
            break;
        case(SDLK_p):
            for (int j = 0; j < 3; j++) {
                triangle[j].rotatingCW = false;
            }
            break;
        case(SDLK_o):
            for (int j = 0; j < 3; j++) {
                triangle[j].rotatingCCW = false;
            }
            break;
        }
    }
}

int gameLoop() {
    /*Point triangle[] = { Point(2,1), Point(3,3), Point(4,2) };
    Point geometric_center(0, 0);*/

    //Point triangle[] = { Point(252,251), Point(253,253), Point(254,252) };
    Point triangle[] = { Point(200, 200), Point(270, 190), Point(210, 140) };
    //Point triangle[] = { Point(100, 200), Point(250, 50), Point(400, 200) };
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
    Uint32 lastPhysicsUpdate = SDL_GetTicks();
    float dt;

    std::vector<SDL_Keycode> keysDown;
    std::vector<SDL_Keycode> keysUp;

    while (!quit) {
        frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                // add to vector
                keysDown.push_back(event.key.keysym.sym);
                break;
            case SDL_KEYUP:
                keysUp.push_back(event.key.keysym.sym);
                break;
            }
        }

        //Update game state
        //dt = (SDL_GetTicks() - lastPhysicsUpdate) / 1000.0f;
        handleInputs(keysDown, keysUp, triangle, &geometric_center);

        geometric_center.simulate(geometric_center);
        for (int i = 0; i < 3; i++) {
            triangle[i].simulate(geometric_center);
        }

        keysDown.clear();
        keysUp.clear();
        lastPhysicsUpdate = SDL_GetTicks();

        // Rendering starts
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        drawTriangle(triangle);
        SDL_RenderDrawPointF(renderer, geometric_center.x, geometric_center.y);

        for (int i = 0; i < 3; i++) {
            Point currPoint = triangle[i];
            Point nextPoint = (i == 2) ? triangle[0] : triangle[i+1];
            float dx = nextPoint.x - currPoint.x;
            float dy = nextPoint.y - currPoint.y;
            Point edge = { dx, dy };
            
            Point perpLine = { -edge.y, edge.x };

            Point perpLineRenderPoint = { edge.x + perpLine.x, edge.y + perpLine.y };

            SDL_RenderDrawLineF(renderer, edge.x, edge.y, perpLineRenderPoint.x, perpLineRenderPoint.y);
        }

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

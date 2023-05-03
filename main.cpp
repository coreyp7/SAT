#include <SDL.h>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <stack>
#include <array>

#include "Polygon.h"

//int setupSDL();
//void cleanup();
//void gameLoop();

const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 500;
const int ROTATION_ANGLE = 15;

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

void handleInputs(std::vector<SDL_Keycode> keysDown, std::vector<SDL_Keycode> keysUp,
    Polygon* polygon) {

    // Handle keys down
    for (int i = 0; i < keysDown.size(); i++) {
        SDL_Keycode key = keysDown[i];
        switch (key) {
        case(SDLK_w):
            polygon->yvel = -5;
            break;
        case(SDLK_a):
            polygon->xvel = -5;
            break;
        case(SDLK_s):
            polygon->yvel = 5;
            break;
        case(SDLK_d):
            polygon->xvel = 5;
            break;
        case(SDLK_p):
            polygon->rotatingCW = true;
            break;
        case(SDLK_o):
            polygon->rotatingCCW = true;
            break;
        }
    }

    // Handle keys up
    for (int i = 0; i < keysUp.size(); i++) {
        SDL_Keycode key = keysUp[i];
        switch (key) {
        case(SDLK_w):
        case(SDLK_s):
            polygon->yvel = 0;
            break;
        case(SDLK_a):
        case(SDLK_d):
            polygon->xvel = 0;
            break;
        case(SDLK_p):
            polygon->rotatingCW = false;
            break;
        case(SDLK_o):
            polygon->rotatingCCW = false;
            break;
        }
    }
}

bool SAT_collision(Polygon* polygon, Polygon* polygon2) {
    //std::stack<Point> perpStack;
    /*SDL_FPoint perpStack[polygon.verticesSize];*/
    //std::stack<SDL_FPoint> perpStack;
    std::vector<SDL_FPoint> perpStack;
    
    // Loop through all edges in triangle and put normal vectors on stack.
    // Loop through all edges in polygon and put normal vectors on stack.
    for (int i = 0; i < polygon->verticesSize; i++) {
        SDL_FPoint currPoint = polygon->vertices[i];
        SDL_FPoint nextPoint = (i == polygon->verticesSize-1) ? polygon->vertices[0] : polygon->vertices[i + 1];
        float dx = nextPoint.x - currPoint.x;
        float dy = nextPoint.y - currPoint.y;
        SDL_FPoint edge = { dx, dy };
        SDL_FPoint perpLine = { -edge.y, edge.x };

        perpStack.push_back(perpLine);
        //perpStack[i] = perpLine;
    }

    for (int i = 0; i < polygon2->verticesSize; i++) {
        SDL_FPoint currPoint = polygon2->vertices[i];
        SDL_FPoint nextPoint = (i == polygon2->verticesSize - 1) ? polygon2->vertices[0] : polygon2->vertices[i + 1];
        float dx = nextPoint.x - currPoint.x;
        float dy = nextPoint.y - currPoint.y;
        SDL_FPoint edge = { dx, dy };
        SDL_FPoint perpLine = { -edge.y, edge.x };

        perpStack.push_back(perpLine);
        //perpStack[i] = perpLine;
    }

    float tmin, tmax, smin, smax;
    SDL_FPoint perpLine;
    //int perpStackSize = perpStack.size();
    // Ok, we have our stack of perp lines. Let's go through all of them.
    for (int i = 0; i < polygon->verticesSize + 4; i++) {
        //perpLine = perpStack[i];
        perpLine = perpStack[i];
        tmin = NULL;
        tmax = NULL;
        smin = NULL;
        smax = NULL;
        
        // Multiply each vertices in polygon by normal, set as max/min.
        // (Do this for both polygons)
        for (int j = 0; j < polygon->verticesSize; j++) {
            float dot = ((polygon->vertices[j].x * perpLine.x) + (polygon->vertices[j].y * perpLine.y));
            if (tmax == NULL || dot > tmax) {
                tmax = dot;
            }
            if (tmin == NULL || dot < tmin) {
                tmin = dot;
            }
        }

        for (int j = 0; j < polygon2->verticesSize; j++) {
            float dot = ((polygon2->vertices[j].x * perpLine.x) + (polygon2->vertices[j].y * perpLine.y));
            if (smax == NULL || dot > smax) {
                smax = dot;
            }
            if (smin == NULL || dot < smin) {
                smin = dot;
            }
        }

        if (((tmin < smax) && (tmin > smin)) || ((smin < tmax) && (smin > tmin))) {
            // Continue with loop, there's a collision here.
            // All done, get rid of this normal.
            //perpStack.pop();
        }
        else {
            return false;
        }


        //// All done, get rid of this normal.
        /*perpStack.pop();*/
    }
    return true;
}

int gameLoop() {
    /*Point triangle[] = { Point(2,1), Point(3,3), Point(4,2) };
    Point geometric_center(0, 0);*/
    //SDL_FRect square = { 300, 300, 75, 75 };

    SDL_FPoint trianglePoints[3] = { {200, 200}, {270, 180}, {230, 120} };
    Polygon triangle = Polygon(trianglePoints, 3);
    SDL_FPoint squarePoints[4] = { {300, 300}, {350, 300}, {350, 350}, {300, 350} };
    Polygon square = Polygon(squarePoints, 4);

   /* Point triangle[] = { Point(252,251), Point(253,253), Point(254,252) };
    Point geometric_center(250, 250);*/

    bool quit = false;
    SDL_Event event;
    Uint32 frameStart = SDL_GetTicks();
    Uint32 frameFinish;
    Uint32 lastPhysicsUpdate = SDL_GetTicks();
    //float dt;

    std::vector<SDL_Keycode> keysDown;
    std::vector<SDL_Keycode> keysUp;

    //triangle.rotate(5); // weird fix: don't know what this is about

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
        
        //handleInputs(keysDown, keysUp, &triangle);
        //triangle.simulate();
        handleInputs(keysDown, keysUp, &square);
        square.simulate();

        keysDown.clear();
        keysUp.clear();
        lastPhysicsUpdate = SDL_GetTicks();

        bool collision = SAT_collision(&triangle, &square);
        //printf("%s\n", collision ? "true" : "");

        // Rendering starts
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        if (collision) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        }
        else {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        }
        //SDL_RenderDrawRectF(renderer, &square);

        //SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        triangle.render(renderer);
        square.render(renderer);

        //SDL_RenderDrawPointF(renderer, geometric_center.x, geometric_center.y);

        // Draw normals ( and will do collision stuff above in similar manner )
        //for (int i = 0; i < 3; i++) {
        //    Point currPoint = triangle[i];
        //    Point nextPoint = (i == 2) ? triangle[0] : triangle[i+1];
        //    float dx = nextPoint.x - currPoint.x;
        //    float dy = nextPoint.y - currPoint.y;
        //    Point edge = { dx, dy };
        //    Point perpLine = { -edge.y, edge.x };

        //    // Actual work not done yet.


        //    // Render normals at the middle of the edge.
        //    Point startPointRender = {
        //        (currPoint.x + nextPoint.x)/2, (currPoint.y + nextPoint.y)/2
        //    };

        //    Point endPointRender = {
        //        startPointRender.x + perpLine.x, startPointRender.y + perpLine.y
        //    };

        //    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        //    SDL_RenderDrawLineF(renderer, startPointRender.x, startPointRender.y, endPointRender.x, endPointRender.y);
        //}

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
        //printf("unsuccessful 2\n");
        return -2;
    }

    renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        printf("unsuccessful 3\n");
        return -3;
    }

    //printf("Successfully setup everything.\n");
}

int main(int argc, char* args[]) {

    setupSDL();

    gameLoop();

    cleanup();

    return 0;
}

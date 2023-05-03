#pragma once
#include <SDL.h>
//#include <vector>

class Polygon {
public:
	// Array of vertices of this polygon
	SDL_FPoint* vertices;
	int verticesSize;
	SDL_FPoint centerPoint;

	float xvel, yvel;

	bool rotatingCW, rotatingCCW;

	// For now, polygon will be hardcoded as a triangle only.
	// Then later I'll make the constructor work properly to
	// accept any set of points.
	Polygon(SDL_FPoint* points, int size);
	~Polygon();
	
	// Function for rotating entire shape.
	// Positive for CW, negative for CCW.
	void rotate(float angle);

	// Moves this (and all its points) according to
	// its 'velocity'. 
	void simulate();

	// For simplicity, will just render from the object.
	// Simple demo, so is fine in this context.
	void render(SDL_Renderer* renderer);

private:
	void translate(float x, float y);
};
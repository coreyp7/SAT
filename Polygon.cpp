#include "Polygon.h"

Polygon::Polygon(SDL_FPoint* points, int size) {
	/*vertices = new SDL_FPoint[3];
	verticesSize = 3;

	vertices[0] = { 200, 200 };
	vertices[1] = { 270, 180 };
	vertices[2] = { 230, 120 };*/

	verticesSize = size;
	vertices = new SDL_FPoint[size];
	for (int i = 0; i < size; i++) {
		vertices[i] = points[i];
	}

	centerPoint.x = 0;
	centerPoint.y = 0;
	
	for (int i = 0; i < verticesSize; i++) {
		centerPoint.x += vertices[i].x / verticesSize;
		centerPoint.y += vertices[i].y / verticesSize;
	}

	xvel = 0;
	yvel = 0;

	rotatingCW = false;
	rotatingCCW = false;
}

Polygon::~Polygon() {
	//delete[] vertices; // TODO: check that this is cool
}

void Polygon::render(SDL_Renderer* renderer) {
	// Render polygon edges
	// SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderDrawPoint(renderer, centerPoint.x, centerPoint.y);
	SDL_RenderDrawLinesF(renderer, vertices, verticesSize);
	SDL_RenderDrawLine(renderer, 
		vertices[verticesSize-1].x, vertices[verticesSize-1].y, 
		vertices[0].x, vertices[0].y);

	// Draw normals from each edge
	for (int i = 0; i < verticesSize; i++) {
		SDL_FPoint currPoint = vertices[i];
		SDL_FPoint nextPoint = (i == verticesSize-1) ? vertices[0] : vertices[i + 1];
		float dx = nextPoint.x - currPoint.x;
		float dy = nextPoint.y - currPoint.y;
		SDL_FPoint edge = { dx, dy };
		SDL_FPoint perpLine = { -edge.y, edge.x };

		// Render normals at the middle of the edge.
		SDL_FPoint startPointRender = {
			(currPoint.x + nextPoint.x) / 2, (currPoint.y + nextPoint.y) / 2
		};

		SDL_FPoint endPointRender = {
			startPointRender.x + perpLine.x, startPointRender.y + perpLine.y
		};

		//SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_RenderDrawLineF(renderer, startPointRender.x, startPointRender.y, endPointRender.x, endPointRender.y);
	}
}

void Polygon::simulate() {
	centerPoint.x += xvel;
	centerPoint.y += yvel;

	for (int i = 0; i < verticesSize; i++) {
		vertices[i].x += xvel;
		vertices[i].y += yvel;
	}

	if (rotatingCW) {
		rotate(5);
	}
	else if (rotatingCCW) {
		rotate(-5);
	}
}

// NOTE: angle is in degrees.
void Polygon::rotate(float angle) {
	for (int i = 0; i < verticesSize; i++) {
		SDL_FPoint currPoint = vertices[i];

		// We're rotating it around the origin.
		float xOrigin = currPoint.x - centerPoint.x;
		float yOrigin = currPoint.y - centerPoint.y;

		double theta = angle * M_PI / 180;

		float rotatedx = xOrigin * cos(theta) - yOrigin * sin(theta);
		float rotatedy = xOrigin * sin(theta) + yOrigin * cos(theta);

		// translate rotatedx back to position 
		// around center of polygon's real position.
		vertices[i].x = rotatedx + centerPoint.x;
		vertices[i].y = rotatedy + centerPoint.y;
	}
}

//// Translates only the points, NOT the center.
//// Used when rotating the shape only.
//void Polygon::translate(float x, float y) {
//	/*centerPoint.x += x;
//	centerPoint.y += y;*/
//	/*for (int i = 0; i < verticesSize; i++) {
//		vertices[i].x = x;
//		vertices[i].y = y;
//	}*/
//}


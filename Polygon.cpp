#include "Polygon.h"

Polygon::Polygon() {
	vertices = new SDL_FPoint[3];
	verticesSize = 3;

	vertices[0] = { 200, 200 };
	vertices[1] = { 270, 180 };
	vertices[2] = { 230, 120 };

	centerPoint.x = 0;
	centerPoint.y = 0;
	
	for (int i = 0; i < verticesSize; i++) {
		centerPoint.x += vertices[i].x / verticesSize;
		centerPoint.y += vertices[i].y / verticesSize;
	}

	xvel = 0;
	yvel = 0;
}

Polygon::~Polygon() {
	delete vertices; // TODO: check that this is cool
}

void Polygon::render(SDL_Renderer* renderer) {
	// Render polygon edges
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
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

		// Actual work not done yet.


		// Render normals at the middle of the edge.
		SDL_FPoint startPointRender = {
			(currPoint.x + nextPoint.x) / 2, (currPoint.y + nextPoint.y) / 2
		};

		SDL_FPoint endPointRender = {
			startPointRender.x + perpLine.x, startPointRender.y + perpLine.y
		};

		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		SDL_RenderDrawLineF(renderer, startPointRender.x, startPointRender.y, endPointRender.x, endPointRender.y);
	}
}

void Polygon::simulate() {

}
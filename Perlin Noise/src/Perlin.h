#pragma once

#include <SDL.h>

#include <iostream>
#include <vector>
#include <Isometric.h>

typedef struct {
	float x, y;
} vector2;

class Perlin {
public:
	int octaves;
	int grid_size;
	bool showColor;
	int seed;
	int size;

	float waterHeight = -0.4;
	float sandHeight = -0.3;
	float grassHeight = 0.1;
	float hillHeight = 0.3;
	float rockHeight = 0.7;
	float mountainsHeight = 1;

	float heights[200 * 200][2] = { {} };

	Uint8* pixels;

	SDL_Texture* texture;

	Perlin(SDL_Renderer* renderer, int octaves, int grid_size, bool showColor, int seed, int size);
	float interpolate(float a0, float a1, float w);
	vector2 randomGradient(float ix, float iy);
	float dotGridGradient(int ix, int iy, float x, float y);
	float perlin(float x, float y);
	void genPerlin(std::vector<Cube>& Cubes, float worldBlockOffset, int xCamOf, int yCamOf);
	void UpdateTexture();
	void RenderTexture(SDL_Renderer* renderer);
};

void genTree(int x, int y, std::vector<Cube>& Cubes, float height, float worldBlockOffset);

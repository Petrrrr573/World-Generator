#pragma once

#include <SDL.h>

#include <iostream>

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

	Uint8* pixels;

	SDL_Texture* texture;

	Perlin(SDL_Renderer* renderer, int octaves, int grid_size, bool showColor, int seed, int size);
	float interpolate(float a0, float a1, float w);
	vector2 randomGradient(float ix, float iy);
	float dotGridGradient(int ix, int iy, float x, float y);
	float perlin(float x, float y);
	void genPerlin();
	void UpdateTexture();
	void RenderTexture(SDL_Renderer* renderer);
};

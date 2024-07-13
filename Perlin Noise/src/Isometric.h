#pragma once

#include <SDL.h>
#include <SDL_image.h>

class Isometric {
public:
	int worldSize;
	float worldScale;
	float waveFreq;
	int waveAmpl;

	SDL_Texture* waterTexture;
	SDL_Texture* sandTexture;
	SDL_Texture* grassTexture;
	SDL_Texture* hillTexture;
	SDL_Texture* rockTexture;
	SDL_Texture* mountainsTexture;

	SDL_Texture* texture;

	float worldHeight = 20;
	float worldBlockOffset = 2 / worldHeight;

	int xCamOf = 0, yCamOf = 0;

	Isometric(int size, float scale, float freq, int ampl, SDL_Renderer* renderer);

	void waves(SDL_Renderer* renderer, float heights[96*96][2]);
};
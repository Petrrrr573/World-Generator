#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <vector>

typedef struct {
	int x, y;
	float height;
	int type;
} Cube;

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

	SDL_Texture* woodTexture;
	SDL_Texture* woodTexture2;
	SDL_Texture* leafTexture;
	SDL_Texture* leafTexture2;

	SDL_Texture* outlineTexture;

	SDL_Texture* texture;

	std::vector<Cube> Cubes;

	float worldHeight = 20;
	float worldBlockOffset = 2 / worldHeight;

	int xCamOf = 0, yCamOf = 0;

	Isometric(int size, float scale, float freq, int ampl, SDL_Renderer* renderer);

	void waves(SDL_Renderer* renderer, float heights[200*200][2]);
};
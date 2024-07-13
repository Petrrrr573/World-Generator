#include "Isometric.h"
#include <iostream>
#include <chrono>

Isometric::Isometric(int size, float scale, float freq, int ampl, SDL_Renderer* renderer) {
	worldSize = size;
	worldScale = scale;
	waveFreq = freq;
	waveAmpl = ampl;


	waterTexture = IMG_LoadTexture(renderer, "C:/Dev/Perlin Noise/Perlin Noise/src/assets/waterBlock.png");
    sandTexture = IMG_LoadTexture(renderer, "C:/Dev/Perlin Noise/Perlin Noise/src/assets/sandBlock.png");
    grassTexture = IMG_LoadTexture(renderer, "C:/Dev/Perlin Noise/Perlin Noise/src/assets/grassBlock.png");
    hillTexture = IMG_LoadTexture(renderer, "C:/Dev/Perlin Noise/Perlin Noise/src/assets/grassBlock.png");
    rockTexture = IMG_LoadTexture(renderer, "C:/Dev/Perlin Noise/Perlin Noise/src/assets/rockBlock.png");
    mountainsTexture = IMG_LoadTexture(renderer, "C:/Dev/Perlin Noise/Perlin Noise/src/assets/snowBlock.png");
}

void Isometric::waves(SDL_Renderer* renderer, float heights[96 * 96][2]) {

    // Get current time in milliseconds
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    for (int x = 0; x < worldSize; x++) {
        for (int y = 0; y < worldSize; y++) {
            double freq = (double)waveFreq;
            double offset = 0;
            int type = heights[y * worldSize + x][0];
            switch (type) {
            case 6:
                offset += floor(heights[y * worldSize + x][1] / worldBlockOffset);
                texture = mountainsTexture;
                break;
            case 5:
                offset += floor(heights[y * worldSize + x][1] / worldBlockOffset);
                texture = rockTexture;
                break;
            case 4:
                offset += floor(heights[y * worldSize + x][1] / worldBlockOffset);
                texture = grassTexture;
                break;
            case 3:
                offset += floor(heights[y * worldSize + x][1] / worldBlockOffset);
                texture = grassTexture;
                break;
            case 2:
                offset += floor(heights[y * worldSize + x][1] / worldBlockOffset);
                texture = sandTexture;
                break;
            case  1:
                offset += floor(heights[y * worldSize + x][1] / worldBlockOffset);
                texture = waterTexture;
                break;
            }

            float wave_sin = waveAmpl * std::sin(freq * millis + x * 0.5);
            float wave_cos = waveAmpl * std::cos(freq * millis + y * 0.5);

            float x0 = x * 1 * 48 * worldScale + y * -1 * 48 * worldScale;
            float y0 = x * 0.5 * 48 * worldScale + y * 0.5 * 48 * worldScale;

            for (int l = -worldHeight/2; l < offset; l++) {
                SDL_Rect rect1 = { x0 - 48 * worldScale + 1280 / 2 + xCamOf*96, y0 - 48 * worldScale + 720 / 4 - l*48*worldScale - yCamOf*96, 96 * worldScale, 96 * worldScale };

                if (heights[y * worldSize + x][0] == 1) {
                    rect1.h += wave_sin + wave_cos;
                }
                SDL_RenderCopy(renderer, texture, nullptr, &rect1);
            }

        }
    }
}
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

void Isometric::waves(SDL_Renderer* renderer, float heights[200 * 200][2]) {

    // Get current time in milliseconds
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    for (int x = 0; x < worldSize; x++) {
        for (int y = 0; y < worldSize; y++) {
            double freq = (double)waveFreq;
            double offset = 0;
            int type = heights[(y+yCamOf) * 200 + x+xCamOf][0];
            switch (type) {
            case 6:
                offset += floor(heights[(y + yCamOf) * 200 + x + xCamOf][1] / worldBlockOffset);
                texture = mountainsTexture;
                break;
            case 5:
                offset += floor(heights[(y + yCamOf) * 200 + x + xCamOf][1] / worldBlockOffset);
                texture = rockTexture;
                break;
            case 4:
                offset += floor(heights[(y + yCamOf) * 200 + x + xCamOf][1] / worldBlockOffset);
                texture = grassTexture;
                break;
            case 3:
                offset += floor(heights[(y + yCamOf) * 200 + x + xCamOf][1] / worldBlockOffset);
                texture = grassTexture;
                break;
            case 2:
                offset += floor(heights[(y + yCamOf) * 200 + x + xCamOf][1] / worldBlockOffset);
                texture = sandTexture;
                break;
            case  1:
                offset += floor(heights[(y + yCamOf) * 200 + x + xCamOf][1] / worldBlockOffset);
                texture = waterTexture;
                break;
            }

            float wave_sin = waveAmpl * std::sin(freq * millis + x * 0.5);
            float wave_cos = waveAmpl * std::cos(freq * millis + y * 0.5);

            float x0 = x * 1 * 48 * worldScale + y * -1 * 48 * worldScale;
            float y0 = x * 0.5 * 48 * worldScale + y * 0.5 * 48 * worldScale;

            if (y+1 < worldSize && x+1 < worldSize) {
                for (int l = floor(heights[(y+1+yCamOf) * 200 + x+xCamOf][1] / worldBlockOffset); l < offset; l++) {
                    SDL_Rect rect1 = { x0 - 48 * worldScale + 1280 / 2, y0 - 48 * worldScale + 720 / 4 - l * 48 * worldScale, 96 * worldScale, 96 * worldScale };

                    if (heights[(y + yCamOf) * 200 + x + xCamOf][0] == 1) {
                        rect1.y += wave_sin + wave_cos;
                    }

                    // Fixes the weird gaps between sqaures
                    rect1.w *= 1.1;

                    SDL_RenderCopy(renderer, texture, nullptr, &rect1);
                }

                for (int l = floor(heights[(y+yCamOf) * 200 + x + 1 + xCamOf][1] / worldBlockOffset); l < offset; l++) {
                    SDL_Rect rect1 = { x0 - 48 * worldScale + 1280 / 2, y0 - 48 * worldScale + 720 / 4 - l * 48 * worldScale, 96 * worldScale, 96 * worldScale };

                    if (heights[(y + yCamOf) * 200 + x + xCamOf][0] == 1) {
                        rect1.y += wave_sin + wave_cos;
                    }

                    // Fixes the weird gaps between sqaures
                    rect1.w *= 1.1;

                    SDL_RenderCopy(renderer, texture, nullptr, &rect1);
                }
            }
            else {
                for (int l = -worldHeight/2; l < offset; l++) {
                    SDL_Rect rect1 = { x0 - 48 * worldScale + 1280 / 2, y0 - 48 * worldScale + 720 / 4 - l * 48 * worldScale, 96 * worldScale, 96 * worldScale };

                    if (heights[(y + yCamOf) * 200 + x + xCamOf][0] == 1) {
                        rect1.y += wave_sin + wave_cos;
                    }

                    // Fixes the weird gaps between sqaures
                    rect1.w *= 1.1;

                    SDL_RenderCopy(renderer, texture, nullptr, &rect1);
                }
            }

            SDL_Rect rect1 = { x0 - 48 * worldScale + 1280 / 2, y0 - 48 * worldScale + 720 / 4 - offset * 48 * worldScale, 96 * worldScale, 96 * worldScale };

            if (heights[(y + yCamOf) * 200 + x + xCamOf][0] == 1) {
                rect1.y += wave_sin + wave_cos;
            }

            // Fixes the weird gaps between sqaures
            rect1.w *= 1.1;

            SDL_RenderCopy(renderer, texture, nullptr, &rect1);

        }
    }
}
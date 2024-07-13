#include "Perlin.h"

Perlin::Perlin(SDL_Renderer* renderer, int octaves, int grid_size, bool showColor, int seed, int size) {
    this->octaves = octaves;
    this->grid_size = grid_size;
    this->showColor = showColor;
    this->seed = seed;
    this->size = size;

    this->pixels = new Uint8[size * size * 4];

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, size, size);
}

float Perlin::interpolate(float a0, float a1, float w) {
    return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
}

vector2 Perlin::randomGradient(float ix, float iy) {
    // No precomputed gradients mean this works for any number of grid coordinates
    const unsigned w = 8 * sizeof(unsigned);
    const unsigned s = w / 2;
    unsigned a = ix, b = iy;
    a *= 3284157443 * seed;

    b ^= a << s | a >> w - s;
    b *= 1911520717 * seed;

    a ^= b << s | b >> w - s;
    a *= 2048419325 * seed;
    float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]

    // Create the vector from the angle
    vector2 v;
    v.x = sin(random);
    v.y = cos(random);

    return v;
}

float Perlin::dotGridGradient(int ix, int iy, float x, float y) {
    vector2 gradient = randomGradient(ix, iy);

    float dx = x - (float)ix;
    float dy = y - (float)iy;

    // Compute the dot-product
    return (dx * gradient.x + dy * gradient.y);
}

float Perlin::perlin(float x, float y) {
    int x0 = (int)x;
    int y0 = (int)y;
    int x1 = x0 + 1;
    int y1 = y0 + 1;

    float sx = x - (float)x0;
    float sy = y - (float)y0;

    float n0, n1, ix0, ix1, value;

    n0 = dotGridGradient(x0, y0, x, y);
    n1 = dotGridGradient(x1, y0, x, y);
    ix0 = interpolate(n0, n1, sx);

    n0 = dotGridGradient(x0, y1, x, y);
    n1 = dotGridGradient(x1, y1, x, y);
    ix1 = interpolate(n0, n1, sx);

    value = interpolate(ix0, ix1, sy);
    return value;
}

void Perlin::genPerlin() {

    for (int x = 0; x < size; x++)
    {
        for (int y = 0; y < size; y++)
        {
            int index = (y * size + x) * 4;


            float val = 0;

            float freq = 1;
            float amp = 1;

            for (int i = 0; i < octaves; i++)
            {
                val += perlin(x * freq / grid_size, y * freq / grid_size) * amp;

                freq *= 2;
                amp /= 2;

            }

            // Contrast
            val *= 1.2;

            // Clipping
            if (val > 1.0f)
                val = 1.0f;
            else if (val < -1.0f)
                val = -1.0f;

            // Convert 1 to -1 into 255 to 0
            int color = (int)(((val + 1.0f) * 0.5f) * 255);

            // Set pixel color
            pixels[index] = 255;
            pixels[index + 1] = color;
            pixels[index + 2] = color;
            pixels[index + 3] = color;
            heights[index / 4][0] = 6;
            heights[index / 4][1] = val;

            if (showColor) {
                if (val <= mountainsHeight && val > rockHeight) {
                    pixels[index + 1] = 220;
                    pixels[index + 2] = 220;
                    pixels[index + 3] = 220;
                    heights[index/4][0] = 6;
                    heights[index / 4][1] = val;
                }
                else if (val <= rockHeight && val > hillHeight) {
                    pixels[index + 1] = 50;
                    pixels[index + 2] = 50;
                    pixels[index + 3] = 50;
                    heights[index / 4][0] = 5;
                    heights[index / 4][1] = val;
                }
                else if (val <= hillHeight && val > grassHeight) {
                    pixels[index + 1] = 0;
                    pixels[index + 2] = 180;
                    pixels[index + 3] = 0;
                    heights[index / 4][0] = 4;
                    heights[index / 4][1] = val;
                }
                else if (val <= grassHeight && val > sandHeight) {
                    pixels[index + 1] = 0;
                    pixels[index + 2] = 200;
                    pixels[index + 3] = 0;
                    heights[index / 4][0] = 3;
                    heights[index / 4][1] = val;
                }
                else if (val <= sandHeight && val > waterHeight) {
                    pixels[index + 1] = 0;
                    pixels[index + 2] = 200;
                    pixels[index + 3] = 200;
                    heights[index / 4][0] = 2;
                    heights[index / 4][1] = val;
                }
                else if (val <= waterHeight) {
                    pixels[index + 1] = 200;
                    pixels[index + 2] = 0;
                    pixels[index + 3] = 0;
                    heights[index / 4][0] = 1;
                    heights[index / 4][1] = val;
                }
            }
        }
    }
}

void Perlin::UpdateTexture() {
    SDL_UpdateTexture(texture, nullptr, pixels, size * 4);
}

void Perlin::RenderTexture(SDL_Renderer* renderer) {
    SDL_Rect rect = { 0, 0, size, size};
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}
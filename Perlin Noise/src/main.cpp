#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <chrono>


#include "Perlin.h"
#include "Isometric.h"


int WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 720;


// Main code
int main(int, char**)
{
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // From 2.0.18: Enable native IME.
    #ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
    #endif

    // Create window with SDL_Renderer graphics context
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL2+SDL_Renderer example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, window_flags);
    if (window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return -1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_Log("Error creating SDL_Renderer!");
        return 0;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    bool show_demo_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    Perlin perlin(renderer, 12, 20, false, 1, 96);

    perlin.genPerlin();

    perlin.UpdateTexture();

    Isometric isometric(10, 1.0f, 0.0025, 40, renderer);

    // Main loop
    bool done = false;
    while (!done)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state

            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            ImGui::SliderInt("Grid size", &perlin.grid_size, 1, 400); 
            ImGui::SliderInt("Octaves", &perlin.octaves, 1, 12);
            ImGui::SliderInt("SEED", &perlin.seed, 1, 10000);
            ImGui::Checkbox("Colors", &perlin.showColor);
            ImGui::SameLine();
            if (ImGui::Button("Reload")) {
                perlin.genPerlin();
                perlin.UpdateTexture();
            }
            ImGui::SameLine();
            ImGui::VSliderFloat("##waterHeight", ImVec2(18, 160), &perlin.waterHeight, -1.0f, 1.0f, "");
            ImGui::SameLine();
            ImGui::VSliderFloat("##sandHeight", ImVec2(18, 160), &perlin.sandHeight, -1.0f, 1.0f, "");
            ImGui::SameLine();
            ImGui::VSliderFloat("##grassHeight", ImVec2(18, 160), &perlin.grassHeight, -1.0f, 1.0f, "");
            ImGui::SameLine();
            ImGui::VSliderFloat("##hillHeight", ImVec2(18, 160), &perlin.hillHeight, -1.0f, 1.0f, "");
            ImGui::SameLine();
            ImGui::VSliderFloat("##rockHeight", ImVec2(18, 160), &perlin.rockHeight, -1.0f, 1.0f, "");
            ImGui::SameLine();
            ImGui::VSliderFloat("##mountainsHeight", ImVec2(18, 160), &perlin.mountainsHeight, -1.0f, 1.0f, "");

            ImGui::SliderFloat("Scale", &isometric.worldScale, 0, 2);
            ImGui::SliderInt("Amp", &isometric.waveAmpl, 0, 40);
            ImGui::SliderFloat("Freq", &isometric.waveFreq, 0.0f, 0.5f);
            ImGui::SliderInt("Size", &isometric.worldSize, 0, 96);

            ImGui::SliderInt("X offset", &isometric.xCamOf, -100, 100);
            ImGui::SliderInt("Y offset", &isometric.yCamOf, -100, 100);


            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }
        // Rendering
        ImGui::Render();
        SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        SDL_RenderClear(renderer);

        perlin.RenderTexture(renderer);


        // Get current time in milliseconds
        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

        isometric.waves(renderer, perlin.heights);

        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);

        SDL_RenderPresent(renderer);
    }

    // Cleanup
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

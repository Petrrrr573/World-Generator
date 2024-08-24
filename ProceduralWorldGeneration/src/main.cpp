#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"
#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <chrono>

#include "Isometric.h"
#include "Perlin.h"


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

    ImVec4 bg_color = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);

    Perlin perlin(renderer, 12, 100, true, 1, 200);

    Isometric isometric(100, 0.25f, 0.0025, 1, renderer);
    perlin.genPerlin(isometric.Cubes, isometric.worldBlockOffset, isometric.xCamOf, isometric.yCamOf);
    perlin.UpdateTexture();

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

        ImGui::ShowDemoWindow();

        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Settings");

            {
                ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
                if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
                {
                    if (ImGui::BeginTabItem("Basic"))
                    {
                        ImGui::Dummy(ImVec2(0.0f, 10.0f));
                        ImGui::ColorEdit3("Background color", (float*)&bg_color);
                        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Perlin Noise"))
                    {
                        ImGui::Dummy(ImVec2(0.0f, 10.0f));
                        ImGui::SliderInt("Grid size", &perlin.grid_size, 1, 400);
                        ImGui::SliderInt("Octaves", &perlin.octaves, 1, 12);
                        ImGui::SliderInt("SEED", &perlin.seed, 1, 10000);
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Heights"))
                    {
                        ImGui::Dummy(ImVec2(0.0f, 10.0f));

                        ImGui::PushID("Heights");
                        {
                            ImGui::PushID("Water");
                            ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor(0 / 255.0f, 110 / 255.0f, 220 / 255.0f));
                            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor(0 / 255.0f, 90 / 255.0f, 200 / 255.0f));
                            ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor(0 / 255.0f, 128 / 255.0f, 235 / 255.0f));
                            ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor(0 / 255.0f, 148 / 255.0f, 255 / 255.0f));
                            ImGui::VSliderFloat("##v", ImVec2(18, 160), &perlin.waterHeight, -1.0f, 1.0f, "");
                            if (ImGui::IsItemActive() || ImGui::IsItemHovered())
                                ImGui::SetTooltip("%.3f", perlin.waterHeight);
                            ImGui::PopStyleColor(4);
                            ImGui::PopID();

                            ImGui::SameLine();

                            ImGui::PushID("Sand");
                            ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor(211 / 255.0f, 194 / 255.0f, 0 / 255.0f));
                            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor(191 / 255.0f, 174 / 255.0f, 0 / 255.0f));
                            ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor(130 / 255.0f, 117 / 255.0f, 0 / 255.0f));
                            ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor(150 / 255.0f, 137 / 255.0f, 0 / 255.0f));
                            ImGui::VSliderFloat("##v", ImVec2(18, 160), &perlin.sandHeight, -1.0f, 1.0f, "");
                            if (ImGui::IsItemActive() || ImGui::IsItemHovered())
                                ImGui::SetTooltip("%.3f", perlin.sandHeight);
                            ImGui::PopStyleColor(4);
                            ImGui::PopID();

                            ImGui::SameLine();

                            ImGui::PushID("Grass");
                            ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor(58 / 255.0f, 147 / 255.0f, 40 / 255.0f));
                            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor(38 / 255.0f, 127 / 255.0f, 20 / 255.0f));
                            ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor(26 / 255.0f, 86 / 255.0f, 20 / 255.0f));
                            ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor(46 / 255.0f, 106 / 255.0f, 40 / 255.0f));
                            ImGui::VSliderFloat("##v", ImVec2(18, 160), &perlin.grassHeight, -1.0f, 1.0f, "");
                            if (ImGui::IsItemActive() || ImGui::IsItemHovered())
                                ImGui::SetTooltip("%.3f", perlin.grassHeight);
                            ImGui::PopStyleColor(4);
                            ImGui::PopID();

                            ImGui::SameLine();

                            ImGui::PushID("Hill");
                            ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor(38 / 255.0f, 127 / 255.0f, 0 / 255.0f));
                            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor(18 / 255.0f, 107 / 255.0f, 0 / 255.0f));
                            ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor(6 / 255.0f, 66 / 255.0f, 0 / 255.0f));
                            ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor(26 / 255.0f, 86 / 255.0f, 0 / 255.0f));
                            ImGui::VSliderFloat("##v", ImVec2(18, 160), &perlin.hillHeight, -1.0f, 1.0f, "");
                            if (ImGui::IsItemActive() || ImGui::IsItemHovered())
                                ImGui::SetTooltip("%.3f", perlin.waterHeight);
                            ImGui::PopStyleColor(4);
                            ImGui::PopID();

                            ImGui::SameLine();

                            ImGui::PushID("Rock");
                            ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor(138 / 255.0f, 138 / 255.0f, 138 / 255.0f));
                            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor(118 / 255.0f, 118 / 255.0f, 118 / 255.0f));
                            ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor(98 / 255.0f, 98 / 255.0f, 98 / 255.0f));
                            ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor(118 / 255.0f, 118 / 255.0f, 118 / 255.0f));
                            ImGui::VSliderFloat("##v", ImVec2(18, 160), &perlin.rockHeight, -1.0f, 1.0f, "");
                            if (ImGui::IsItemActive() || ImGui::IsItemHovered())
                                ImGui::SetTooltip("%.3f", perlin.rockHeight);
                            ImGui::PopStyleColor(4);
                            ImGui::PopID();

                            ImGui::SameLine();

                            ImGui::PushID("Mountains");
                            ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor(220 / 255.0f, 220 / 255.0f, 220 / 255.0f));
                            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor(180 / 255.0f, 180 / 255.0f, 180 / 255.0f));
                            ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor(180 / 255.0f, 180 / 255.0f, 180 / 255.0f));
                            ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor(180 / 255.0f, 180 / 255.0f, 180 / 255.0f));
                            ImGui::VSliderFloat("##v", ImVec2(18, 160), &perlin.mountainsHeight, -1.0f, 1.0f, "");
                            if (ImGui::IsItemActive() || ImGui::IsItemHovered())
                                ImGui::SetTooltip("%.3f", perlin.mountainsHeight);
                            ImGui::PopStyleColor(4);
                            ImGui::PopID();
                        }
                        ImGui::PopID();
                        

                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Projection"))
                    {
                        ImGui::Dummy(ImVec2(0.0f, 10.0f));
                        ImGui::SliderFloat("Scale", &isometric.worldScale, 0, 2);
                        ImGui::SliderInt("Amp", &isometric.waveAmpl, 0, 40);
                        ImGui::SliderFloat("Freq", &isometric.waveFreq, 0.0f, 0.5f);
                        ImGui::SliderInt("Size", &isometric.worldSize, 0, 200 - std::max(isometric.xCamOf, isometric.yCamOf));
                        ImGui::SliderInt("Tree Chance", &perlin.treeChance, 0, 500);
                        if (ImGui::SliderFloat("Height", &isometric.worldHeight, 0, 200)) {
                            isometric.worldBlockOffset = 2 / isometric.worldHeight;
                        }

                        ImGui::SliderInt("X offset", &isometric.xCamOf, 0, 200 - isometric.worldSize);
                        ImGui::SliderInt("Y offset", &isometric.yCamOf, 0, 200 - isometric.worldSize);
                        ImGui::EndTabItem();
                    }
                    ImGui::EndTabBar();
                }
                ImGui::Separator();
            }
            ImGui::Dummy(ImVec2(0.0f, 20.0f));
            ImGui::Checkbox("Colors", &perlin.showColor);
            ImGui::SameLine();
            if (ImGui::Button("Reload")) {
                perlin.genPerlin(isometric.Cubes, isometric.worldBlockOffset, isometric.xCamOf, isometric.yCamOf);
                perlin.UpdateTexture();
            }
            ImGui::End();
        }
        // Rendering
        ImGui::Render();
        SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColor(renderer, (Uint8)(bg_color.x * 255), (Uint8)(bg_color.y * 255), (Uint8)(bg_color.z * 255), (Uint8)(bg_color.w * 255));
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

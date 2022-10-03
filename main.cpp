#include "lib/imgui/imgui.h"
#include "lib/imgui/imgui_impl_sdl.h"
#include "lib/imgui/imgui_impl_sdlrenderer.h"
#include "src/definitions.h"
#include "src/mmu.h"
#include "src/cpu.h"

#include <cstdio>
#include <SDL.h>

int main(int argc, char* argv[])
{
    PrintMessage(Info, "Initializing SDL");
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Failed to initialize: %s\n", SDL_GetError());
        return -1;
    }
    SDL_Window* window = SDL_CreateWindow("gbemu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GB_HEIGHT * 4, GB_WIDTH * 4, NULL);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_Log("Failed to create SDL renderer");
        return -1;
    }

    PrintMessage(Info, "Initializing ImGui context");
    // Initialize ImGui context
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);

    MMU* mmu = new MMU();
    mmu->load(argv[1]);
    CPU* cpu = new CPU(mmu);

    // Main event loop
    bool end = false;
    while (!end) {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                end = true;
        }
        cpu->cycle();
        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("CPU");
        ImGui::Text("AF %02X", cpu->AF.getRegister());
        ImGui::Text("BC %02X", cpu->BC.getRegister());
        ImGui::Text("DE %02X", cpu->DE.getRegister());
        ImGui::Text("HL %02X", cpu->HL.getRegister());
        ImGui::Text("SP %02X", cpu->sp);
        ImGui::Text("PC %02X", cpu->pc);
        ImGui::Text("Cycles %02X", cpu->cycles);
        ImGui::End();
        ImGui::Render();
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        SDL_SetRenderDrawColor(renderer, (uint8_t)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
    }
    PrintMessage(Info, "Closing application");
    // Clean and close
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
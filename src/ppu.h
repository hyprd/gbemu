#pragma once
#include <SDL.h>
#include "definitions.h"

class PPU {
	public:
		SDL_Window* window;
		SDL_Renderer* renderer;
		SDL_Texture* texture;

		PPU(const char* title);
		~PPU();
};


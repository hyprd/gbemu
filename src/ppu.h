#pragma once
#include <SDL.h>
#include "definitions.h"

class PPU {
	public:
		SDL_Window* window;
		SDL_Renderer* renderer;
		SDL_Texture* texture;

		uint16_t pixelbuffer[GB_HEIGHT * GB_WIDTH];
		uint16_t pixelbufferReady[GB_HEIGHT * GB_WIDTH];

		PPU(const char* title);
		~PPU();
		void setPixel(uint16_t x, uint16_t y, uint16_t colour);
		void render();
};

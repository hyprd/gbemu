#pragma once
#include <SDL.h>
#include "definitions.h"

class PPU {
	public:
		PPU(const char* title);
		~PPU();

		void render();
		void update(uint8_t memory[]);
	private:
		uint16_t LCDC = 0;
		uint16_t backgroundTable;
		uint16_t windowTable;
		uint16_t addressingMode;

		uint8_t paletteSprite0[4];
		uint8_t paletteSprite1[4];
		uint8_t paletteBackground[4];

		SDL_Window* window;
		SDL_Renderer* renderer;
		SDL_Texture* texture;

		uint16_t pixelbuffer[GB_HEIGHT * GB_WIDTH];
		uint16_t pixelbufferReady[GB_HEIGHT * GB_WIDTH];

		void setPixel(uint16_t x, uint16_t y, uint16_t colour);
		uint8_t getBit(uint8_t bit, uint16_t address);
		
};


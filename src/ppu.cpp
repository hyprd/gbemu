#include "ppu.h"

PPU::PPU(const char* title) {
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GB_HEIGHT * 4, GB_WIDTH * 4, NULL);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB4444, SDL_TEXTUREACCESS_STREAMING, GB_WIDTH, GB_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(renderer);
}

PPU::~PPU() {
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void PPU::setPixel(uint16_t x, uint16_t y, uint16_t colour) {
	int16_t pixel = y * GB_WIDTH + x;
	pixelbuffer[pixel] = colour;
}

void PPU::render() {
	SDL_UpdateTexture(texture, nullptr, pixelbufferReady, 2 * GB_WIDTH);
	SDL_RenderCopy(renderer, texture, nullptr, nullptr);
	SDL_RenderPresent(renderer);
}





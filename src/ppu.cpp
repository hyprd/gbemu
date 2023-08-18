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

uint8_t PPU::getBit(uint8_t bit, uint16_t address) {
	return (address >> bit) & 0x1;
}

void PPU::update(uint8_t memory[]) {
	backgroundTable = (getBit(memory[0xFF40], 3)) ? 0x9C00 : 0x9800;
	windowTable = (getBit(memory[0xFF40], 6)) ? 0x9C00 : 0x9800;
	addressingMode = getBit(memory[0xFF40], 4);

	if ((LCDC >> 0) & 0x1) {
		paletteBackground[0] = getBit(memory[0xFF47], 0) | getBit(memory[0xFF47], 1) << 1;
		paletteBackground[1] = getBit(memory[0xFF47], 2) | getBit(memory[0xFF47], 3) << 1;
		paletteBackground[2] = getBit(memory[0xFF47], 4) | getBit(memory[0xFF47], 5) << 1;
		paletteBackground[3] = getBit(memory[0xFF47], 6) | getBit(memory[0xFF47], 7) << 1;
	}

	if ((LCDC >> 1) & 0x1) {
		// Lower two bits are ignored because they are transparency values for sprites
		paletteSprite0[1] = getBit(memory[0xFF48], 2) | getBit(memory[0xFF48], 3) << 1;
		paletteSprite0[2] = getBit(memory[0xFF48], 4) | getBit(memory[0xFF48], 5) << 1;
		paletteSprite0[3] = getBit(memory[0xFF48], 6) | getBit(memory[0xFF48], 7) << 1;
		
		paletteSprite1[1] = getBit(memory[0xFF49], 2) | getBit(memory[0xFF49], 3) << 1;
		paletteSprite1[2] = getBit(memory[0xFF49], 4) | getBit(memory[0xFF49], 5) << 1;
		paletteSprite1[3] = getBit(memory[0xFF49], 6) | getBit(memory[0xFF49], 7) << 1;
	}
}







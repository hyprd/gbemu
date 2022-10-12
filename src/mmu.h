#pragma once
#include "definitions.h"

class MMU {
public:
	MMU();
	~MMU();
	uint8_t memory[GB_MEMORY];
	uint32_t romSize = 0;

	void load(std::string file);
	void set(uint16_t address, uint8_t value);
	uint8_t get(uint16_t);

	void setBit(uint8_t& byte, uint8_t bit);
	void clearBit(uint8_t & byte, uint8_t bit);
	void toggleBit(uint8_t & byte, uint8_t bit);
	uint8_t getBit(uint8_t byte, uint8_t bit);

	uint16_t formWord(uint8_t high, uint8_t low);
};

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
};

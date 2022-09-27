#pragma once
#include "definitions.h"

class MMU {
public:
	MMU();
	~MMU();
	uint8_t memory[GB_MEMORY];
	uint32_t romSize = 0;

	void load(std::string file);
};

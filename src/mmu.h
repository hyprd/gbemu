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

	void SetBit(uint8_t& byte, uint8_t bit);
	void ClearBit(uint8_t & byte, uint8_t bit);
	void ToggleBit(uint8_t & byte, uint8_t bit);
	uint8_t GetBit(uint8_t byte, uint8_t bit);
};

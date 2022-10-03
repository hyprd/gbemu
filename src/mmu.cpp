#include "mmu.h"

MMU::MMU() {
	PrintMessage(Info, "Instantiating memory array");
	memset(memory, 0, GB_MEMORY);
}

MMU::~MMU() {}

void MMU::load(std::string file) {
	PrintMessage(Info, "Loading cartridge"); 
    std::ifstream rom(file, std::ios::binary);
    rom.seekg(0, std::ios::end);
    auto size = rom.tellg();
    rom.seekg(std::ios::beg);
    rom.read((char *)memory, size);
}

void MMU::set(uint16_t address, uint8_t value) {
    memory[address] = value;
}

uint8_t MMU::get(uint16_t address) {
    return memory[address];
}

void MMU::SetBit(uint8_t& byte, uint8_t bit) {
    byte |= 1UL << bit;
}

void MMU::ClearBit(uint8_t& byte, uint8_t bit) {
    byte &= ~(1UL << bit);
}

void MMU::ToggleBit(uint8_t& byte, uint8_t bit) {
    byte ^= 1UL << bit;
}

uint8_t MMU::GetBit(uint8_t byte, uint8_t bit) {
    return !!(byte >> bit);
}
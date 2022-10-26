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

void MMU::setBit(uint8_t& byte, uint8_t bit) {
    std::bitset<8> b(byte);
    b.set(bit);
    byte = b.to_ulong();
}

void MMU::clearBit(uint8_t& byte, uint8_t bit) {
    std::bitset<8> b(byte);
    b.reset(bit);
    byte = b.to_ulong();
}

void MMU::toggleBit(uint8_t& byte, uint8_t bit) {
    byte ^= 1UL << bit;
}

uint8_t MMU::getBit(uint8_t byte, uint8_t bit) {
    return !!(byte >> bit);
}

uint16_t MMU::formWord(uint8_t high, uint8_t low) {
    return static_cast<uint16_t>(high << 8 | low & 0x00FF);
}
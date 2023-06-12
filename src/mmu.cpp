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
    rom.close();
    for (int i = 0x0134; i < 0x0143; i++) title.push_back(memory[i]);
}

void MMU::set(uint16_t address, uint8_t value) {
    if (address == 0xFF01) std::cout << value;
    memory[address] = value;
}

uint8_t MMU::get(uint16_t address) {
    return memory[address];
}

void MMU::setBit(uint8_t& byte, uint8_t bit) {
    std::bitset<8> b(byte);
    b.set(bit);
    byte = static_cast<uint8_t>(b.to_ulong());
}

void MMU::clearBit(uint8_t& byte, uint8_t bit) {
    std::bitset<8> b(byte);
    b.reset(bit);
    byte = static_cast<uint8_t>(b.to_ulong());
}

void MMU::toggleBit(uint8_t& byte, uint8_t bit) {
    std::bitset<8> b(byte);
    b.flip(bit);
    byte = static_cast<uint8_t>(b.to_ulong());
}

uint8_t MMU::getBit(uint8_t byte, uint8_t bit) {
    return (byte & (1 << +bit)) >> +bit;
}

uint16_t MMU::formWord(uint8_t high, uint8_t low) {
    return static_cast<uint16_t>(high << 8 | low & 0x00FF);
}
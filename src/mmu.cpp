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
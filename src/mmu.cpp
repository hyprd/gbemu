#include "mmu.h"

MMU::MMU() {
	PrintMessage(Info, "Instantiating memory array");
	memset(memory, 0, GB_MEMORY);
}

MMU::~MMU() {}

void MMU::load(std::string file) {
	PrintMessage(Info, "Loading cartridge"); 
    std::fstream filestream;
    filestream.open(file, std::ios::in);
    uint8_t ch;
    for (int i = 0; i < sizeof(this->memory) / sizeof(this->memory[0]); i++) {
        filestream >> ch;
        if (filestream.eof()) { 
        break; 
        }
        memory[i] = ch;
    }
    filestream.close();
}
#include "mmu.h"

MMU::MMU() {
	memset(memory, 0, GB_MEMORY);
}

MMU::~MMU() {}

void MMU::load(std::string file) {
	std::cout << file << std::endl;
}
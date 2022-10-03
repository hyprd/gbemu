#include "cpu.h"

Register::Register() {
	this->low = NULL;
	this->high = NULL;
}

Register::~Register() {}

Register::Register(uint8_t* l, uint8_t* h) {
	low = l;
	high = h;
}

uint16_t Register::getRegister() {
	return *high << 8 | (*low & 0x00FF);
}

void Register::setRegister(uint16_t value) {
	*low = value & 0x00FF;
	*high = value >> 8;
}

CPU::CPU(MMU * mmu) {
	this->mmu = mmu;
	this->initialize();
}

CPU::~CPU() {}

void CPU::initialize() {
	A = B = C = D = E = F = H = L = 0x0000;
	// Assign 8-bit registers to 16-bit counterparts
	AF = Register(&A, &F);
	BC = Register(&B, &C);
	DE = Register(&D, &E);
	HL = Register(&H, &L);
	// Set 16-bit registers to their default values
	AF.setRegister(0x01B0);
	BC.setRegister(0x0013);
	DE.setRegister(0x00D8);
	HL.setRegister(0x014D);
	// Set stack pointer, program counter and cycles members to 
	// default values
	sp = pc = cycles = 0x0000;
}

void CPU::cycle() {
	execute(mmu->memory[pc]);
}

void CPU::execute(uint8_t inst) {
	if (!extended) {
		//(this->*opcodes[inst])();
		// add cycles
	}
	else {
		//(this->*extendedOpcodes[inst])();
		// add cycles
		extended = false;
	}
	if (!halted) pc++;
	// handle interrupt function
}
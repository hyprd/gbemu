#pragma once
#include "definitions.h"

class Register {
public:
	Register();
	~Register();
	Register(uint8_t* l, uint8_t* h);
	uint8_t* low;
	uint8_t* high;
	uint16_t getRegister();
	void setRegister(uint16_t value);
};

class CPU {
public: 
	CPU();
	~CPU();

	uint8_t A, B, C, D, E, F, H, L;
	uint8_t FLAG_Z = 7;
	uint8_t FLAG_N = 6;
	uint8_t FLAG_H = 5;
	uint8_t FLAG_C = 4;

	Register AF, BC, DE, HL;
	
	unsigned long int sp;
	unsigned long int pc;
	unsigned long int cycles;

	void initialize();
	void cycle();
	void execute(uint8_t inst);

	typedef void (CPU::* Opcode)(void);
	Opcode opcodes[0x100];
	Opcode extendedOpcodes[0x100];
};

#pragma once
#include "definitions.h"
#include "mmu.h"


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
	CPU(MMU * mmu);
	~CPU();

	MMU* mmu;

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
	void bindOpcodes();
	
	bool didCarry(uint8_t reg);
	bool didHalfCarry(uint8_t reg);
	bool didBorrow(uint8_t reg);
	bool didHalfBorrow(uint8_t reg);
	bool didCarry16(uint16_t reg, uint16_t reg2);
	bool didHalfCarry16(uint16_t reg, uint16_t reg2);

	uint8_t getFlag(uint8_t flag);
	void setFlag(uint8_t flag);
	void clearFlag(uint8_t flag);

	bool extended = false;
	bool halted = false;

	typedef void (CPU::*Opcode)(void);
	Opcode opcodes[0x100];
	Opcode extendedOpcodes[0x100];

	void LD(uint8_t& reg1, uint8_t reg2); // LD X,Y
	void LD(uint16_t address, uint8_t reg); // LD (YZ),X, LD d16,X
	void LD(uint8_t& reg, uint16_t address); // LD X,(YZ), LD X,d16

	void ADD(uint8_t reg);
	void ADD_HL(Register reg);
	void ADD_SP();
	void ADC(uint8_t reg);

	void SUB(uint8_t reg);
	void SBC(uint8_t reg);

	void AND(uint8_t reg);
	void OR(uint8_t reg);
	void XOR(uint8_t reg);
	void CP(uint8_t reg);

	void INC(uint8_t* reg);
	void INC(Register reg);
	void DEC(uint8_t * reg);
	void DEC(Register reg);
	
	void Opcode0x00();
	void Opcode0x01();
	void Opcode0x02();
	void Opcode0x03();
	void Opcode0x04();
	void Opcode0x05();
	void Opcode0x06();
	void Opcode0x07();
	void Opcode0x08();
	void Opcode0x09();
	void Opcode0x0A();
	void Opcode0x0B();
	void Opcode0x0C();
	void Opcode0x0D();
	void Opcode0x0E();
	void Opcode0x0F();
	void Opcode0x10();
	void Opcode0x11();
	void Opcode0x12();
	void Opcode0x13();
	void Opcode0x14();
	void Opcode0x15();
	void Opcode0x16();
	void Opcode0x17();
	void Opcode0x18();
	void Opcode0x19();
	void Opcode0x1A();
	void Opcode0x1B();
	void Opcode0x1C();
	void Opcode0x1D();
	void Opcode0x1E();
	void Opcode0x1F();
	void Opcode0x20();
	void Opcode0x21();
	void Opcode0x22();
	void Opcode0x23();
	void Opcode0x24();
	void Opcode0x25();
	void Opcode0x26();
	void Opcode0x27();
	void Opcode0x28();
	void Opcode0x29();
	void Opcode0x2A();
	void Opcode0x2B();
	void Opcode0x2C();
	void Opcode0x2D();
	void Opcode0x2E();
	void Opcode0x2F();
	void Opcode0x30();
	void Opcode0x31();
	void Opcode0x32();
	void Opcode0x33();
	void Opcode0x34();
	void Opcode0x35();
	void Opcode0x36();
	void Opcode0x37();
	void Opcode0x38();
	void Opcode0x39();
	void Opcode0x3A();
	void Opcode0x3B();
	void Opcode0x3C();
	void Opcode0x3D();
	void Opcode0x3E();
	void Opcode0x3F();
	void Opcode0x40();
	void Opcode0x41();
	void Opcode0x42();
	void Opcode0x43();
	void Opcode0x44();
	void Opcode0x45();
	void Opcode0x46();
	void Opcode0x47();
	void Opcode0x48();
	void Opcode0x49();
	void Opcode0x4A();
	void Opcode0x4B();
	void Opcode0x4C();
	void Opcode0x4D();
	void Opcode0x4E();
	void Opcode0x4F();
	void Opcode0x50();
	void Opcode0x51();
	void Opcode0x52();
	void Opcode0x53();
	void Opcode0x54();
	void Opcode0x55();
	void Opcode0x56();
	void Opcode0x57();
	void Opcode0x58();
	void Opcode0x59();
	void Opcode0x5A();
	void Opcode0x5B();
	void Opcode0x5C();
	void Opcode0x5D();
	void Opcode0x5E();
	void Opcode0x5F();
	void Opcode0x60();
	void Opcode0x61();
	void Opcode0x62();
	void Opcode0x63();
	void Opcode0x64();
	void Opcode0x65();
	void Opcode0x66();
	void Opcode0x67();
	void Opcode0x68();
	void Opcode0x69();
	void Opcode0x6A();
	void Opcode0x6B();
	void Opcode0x6C();
	void Opcode0x6D();
	void Opcode0x6E();
	void Opcode0x6F();
	void Opcode0x70();
	void Opcode0x71();
	void Opcode0x72();
	void Opcode0x73();
	void Opcode0x74();
	void Opcode0x75();
	void Opcode0x76();
	void Opcode0x77();
	void Opcode0x78();
	void Opcode0x79();
	void Opcode0x7A();
	void Opcode0x7B();
	void Opcode0x7C();
	void Opcode0x7D();
	void Opcode0x7E();
	void Opcode0x7F();
	void Opcode0x80();
	void Opcode0x81();
	void Opcode0x82();
	void Opcode0x83();
	void Opcode0x84();
	void Opcode0x85();
	void Opcode0x86();
	void Opcode0x87();
	void Opcode0x88();
	void Opcode0x89();
	void Opcode0x8A();
	void Opcode0x8B();
	void Opcode0x8C();
	void Opcode0x8D();
	void Opcode0x8E();
	void Opcode0x8F();
	void Opcode0x90();
	void Opcode0x91();
	void Opcode0x92();
	void Opcode0x93();
	void Opcode0x94();
	void Opcode0x95();
	void Opcode0x96();
	void Opcode0x97();
	void Opcode0x98();
	void Opcode0x99();
	void Opcode0x9A();
	void Opcode0x9B();
	void Opcode0x9C();
	void Opcode0x9D();
	void Opcode0x9E();
	void Opcode0x9F();
	void Opcode0xA0();
	void Opcode0xA1();
	void Opcode0xA2();
	void Opcode0xA3();
	void Opcode0xA4();
	void Opcode0xA5();
	void Opcode0xA6();
	void Opcode0xA7();
	void Opcode0xA8();
	void Opcode0xA9();
	void Opcode0xAA();
	void Opcode0xAB();
	void Opcode0xAC();
	void Opcode0xAD();
	void Opcode0xAE();
	void Opcode0xAF();
	void Opcode0xB0();
	void Opcode0xB1();
	void Opcode0xB2();
	void Opcode0xB3();
	void Opcode0xB4();
	void Opcode0xB5();
	void Opcode0xB6();
	void Opcode0xB7();
	void Opcode0xB8();
	void Opcode0xB9();
	void Opcode0xBA();
	void Opcode0xBB();
	void Opcode0xBC();
	void Opcode0xBD();
	void Opcode0xBE();
	void Opcode0xBF();
	void Opcode0xC0();
	void Opcode0xC1();
	void Opcode0xC2();
	void Opcode0xC3();
	void Opcode0xC4();
	void Opcode0xC5();
	void Opcode0xC6();
	void Opcode0xC7();
	void Opcode0xC8();
	void Opcode0xC9();
	void Opcode0xCA();
	void Opcode0xCB();
	void Opcode0xCC();
	void Opcode0xCD();
	void Opcode0xCE();
	void Opcode0xCF();
	void Opcode0xD0();
	void Opcode0xD1();
	void Opcode0xD2();
	void Opcode0xD3();
	void Opcode0xD4();
	void Opcode0xD5();
	void Opcode0xD6();
	void Opcode0xD7();
	void Opcode0xD8();
	void Opcode0xD9();
	void Opcode0xDA();
	void Opcode0xDB();
	void Opcode0xDC();
	void Opcode0xDD();
	void Opcode0xDE();
	void Opcode0xDF();
	void Opcode0xE0();
	void Opcode0xE1();
	void Opcode0xE2();
	void Opcode0xE3();
	void Opcode0xE4();
	void Opcode0xE5();
	void Opcode0xE6();
	void Opcode0xE7();
	void Opcode0xE8();
	void Opcode0xE9();
	void Opcode0xEA();
	void Opcode0xEB();
	void Opcode0xEC();
	void Opcode0xED();
	void Opcode0xEE();
	void Opcode0xEF();
	void Opcode0xF0();
	void Opcode0xF1();
	void Opcode0xF2();
	void Opcode0xF3();
	void Opcode0xF4();
	void Opcode0xF5();
	void Opcode0xF6();
	void Opcode0xF7();
	void Opcode0xF8();
	void Opcode0xF9();
	void Opcode0xFA();
	void Opcode0xFB();
	void Opcode0xFC();
	void Opcode0xFD();
	void Opcode0xFE();
	void Opcode0xFF();
	
	void extendedOpcode0x00();
	void extendedOpcode0x01();
	void extendedOpcode0x02();
	void extendedOpcode0x03();
	void extendedOpcode0x04();
	void extendedOpcode0x05();
	void extendedOpcode0x06();
	void extendedOpcode0x07();
	void extendedOpcode0x08();
	void extendedOpcode0x09();
	void extendedOpcode0x0A();
	void extendedOpcode0x0B();
	void extendedOpcode0x0C();
	void extendedOpcode0x0D();
	void extendedOpcode0x0E();
	void extendedOpcode0x0F();
	void extendedOpcode0x10();
	void extendedOpcode0x11();
	void extendedOpcode0x12();
	void extendedOpcode0x13();
	void extendedOpcode0x14();
	void extendedOpcode0x15();
	void extendedOpcode0x16();
	void extendedOpcode0x17();
	void extendedOpcode0x18();
	void extendedOpcode0x19();
	void extendedOpcode0x1A();
	void extendedOpcode0x1B();
	void extendedOpcode0x1C();
	void extendedOpcode0x1D();
	void extendedOpcode0x1E();
	void extendedOpcode0x1F();
	void extendedOpcode0x20();
	void extendedOpcode0x21();
	void extendedOpcode0x22();
	void extendedOpcode0x23();
	void extendedOpcode0x24();
	void extendedOpcode0x25();
	void extendedOpcode0x26();
	void extendedOpcode0x27();
	void extendedOpcode0x28();
	void extendedOpcode0x29();
	void extendedOpcode0x2A();
	void extendedOpcode0x2B();
	void extendedOpcode0x2C();
	void extendedOpcode0x2D();
	void extendedOpcode0x2E();
	void extendedOpcode0x2F();
	void extendedOpcode0x30();
	void extendedOpcode0x31();
	void extendedOpcode0x32();
	void extendedOpcode0x33();
	void extendedOpcode0x34();
	void extendedOpcode0x35();
	void extendedOpcode0x36();
	void extendedOpcode0x37();
	void extendedOpcode0x38();
	void extendedOpcode0x39();
	void extendedOpcode0x3A();
	void extendedOpcode0x3B();
	void extendedOpcode0x3C();
	void extendedOpcode0x3D();
	void extendedOpcode0x3E();
	void extendedOpcode0x3F();
	void extendedOpcode0x40();
	void extendedOpcode0x41();
	void extendedOpcode0x42();
	void extendedOpcode0x43();
	void extendedOpcode0x44();
	void extendedOpcode0x45();
	void extendedOpcode0x46();
	void extendedOpcode0x47();
	void extendedOpcode0x48();
	void extendedOpcode0x49();
	void extendedOpcode0x4A();
	void extendedOpcode0x4B();
	void extendedOpcode0x4C();
	void extendedOpcode0x4D();
	void extendedOpcode0x4E();
	void extendedOpcode0x4F();
	void extendedOpcode0x50();
	void extendedOpcode0x51();
	void extendedOpcode0x52();
	void extendedOpcode0x53();
	void extendedOpcode0x54();
	void extendedOpcode0x55();
	void extendedOpcode0x56();
	void extendedOpcode0x57();
	void extendedOpcode0x58();
	void extendedOpcode0x59();
	void extendedOpcode0x5A();
	void extendedOpcode0x5B();
	void extendedOpcode0x5C();
	void extendedOpcode0x5D();
	void extendedOpcode0x5E();
	void extendedOpcode0x5F();
	void extendedOpcode0x60();
	void extendedOpcode0x61();
	void extendedOpcode0x62();
	void extendedOpcode0x63();
	void extendedOpcode0x64();
	void extendedOpcode0x65();
	void extendedOpcode0x66();
	void extendedOpcode0x67();
	void extendedOpcode0x68();
	void extendedOpcode0x69();
	void extendedOpcode0x6A();
	void extendedOpcode0x6B();
	void extendedOpcode0x6C();
	void extendedOpcode0x6D();
	void extendedOpcode0x6E();
	void extendedOpcode0x6F();
	void extendedOpcode0x70();
	void extendedOpcode0x71();
	void extendedOpcode0x72();
	void extendedOpcode0x73();
	void extendedOpcode0x74();
	void extendedOpcode0x75();
	void extendedOpcode0x76();
	void extendedOpcode0x77();
	void extendedOpcode0x78();
	void extendedOpcode0x79();
	void extendedOpcode0x7A();
	void extendedOpcode0x7B();
	void extendedOpcode0x7C();
	void extendedOpcode0x7D();
	void extendedOpcode0x7E();
	void extendedOpcode0x7F();
	void extendedOpcode0x80();
	void extendedOpcode0x81();
	void extendedOpcode0x82();
	void extendedOpcode0x83();
	void extendedOpcode0x84();
	void extendedOpcode0x85();
	void extendedOpcode0x86();
	void extendedOpcode0x87();
	void extendedOpcode0x88();
	void extendedOpcode0x89();
	void extendedOpcode0x8A();
	void extendedOpcode0x8B();
	void extendedOpcode0x8C();
	void extendedOpcode0x8D();
	void extendedOpcode0x8E();
	void extendedOpcode0x8F();
	void extendedOpcode0x90();
	void extendedOpcode0x91();
	void extendedOpcode0x92();
	void extendedOpcode0x93();
	void extendedOpcode0x94();
	void extendedOpcode0x95();
	void extendedOpcode0x96();
	void extendedOpcode0x97();
	void extendedOpcode0x98();
	void extendedOpcode0x99();
	void extendedOpcode0x9A();
	void extendedOpcode0x9B();
	void extendedOpcode0x9C();
	void extendedOpcode0x9D();
	void extendedOpcode0x9E();
	void extendedOpcode0x9F();
	void extendedOpcode0xA0();
	void extendedOpcode0xA1();
	void extendedOpcode0xA2();
	void extendedOpcode0xA3();
	void extendedOpcode0xA4();
	void extendedOpcode0xA5();
	void extendedOpcode0xA6();
	void extendedOpcode0xA7();
	void extendedOpcode0xA8();
	void extendedOpcode0xA9();
	void extendedOpcode0xAA();
	void extendedOpcode0xAB();
	void extendedOpcode0xAC();
	void extendedOpcode0xAD();
	void extendedOpcode0xAE();
	void extendedOpcode0xAF();
	void extendedOpcode0xB0();
	void extendedOpcode0xB1();
	void extendedOpcode0xB2();
	void extendedOpcode0xB3();
	void extendedOpcode0xB4();
	void extendedOpcode0xB5();
	void extendedOpcode0xB6();
	void extendedOpcode0xB7();
	void extendedOpcode0xB8();
	void extendedOpcode0xB9();
	void extendedOpcode0xBA();
	void extendedOpcode0xBB();
	void extendedOpcode0xBC();
	void extendedOpcode0xBD();
	void extendedOpcode0xBE();
	void extendedOpcode0xBF();
	void extendedOpcode0xC0();
	void extendedOpcode0xC1();
	void extendedOpcode0xC2();
	void extendedOpcode0xC3();
	void extendedOpcode0xC4();
	void extendedOpcode0xC5();
	void extendedOpcode0xC6();
	void extendedOpcode0xC7();
	void extendedOpcode0xC8();
	void extendedOpcode0xC9();
	void extendedOpcode0xCA();
	void extendedOpcode0xCB();
	void extendedOpcode0xCC();
	void extendedOpcode0xCD();
	void extendedOpcode0xCE();
	void extendedOpcode0xCF();
	void extendedOpcode0xD0();
	void extendedOpcode0xD1();
	void extendedOpcode0xD2();
	void extendedOpcode0xD3();
	void extendedOpcode0xD4();
	void extendedOpcode0xD5();
	void extendedOpcode0xD6();
	void extendedOpcode0xD7();
	void extendedOpcode0xD8();
	void extendedOpcode0xD9();
	void extendedOpcode0xDA();
	void extendedOpcode0xDB();
	void extendedOpcode0xDC();
	void extendedOpcode0xDD();
	void extendedOpcode0xDE();
	void extendedOpcode0xDF();
	void extendedOpcode0xE0();
	void extendedOpcode0xE1();
	void extendedOpcode0xE2();
	void extendedOpcode0xE3();
	void extendedOpcode0xE4();
	void extendedOpcode0xE5();
	void extendedOpcode0xE6();
	void extendedOpcode0xE7();
	void extendedOpcode0xE8();
	void extendedOpcode0xE9();
	void extendedOpcode0xEA();
	void extendedOpcode0xEB();
	void extendedOpcode0xEC();
	void extendedOpcode0xED();
	void extendedOpcode0xEE();
	void extendedOpcode0xEF();
	void extendedOpcode0xF0();
	void extendedOpcode0xF1();
	void extendedOpcode0xF2();
	void extendedOpcode0xF3();
	void extendedOpcode0xF4();
	void extendedOpcode0xF5();
	void extendedOpcode0xF6();
	void extendedOpcode0xF7();
	void extendedOpcode0xF8();
	void extendedOpcode0xF9();
	void extendedOpcode0xFA();
	void extendedOpcode0xFB();
	void extendedOpcode0xFC();
	void extendedOpcode0xFD();
	void extendedOpcode0xFE();
	void extendedOpcode0xFF();
};

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

void CPU::bindOpcodes() {
	this->opcodes[0x00] = &CPU::Opcode0x00;
	this->opcodes[0x01] = &CPU::Opcode0x01;
	this->opcodes[0x02] = &CPU::Opcode0x02;
	this->opcodes[0x03] = &CPU::Opcode0x03;
	this->opcodes[0x04] = &CPU::Opcode0x04;
	this->opcodes[0x05] = &CPU::Opcode0x05;
	this->opcodes[0x06] = &CPU::Opcode0x06;
	this->opcodes[0x07] = &CPU::Opcode0x07;
	this->opcodes[0x08] = &CPU::Opcode0x08;
	this->opcodes[0x09] = &CPU::Opcode0x09;
	this->opcodes[0x0A] = &CPU::Opcode0x0A;
	this->opcodes[0x0B] = &CPU::Opcode0x0B;
	this->opcodes[0x0C] = &CPU::Opcode0x0C;
	this->opcodes[0x0D] = &CPU::Opcode0x0D;
	this->opcodes[0x0E] = &CPU::Opcode0x0E;
	this->opcodes[0x0F] = &CPU::Opcode0x0F;
	this->opcodes[0x10] = &CPU::Opcode0x10;
	this->opcodes[0x11] = &CPU::Opcode0x11;
	this->opcodes[0x12] = &CPU::Opcode0x12;
	this->opcodes[0x13] = &CPU::Opcode0x13;
	this->opcodes[0x14] = &CPU::Opcode0x14;
	this->opcodes[0x15] = &CPU::Opcode0x15;
	this->opcodes[0x16] = &CPU::Opcode0x16;
	this->opcodes[0x17] = &CPU::Opcode0x17;
	this->opcodes[0x18] = &CPU::Opcode0x18;
	this->opcodes[0x19] = &CPU::Opcode0x19;
	this->opcodes[0x1A] = &CPU::Opcode0x1A;
	this->opcodes[0x1B] = &CPU::Opcode0x1B;
	this->opcodes[0x1C] = &CPU::Opcode0x1C;
	this->opcodes[0x1D] = &CPU::Opcode0x1D;
	this->opcodes[0x1E] = &CPU::Opcode0x1E;
	this->opcodes[0x1F] = &CPU::Opcode0x1F;
	this->opcodes[0x20] = &CPU::Opcode0x20;
	this->opcodes[0x21] = &CPU::Opcode0x21;
	this->opcodes[0x22] = &CPU::Opcode0x22;
	this->opcodes[0x23] = &CPU::Opcode0x23;
	this->opcodes[0x24] = &CPU::Opcode0x24;
	this->opcodes[0x25] = &CPU::Opcode0x25;
	this->opcodes[0x26] = &CPU::Opcode0x26;
	this->opcodes[0x27] = &CPU::Opcode0x27;
	this->opcodes[0x28] = &CPU::Opcode0x28;
	this->opcodes[0x29] = &CPU::Opcode0x29;
	this->opcodes[0x2A] = &CPU::Opcode0x2A;
	this->opcodes[0x2B] = &CPU::Opcode0x2B;
	this->opcodes[0x2C] = &CPU::Opcode0x2C;
	this->opcodes[0x2D] = &CPU::Opcode0x2D;
	this->opcodes[0x2E] = &CPU::Opcode0x2E;
	this->opcodes[0x2F] = &CPU::Opcode0x2F;
	this->opcodes[0x30] = &CPU::Opcode0x30;
	this->opcodes[0x31] = &CPU::Opcode0x31;
	this->opcodes[0x32] = &CPU::Opcode0x32;
	this->opcodes[0x33] = &CPU::Opcode0x33;
	this->opcodes[0x34] = &CPU::Opcode0x34;
	this->opcodes[0x35] = &CPU::Opcode0x35;
	this->opcodes[0x36] = &CPU::Opcode0x36;
	this->opcodes[0x37] = &CPU::Opcode0x37;
	this->opcodes[0x38] = &CPU::Opcode0x38;
	this->opcodes[0x39] = &CPU::Opcode0x39;
	this->opcodes[0x3A] = &CPU::Opcode0x3A;
	this->opcodes[0x3B] = &CPU::Opcode0x3B;
	this->opcodes[0x3C] = &CPU::Opcode0x3C;
	this->opcodes[0x3D] = &CPU::Opcode0x3D;
	this->opcodes[0x3E] = &CPU::Opcode0x3E;
	this->opcodes[0x3F] = &CPU::Opcode0x3F;
	this->opcodes[0x40] = &CPU::Opcode0x40;
	this->opcodes[0x41] = &CPU::Opcode0x41;
	this->opcodes[0x42] = &CPU::Opcode0x42;
	this->opcodes[0x43] = &CPU::Opcode0x43;
	this->opcodes[0x44] = &CPU::Opcode0x44;
	this->opcodes[0x45] = &CPU::Opcode0x45;
	this->opcodes[0x46] = &CPU::Opcode0x46;
	this->opcodes[0x47] = &CPU::Opcode0x47;
	this->opcodes[0x48] = &CPU::Opcode0x48;
	this->opcodes[0x49] = &CPU::Opcode0x49;
	this->opcodes[0x4A] = &CPU::Opcode0x4A;
	this->opcodes[0x4B] = &CPU::Opcode0x4B;
	this->opcodes[0x4C] = &CPU::Opcode0x4C;
	this->opcodes[0x4D] = &CPU::Opcode0x4D;
	this->opcodes[0x4E] = &CPU::Opcode0x4E;
	this->opcodes[0x4F] = &CPU::Opcode0x4F;
	this->opcodes[0x50] = &CPU::Opcode0x50;
	this->opcodes[0x51] = &CPU::Opcode0x51;
	this->opcodes[0x52] = &CPU::Opcode0x52;
	this->opcodes[0x53] = &CPU::Opcode0x53;
	this->opcodes[0x54] = &CPU::Opcode0x54;
	this->opcodes[0x55] = &CPU::Opcode0x55;
	this->opcodes[0x56] = &CPU::Opcode0x56;
	this->opcodes[0x57] = &CPU::Opcode0x57;
	this->opcodes[0x58] = &CPU::Opcode0x58;
	this->opcodes[0x59] = &CPU::Opcode0x59;
	this->opcodes[0x5A] = &CPU::Opcode0x5A;
	this->opcodes[0x5B] = &CPU::Opcode0x5B;
	this->opcodes[0x5C] = &CPU::Opcode0x5C;
	this->opcodes[0x5D] = &CPU::Opcode0x5D;
	this->opcodes[0x5E] = &CPU::Opcode0x5E;
	this->opcodes[0x5F] = &CPU::Opcode0x5F;
	this->opcodes[0x60] = &CPU::Opcode0x60;
	this->opcodes[0x61] = &CPU::Opcode0x61;
	this->opcodes[0x62] = &CPU::Opcode0x62;
	this->opcodes[0x63] = &CPU::Opcode0x63;
	this->opcodes[0x64] = &CPU::Opcode0x64;
	this->opcodes[0x65] = &CPU::Opcode0x65;
	this->opcodes[0x66] = &CPU::Opcode0x66;
	this->opcodes[0x67] = &CPU::Opcode0x67;
	this->opcodes[0x68] = &CPU::Opcode0x68;
	this->opcodes[0x69] = &CPU::Opcode0x69;
	this->opcodes[0x6A] = &CPU::Opcode0x6A;
	this->opcodes[0x6B] = &CPU::Opcode0x6B;
	this->opcodes[0x6C] = &CPU::Opcode0x6C;
	this->opcodes[0x6D] = &CPU::Opcode0x6D;
	this->opcodes[0x6E] = &CPU::Opcode0x6E;
	this->opcodes[0x6F] = &CPU::Opcode0x6F;
	this->opcodes[0x70] = &CPU::Opcode0x70;
	this->opcodes[0x71] = &CPU::Opcode0x71;
	this->opcodes[0x72] = &CPU::Opcode0x72;
	this->opcodes[0x73] = &CPU::Opcode0x73;
	this->opcodes[0x74] = &CPU::Opcode0x74;
	this->opcodes[0x75] = &CPU::Opcode0x75;
	this->opcodes[0x76] = &CPU::Opcode0x76;
	this->opcodes[0x77] = &CPU::Opcode0x77;
	this->opcodes[0x78] = &CPU::Opcode0x78;
	this->opcodes[0x79] = &CPU::Opcode0x79;
	this->opcodes[0x7A] = &CPU::Opcode0x7A;
	this->opcodes[0x7B] = &CPU::Opcode0x7B;
	this->opcodes[0x7C] = &CPU::Opcode0x7C;
	this->opcodes[0x7D] = &CPU::Opcode0x7D;
	this->opcodes[0x7E] = &CPU::Opcode0x7E;
	this->opcodes[0x7F] = &CPU::Opcode0x7F;
	this->opcodes[0x80] = &CPU::Opcode0x80;
	this->opcodes[0x81] = &CPU::Opcode0x81;
	this->opcodes[0x82] = &CPU::Opcode0x82;
	this->opcodes[0x83] = &CPU::Opcode0x83;
	this->opcodes[0x84] = &CPU::Opcode0x84;
	this->opcodes[0x85] = &CPU::Opcode0x85;
	this->opcodes[0x86] = &CPU::Opcode0x86;
	this->opcodes[0x87] = &CPU::Opcode0x87;
	this->opcodes[0x88] = &CPU::Opcode0x88;
	this->opcodes[0x89] = &CPU::Opcode0x89;
	this->opcodes[0x8A] = &CPU::Opcode0x8A;
	this->opcodes[0x8B] = &CPU::Opcode0x8B;
	this->opcodes[0x8C] = &CPU::Opcode0x8C;
	this->opcodes[0x8D] = &CPU::Opcode0x8D;
	this->opcodes[0x8E] = &CPU::Opcode0x8E;
	this->opcodes[0x8F] = &CPU::Opcode0x8F;
	this->opcodes[0x90] = &CPU::Opcode0x90;
	this->opcodes[0x91] = &CPU::Opcode0x91;
	this->opcodes[0x92] = &CPU::Opcode0x92;
	this->opcodes[0x93] = &CPU::Opcode0x93;
	this->opcodes[0x94] = &CPU::Opcode0x94;
	this->opcodes[0x95] = &CPU::Opcode0x95;
	this->opcodes[0x96] = &CPU::Opcode0x96;
	this->opcodes[0x97] = &CPU::Opcode0x97;
	this->opcodes[0x98] = &CPU::Opcode0x98;
	this->opcodes[0x99] = &CPU::Opcode0x99;
	this->opcodes[0x9A] = &CPU::Opcode0x9A;
	this->opcodes[0x9B] = &CPU::Opcode0x9B;
	this->opcodes[0x9C] = &CPU::Opcode0x9C;
	this->opcodes[0x9D] = &CPU::Opcode0x9D;
	this->opcodes[0x9E] = &CPU::Opcode0x9E;
	this->opcodes[0x9F] = &CPU::Opcode0x9F;
	this->opcodes[0xA0] = &CPU::Opcode0xA0;
	this->opcodes[0xA1] = &CPU::Opcode0xA1;
	this->opcodes[0xA2] = &CPU::Opcode0xA2;
	this->opcodes[0xA3] = &CPU::Opcode0xA3;
	this->opcodes[0xA4] = &CPU::Opcode0xA4;
	this->opcodes[0xA5] = &CPU::Opcode0xA5;
	this->opcodes[0xA6] = &CPU::Opcode0xA6;
	this->opcodes[0xA7] = &CPU::Opcode0xA7;
	this->opcodes[0xA8] = &CPU::Opcode0xA8;
	this->opcodes[0xA9] = &CPU::Opcode0xA9;
	this->opcodes[0xAA] = &CPU::Opcode0xAA;
	this->opcodes[0xAB] = &CPU::Opcode0xAB;
	this->opcodes[0xAC] = &CPU::Opcode0xAC;
	this->opcodes[0xAD] = &CPU::Opcode0xAD;
	this->opcodes[0xAE] = &CPU::Opcode0xAE;
	this->opcodes[0xAF] = &CPU::Opcode0xAF;
	this->opcodes[0xB0] = &CPU::Opcode0xB0;
	this->opcodes[0xB1] = &CPU::Opcode0xB1;
	this->opcodes[0xB2] = &CPU::Opcode0xB2;
	this->opcodes[0xB3] = &CPU::Opcode0xB3;
	this->opcodes[0xB4] = &CPU::Opcode0xB4;
	this->opcodes[0xB5] = &CPU::Opcode0xB5;
	this->opcodes[0xB6] = &CPU::Opcode0xB6;
	this->opcodes[0xB7] = &CPU::Opcode0xB7;
	this->opcodes[0xB8] = &CPU::Opcode0xB8;
	this->opcodes[0xB9] = &CPU::Opcode0xB9;
	this->opcodes[0xBA] = &CPU::Opcode0xBA;
	this->opcodes[0xBB] = &CPU::Opcode0xBB;
	this->opcodes[0xBC] = &CPU::Opcode0xBC;
	this->opcodes[0xBD] = &CPU::Opcode0xBD;
	this->opcodes[0xBE] = &CPU::Opcode0xBE;
	this->opcodes[0xBF] = &CPU::Opcode0xBF;
	this->opcodes[0xC0] = &CPU::Opcode0xC0;
	this->opcodes[0xC1] = &CPU::Opcode0xC1;
	this->opcodes[0xC2] = &CPU::Opcode0xC2;
	this->opcodes[0xC3] = &CPU::Opcode0xC3;
	this->opcodes[0xC4] = &CPU::Opcode0xC4;
	this->opcodes[0xC5] = &CPU::Opcode0xC5;
	this->opcodes[0xC6] = &CPU::Opcode0xC6;
	this->opcodes[0xC7] = &CPU::Opcode0xC7;
	this->opcodes[0xC8] = &CPU::Opcode0xC8;
	this->opcodes[0xC9] = &CPU::Opcode0xC9;
	this->opcodes[0xCA] = &CPU::Opcode0xCA;
	this->opcodes[0xCB] = &CPU::Opcode0xCB;
	this->opcodes[0xCC] = &CPU::Opcode0xCC;
	this->opcodes[0xCD] = &CPU::Opcode0xCD;
	this->opcodes[0xCE] = &CPU::Opcode0xCE;
	this->opcodes[0xCF] = &CPU::Opcode0xCF;
	this->opcodes[0xD0] = &CPU::Opcode0xD0;
	this->opcodes[0xD1] = &CPU::Opcode0xD1;
	this->opcodes[0xD2] = &CPU::Opcode0xD2;
	this->opcodes[0xD3] = &CPU::Opcode0xD3;
	this->opcodes[0xD4] = &CPU::Opcode0xD4;
	this->opcodes[0xD5] = &CPU::Opcode0xD5;
	this->opcodes[0xD6] = &CPU::Opcode0xD6;
	this->opcodes[0xD7] = &CPU::Opcode0xD7;
	this->opcodes[0xD8] = &CPU::Opcode0xD8;
	this->opcodes[0xD9] = &CPU::Opcode0xD9;
	this->opcodes[0xDA] = &CPU::Opcode0xDA;
	this->opcodes[0xDB] = &CPU::Opcode0xDB;
	this->opcodes[0xDC] = &CPU::Opcode0xDC;
	this->opcodes[0xDD] = &CPU::Opcode0xDD;
	this->opcodes[0xDE] = &CPU::Opcode0xDE;
	this->opcodes[0xDF] = &CPU::Opcode0xDF;
	this->opcodes[0xE0] = &CPU::Opcode0xE0;
	this->opcodes[0xE1] = &CPU::Opcode0xE1;
	this->opcodes[0xE2] = &CPU::Opcode0xE2;
	this->opcodes[0xE3] = &CPU::Opcode0xE3;
	this->opcodes[0xE4] = &CPU::Opcode0xE4;
	this->opcodes[0xE5] = &CPU::Opcode0xE5;
	this->opcodes[0xE6] = &CPU::Opcode0xE6;
	this->opcodes[0xE7] = &CPU::Opcode0xE7;
	this->opcodes[0xE8] = &CPU::Opcode0xE8;
	this->opcodes[0xE9] = &CPU::Opcode0xE9;
	this->opcodes[0xEA] = &CPU::Opcode0xEA;
	this->opcodes[0xEB] = &CPU::Opcode0xEB;
	this->opcodes[0xEC] = &CPU::Opcode0xEC;
	this->opcodes[0xED] = &CPU::Opcode0xED;
	this->opcodes[0xEE] = &CPU::Opcode0xEE;
	this->opcodes[0xEF] = &CPU::Opcode0xEF;
	this->opcodes[0xF0] = &CPU::Opcode0xF0;
	this->opcodes[0xF1] = &CPU::Opcode0xF1;
	this->opcodes[0xF2] = &CPU::Opcode0xF2;
	this->opcodes[0xF3] = &CPU::Opcode0xF3;
	this->opcodes[0xF4] = &CPU::Opcode0xF4;
	this->opcodes[0xF5] = &CPU::Opcode0xF5;
	this->opcodes[0xF6] = &CPU::Opcode0xF6;
	this->opcodes[0xF7] = &CPU::Opcode0xF7;
	this->opcodes[0xF8] = &CPU::Opcode0xF8;
	this->opcodes[0xF9] = &CPU::Opcode0xF9;
	this->opcodes[0xFA] = &CPU::Opcode0xFA;
	this->opcodes[0xFB] = &CPU::Opcode0xFB;
	this->opcodes[0xFC] = &CPU::Opcode0xFC;
	this->opcodes[0xFD] = &CPU::Opcode0xFD;
	this->opcodes[0xFE] = &CPU::Opcode0xFE;
	this->opcodes[0xFF] = &CPU::Opcode0xFF;
}

void CPU::Opcode0x00(){}
void CPU::Opcode0x01(){}
void CPU::Opcode0x02(){}
void CPU::Opcode0x03(){}
void CPU::Opcode0x04(){}
void CPU::Opcode0x05(){}
void CPU::Opcode0x06(){}
void CPU::Opcode0x07(){}
void CPU::Opcode0x08(){}
void CPU::Opcode0x09(){}
void CPU::Opcode0x0A(){}
void CPU::Opcode0x0B(){}
void CPU::Opcode0x0C(){}
void CPU::Opcode0x0D(){}
void CPU::Opcode0x0E(){}
void CPU::Opcode0x0F(){}
void CPU::Opcode0x10(){}
void CPU::Opcode0x11(){}
void CPU::Opcode0x12(){}
void CPU::Opcode0x13(){}
void CPU::Opcode0x14(){}
void CPU::Opcode0x15(){}
void CPU::Opcode0x16(){}
void CPU::Opcode0x17(){}
void CPU::Opcode0x18(){}
void CPU::Opcode0x19(){}
void CPU::Opcode0x1A(){}
void CPU::Opcode0x1B(){}
void CPU::Opcode0x1C(){}
void CPU::Opcode0x1D(){}
void CPU::Opcode0x1E(){}
void CPU::Opcode0x1F(){}
void CPU::Opcode0x20(){}
void CPU::Opcode0x21(){}
void CPU::Opcode0x22(){}
void CPU::Opcode0x23(){}
void CPU::Opcode0x24(){}
void CPU::Opcode0x25(){}
void CPU::Opcode0x26(){}
void CPU::Opcode0x27(){}
void CPU::Opcode0x28(){}
void CPU::Opcode0x29(){}
void CPU::Opcode0x2A(){}
void CPU::Opcode0x2B(){}
void CPU::Opcode0x2C(){}
void CPU::Opcode0x2D(){}
void CPU::Opcode0x2E(){}
void CPU::Opcode0x2F(){}
void CPU::Opcode0x30(){}
void CPU::Opcode0x31(){}
void CPU::Opcode0x32(){}
void CPU::Opcode0x33(){}
void CPU::Opcode0x34(){}
void CPU::Opcode0x35(){}
void CPU::Opcode0x36(){}
void CPU::Opcode0x37(){}
void CPU::Opcode0x38(){}
void CPU::Opcode0x39(){}
void CPU::Opcode0x3A(){}
void CPU::Opcode0x3B(){}
void CPU::Opcode0x3C(){}
void CPU::Opcode0x3D(){}
void CPU::Opcode0x3E(){}
void CPU::Opcode0x3F(){}
void CPU::Opcode0x40(){}
void CPU::Opcode0x41(){}
void CPU::Opcode0x42(){}
void CPU::Opcode0x43(){}
void CPU::Opcode0x44(){}
void CPU::Opcode0x45(){}
void CPU::Opcode0x46(){}
void CPU::Opcode0x47(){}
void CPU::Opcode0x48(){}
void CPU::Opcode0x49(){}
void CPU::Opcode0x4A(){}
void CPU::Opcode0x4B(){}
void CPU::Opcode0x4C(){}
void CPU::Opcode0x4D(){}
void CPU::Opcode0x4E(){}
void CPU::Opcode0x4F(){}
void CPU::Opcode0x50(){}
void CPU::Opcode0x51(){}
void CPU::Opcode0x52(){}
void CPU::Opcode0x53(){}
void CPU::Opcode0x54(){}
void CPU::Opcode0x55(){}
void CPU::Opcode0x56(){}
void CPU::Opcode0x57(){}
void CPU::Opcode0x58(){}
void CPU::Opcode0x59(){}
void CPU::Opcode0x5A(){}
void CPU::Opcode0x5B(){}
void CPU::Opcode0x5C(){}
void CPU::Opcode0x5D(){}
void CPU::Opcode0x5E(){}
void CPU::Opcode0x5F(){}
void CPU::Opcode0x60(){}
void CPU::Opcode0x61(){}
void CPU::Opcode0x62(){}
void CPU::Opcode0x63(){}
void CPU::Opcode0x64(){}
void CPU::Opcode0x65(){}
void CPU::Opcode0x66(){}
void CPU::Opcode0x67(){}
void CPU::Opcode0x68(){}
void CPU::Opcode0x69(){}
void CPU::Opcode0x6A(){}
void CPU::Opcode0x6B(){}
void CPU::Opcode0x6C(){}
void CPU::Opcode0x6D(){}
void CPU::Opcode0x6E(){}
void CPU::Opcode0x6F(){}
void CPU::Opcode0x70(){}
void CPU::Opcode0x71(){}
void CPU::Opcode0x72(){}
void CPU::Opcode0x73(){}
void CPU::Opcode0x74(){}
void CPU::Opcode0x75(){}
void CPU::Opcode0x76(){}
void CPU::Opcode0x77(){}
void CPU::Opcode0x78(){}
void CPU::Opcode0x79(){}
void CPU::Opcode0x7A(){}
void CPU::Opcode0x7B(){}
void CPU::Opcode0x7C(){}
void CPU::Opcode0x7D(){}
void CPU::Opcode0x7E(){}
void CPU::Opcode0x7F(){}
void CPU::Opcode0x80(){}
void CPU::Opcode0x81(){}
void CPU::Opcode0x82(){}
void CPU::Opcode0x83(){}
void CPU::Opcode0x84(){}
void CPU::Opcode0x85(){}
void CPU::Opcode0x86(){}
void CPU::Opcode0x87(){}
void CPU::Opcode0x88(){}
void CPU::Opcode0x89(){}
void CPU::Opcode0x8A(){}
void CPU::Opcode0x8B(){}
void CPU::Opcode0x8C(){}
void CPU::Opcode0x8D(){}
void CPU::Opcode0x8E(){}
void CPU::Opcode0x8F(){}
void CPU::Opcode0x90(){}
void CPU::Opcode0x91(){}
void CPU::Opcode0x92(){}
void CPU::Opcode0x93(){}
void CPU::Opcode0x94(){}
void CPU::Opcode0x95(){}
void CPU::Opcode0x96(){}
void CPU::Opcode0x97(){}
void CPU::Opcode0x98(){}
void CPU::Opcode0x99(){}
void CPU::Opcode0x9A(){}
void CPU::Opcode0x9B(){}
void CPU::Opcode0x9C(){}
void CPU::Opcode0x9D(){}
void CPU::Opcode0x9E(){}
void CPU::Opcode0x9F(){}
void CPU::Opcode0xA0(){}
void CPU::Opcode0xA1(){}
void CPU::Opcode0xA2(){}
void CPU::Opcode0xA3(){}
void CPU::Opcode0xA4(){}
void CPU::Opcode0xA5(){}
void CPU::Opcode0xA6(){}
void CPU::Opcode0xA7(){}
void CPU::Opcode0xA8(){}
void CPU::Opcode0xA9(){}
void CPU::Opcode0xAA(){}
void CPU::Opcode0xAB(){}
void CPU::Opcode0xAC(){}
void CPU::Opcode0xAD(){}
void CPU::Opcode0xAE(){}
void CPU::Opcode0xAF(){}
void CPU::Opcode0xB0(){}
void CPU::Opcode0xB1(){}
void CPU::Opcode0xB2(){}
void CPU::Opcode0xB3(){}
void CPU::Opcode0xB4(){}
void CPU::Opcode0xB5(){}
void CPU::Opcode0xB6(){}
void CPU::Opcode0xB7(){}
void CPU::Opcode0xB8(){}
void CPU::Opcode0xB9(){}
void CPU::Opcode0xBA(){}
void CPU::Opcode0xBB(){}
void CPU::Opcode0xBC(){}
void CPU::Opcode0xBD(){}
void CPU::Opcode0xBE(){}
void CPU::Opcode0xBF(){}
void CPU::Opcode0xC0(){}
void CPU::Opcode0xC1(){}
void CPU::Opcode0xC2(){}
void CPU::Opcode0xC3(){}
void CPU::Opcode0xC4(){}
void CPU::Opcode0xC5(){}
void CPU::Opcode0xC6(){}
void CPU::Opcode0xC7(){}
void CPU::Opcode0xC8(){}
void CPU::Opcode0xC9(){}
void CPU::Opcode0xCA(){}
void CPU::Opcode0xCB(){}
void CPU::Opcode0xCC(){}
void CPU::Opcode0xCD(){}
void CPU::Opcode0xCE(){}
void CPU::Opcode0xCF(){}
void CPU::Opcode0xD0(){}
void CPU::Opcode0xD1(){}
void CPU::Opcode0xD2(){}
void CPU::Opcode0xD3(){}
void CPU::Opcode0xD4(){}
void CPU::Opcode0xD5(){}
void CPU::Opcode0xD6(){}
void CPU::Opcode0xD7(){}
void CPU::Opcode0xD8(){}
void CPU::Opcode0xD9(){}
void CPU::Opcode0xDA(){}
void CPU::Opcode0xDB(){}
void CPU::Opcode0xDC(){}
void CPU::Opcode0xDD(){}
void CPU::Opcode0xDE(){}
void CPU::Opcode0xDF(){}
void CPU::Opcode0xE0(){}
void CPU::Opcode0xE1(){}
void CPU::Opcode0xE2(){}
void CPU::Opcode0xE3(){}
void CPU::Opcode0xE4(){}
void CPU::Opcode0xE5(){}
void CPU::Opcode0xE6(){}
void CPU::Opcode0xE7(){}
void CPU::Opcode0xE8(){}
void CPU::Opcode0xE9(){}
void CPU::Opcode0xEA(){}
void CPU::Opcode0xEB(){}
void CPU::Opcode0xEC(){}
void CPU::Opcode0xED(){}
void CPU::Opcode0xEE(){}
void CPU::Opcode0xEF(){}
void CPU::Opcode0xF0(){}
void CPU::Opcode0xF1(){}
void CPU::Opcode0xF2(){}
void CPU::Opcode0xF3(){}
void CPU::Opcode0xF4(){}
void CPU::Opcode0xF5(){}
void CPU::Opcode0xF6(){}
void CPU::Opcode0xF7(){}
void CPU::Opcode0xF8(){}
void CPU::Opcode0xF9(){}
void CPU::Opcode0xFA(){}
void CPU::Opcode0xFB(){}
void CPU::Opcode0xFC(){}
void CPU::Opcode0xFD(){}
void CPU::Opcode0xFE(){}
void CPU::Opcode0xFF(){}
#include "cpu.h"

Register::Register() {
	this->low = NULL;
	this->high = NULL;
}

Register::~Register() {}

Register::Register(uint8_t* h, uint8_t* l) {
	high = h;
	low = l;
}

uint16_t Register::getRegister() {
	return *high << 8 | (*low & 0x00FF);
}

void Register::setRegister(uint16_t value) {
	*high = value >> 8;
	*low = static_cast<uint8_t>(value & 0x00FF);
}

CPU::CPU(MMU * mmu) {
	this->mmu = mmu;
	this->initialize();
}

CPU::~CPU() {}

void CPU::initialize() {
	bindOpcodes();
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
	mmu->set(0xFF05, 0x00);
	mmu->set(0xFF06, 0x00);
	mmu->set(0xFF07, 0x00);
	mmu->set(0xFF10, 0x80);
	mmu->set(0xFF11, 0xBF);
	mmu->set(0xFF12, 0xF3);
	mmu->set(0xFF14, 0xBF);
	mmu->set(0xFF16, 0x3F);
	mmu->set(0xFF17, 0x00);
	mmu->set(0xFF19, 0xBF);
	mmu->set(0xFF1A, 0x7F);
	mmu->set(0xFF1B, 0xFF);
	mmu->set(0xFF1C, 0x9F);
	mmu->set(0xFF1E, 0xBF);
	mmu->set(0xFF20, 0xFF);
	mmu->set(0xFF21, 0x00);
	mmu->set(0xFF22, 0x00);
	mmu->set(0xFF23, 0xBF);
	mmu->set(0xFF24, 0x77);
	mmu->set(0xFF25, 0xF3);
	mmu->set(0xFF26, 0xF1);
	mmu->set(0xFF40, 0x91);
	mmu->set(0xFF42, 0x00);
	mmu->set(0xFF43, 0x00);
	mmu->set(0xFF45, 0x00);
	mmu->set(0xFF47, 0xFC);
	mmu->set(0xFF48, 0xFF);
	mmu->set(0xFF49, 0xFF);
	mmu->set(0xFF4A, 0x00);
	mmu->set(0xFF4B, 0x00);
	mmu->set(0xFFFF, 0x00);
	// Set stack pointer, program counter and cycles members to 
	// default values
	pc = 0x100;
	cycles = 0x0000;
	sp = 0xFFFE;
	std::string f = "debug.txt";
	dbg = std::ofstream (f, std::ios::binary);
}

void CPU::cycle() {
	execute(mmu->memory[+pc]);
}

void CPU::execute(uint8_t inst) {
	/* here lies the forbidden code  */
	dbg << std::hex << std::uppercase << std::setfill('0') <<
		"A: " << std::setw(2) << +*AF.high << 
		" F: " << std::setw(2) << +*AF.low	<< 
		" B: " << std::setw(2) << +*BC.high << 
		" C: " << std::setw(2) << +*BC.low << 
		" D: " << std::setw(2) << +*DE.high << 
		" E: " << std::setw(2) << +*DE.low << 
		" H: " << std::setw(2) << +*HL.high << 
		" L: " << std::setw(2) << +*HL.low << 
		" SP: " << std::setw(4) << +sp << 
		" PC: " << std::setw(4) << +pc << "\n";
	if (++count == 50000) {
		PrintMessage(Debug, "Operation completed");
		dbg.close();
	}
	if (!extended) {
		(this->*opcodes[inst])();
	}
	else {
		(this->*extendedOpcodes[inst])();
		extended = false;
	}
	if (!halted) pc++;
}

bool CPU::didCarry(uint8_t reg) {
	return (A + reg) > 0xFF;
}

bool CPU::didHalfCarry(uint8_t reg) {
	return (reg & 0x0F) == 0x0F;
}

bool CPU::didBorrow(uint8_t reg) {
	return static_cast<int8_t>((A - reg)) < 0x0;
}

bool CPU::didHalfBorrow(uint8_t reg) {
	return static_cast<int8_t>((A & 0x0F) - (reg & 0x0F)) < 0x0;
}

bool CPU::didCarry16(uint16_t reg, uint16_t reg2) {
	return (reg + reg2) > 0xFFFF;
}

bool CPU::didHalfCarry16(uint16_t reg, uint16_t reg2) {
	return ((reg & 0x00FF) + (reg2 & 0x00FF)) > 0x00FF;
}

uint8_t CPU::getFlag(uint8_t flag) {
	return mmu->getBit(F, flag);
}

void CPU::setFlag(uint8_t flag) {
	mmu->setBit(F, flag);
}

void CPU::clearFlag(uint8_t flag) {
	mmu->clearBit(F, flag);
}

void CPU::getFlags() {
	std::bitset<8> flags(*AF.low);
	std::cout << "FLAGS " << flags.to_string() << std::endl;
}

void CPU::PUSHSTACK16(uint16_t word) {
	mmu->set(sp - 1, static_cast<uint8_t>((word >> 8) & 0xFF));
	mmu->set(sp - 2, static_cast<uint8_t>(word & 0xFF));
	sp -= 2;
}

void CPU::POPSTACK(Register reg) {
	*reg.high = mmu->get(sp + 1);
	*reg.low = mmu->get(sp);
	sp += 2;
}

void CPU::POPSTACK16() {
	pc = mmu->formWord(mmu->get(sp), mmu->get(sp + 1));
	sp += 2;
}

/* ASSIGNMENT FUNCTIONS */

void CPU::LD(uint8_t& reg1, uint8_t reg2) {
	reg1 = reg2;
}

void CPU::LD(uint16_t address, uint8_t reg) {
	mmu->set(address, reg);
}

void CPU::LD(uint8_t& reg, uint16_t address) {
	reg = mmu->get(address);
}

/* ARITHMETIC FUNCTIONS */

void CPU::ADD(uint8_t reg) {
	uint8_t eval = static_cast<uint8_t>(A + reg);
	eval == 0 ? setFlag(FLAG_Z) : clearFlag(FLAG_Z);
	clearFlag(FLAG_N);
	didHalfCarry(reg) ? setFlag(FLAG_H) : clearFlag(FLAG_H);
	didCarry(reg) ? setFlag(FLAG_C) : clearFlag(FLAG_C);
}

void CPU::ADD_HL(uint16_t v) {
	HL.setRegister(HL.getRegister() + v);
	clearFlag(FLAG_N);
	didHalfCarry16(HL.getRegister(), v) ? setFlag(FLAG_H) : clearFlag(FLAG_H);
	didCarry16(HL.getRegister(), v) ? setFlag(FLAG_C): clearFlag(FLAG_C);
}

void CPU::ADD_SP() {
	uint8_t imm = mmu->get(pc++);
	sp += imm;
	clearFlag(FLAG_Z);
	clearFlag(FLAG_N);
	((sp & 0x0FFF) + imm) > 0x0FFF ? setFlag(FLAG_H) : clearFlag(FLAG_H);
	didCarry16(sp, static_cast<uint16_t>(imm)) ? setFlag(FLAG_C) : clearFlag(FLAG_C);
}

void CPU::ADC(uint8_t reg) {
	uint8_t carry = mmu->getBit(F, FLAG_C);
	uint8_t eval = static_cast<uint8_t>(A + reg + carry);
	eval == 0 ? setFlag(FLAG_Z) : clearFlag(FLAG_Z);
	clearFlag(FLAG_N);
	didHalfCarry(reg + carry) ? setFlag(FLAG_H) : clearFlag(FLAG_H);
	didCarry(reg + carry) ? setFlag(FLAG_C) : setFlag(FLAG_C);
	A = eval;
}

void CPU::SUB(uint8_t reg) {
	int8_t eval = static_cast<int8_t>(A - reg);
	eval == 0 ? setFlag(FLAG_Z) : clearFlag(FLAG_Z);
	setFlag(FLAG_N);
	didHalfBorrow(reg) ? setFlag(FLAG_H) : clearFlag(FLAG_H);
	didBorrow(reg) ? setFlag(FLAG_C) : clearFlag(FLAG_C);
	A = eval;
}

void CPU::SBC(uint8_t reg) {
	uint8_t carry = getFlag(FLAG_C);
	int8_t eval = static_cast<int8_t>(A - (reg + carry));
	eval == 0 ? setFlag(FLAG_Z) : clearFlag(FLAG_Z);
	setFlag(FLAG_N);
	didHalfBorrow(reg + carry) ? setFlag(FLAG_H) : clearFlag(FLAG_H);
	didBorrow(reg + carry) ? setFlag(FLAG_C) : clearFlag(FLAG_C);
	A = eval;
}

/* LOGICAL FUNCTIONS */

void CPU::AND(uint8_t reg) {
	A &= reg;
	A == 0 ? setFlag(FLAG_Z) : clearFlag(FLAG_Z);
	clearFlag(FLAG_N);
	setFlag(FLAG_H);
	clearFlag(FLAG_C);
}

void CPU::OR(uint8_t reg) {
	A |= reg;
	A == 0 ? setFlag(FLAG_Z) : clearFlag(FLAG_Z);
	clearFlag(FLAG_N);
	clearFlag(FLAG_H);
	clearFlag(FLAG_C);
}

void CPU::XOR(uint8_t reg) {
	A ^= reg;
	A == 0 ? setFlag(FLAG_Z) : clearFlag(FLAG_Z);
	clearFlag(FLAG_N);
	clearFlag(FLAG_H);
	clearFlag(FLAG_C);
}

void CPU::CP(uint8_t reg) {
	uint8_t eval =  A - reg;
	eval == 0 || eval == reg ? setFlag(FLAG_Z) : clearFlag(FLAG_Z);
	setFlag(FLAG_N);
	if (eval > 0x0F) {
		setFlag(FLAG_H);
	}
	else {
		clearFlag(FLAG_H);
	}
	didCarry(eval) ? setFlag(FLAG_C) : clearFlag(FLAG_C); 
}

void CPU::INC(uint8_t & reg) {
	reg += 1;
	reg == 0 ? setFlag(FLAG_Z) : clearFlag(FLAG_Z);
	clearFlag(FLAG_N);
	if ((reg & 0x0F) == 0) {
		setFlag(FLAG_H);
	}
	else {
		clearFlag(FLAG_H);
	}
 }

void CPU::INC(Register reg) {
	reg.setRegister(reg.getRegister() + 1);
}

void CPU::INC_SP() {
	sp += 1;
}

void CPU::DEC(uint8_t & reg) {
	reg -= 1;
	reg == 0 ? setFlag(FLAG_Z) : clearFlag(FLAG_Z);
	setFlag(FLAG_N);
	if ((reg & 0x0F) == 0x0F) {
		setFlag(FLAG_H);
	}
	else {
		clearFlag(FLAG_H);
	}
}

void CPU::DEC(Register reg) {
	reg.setRegister(reg.getRegister() - 1);
}

void CPU::DEC_SP() {
	sp -= 1;
}

/* BIT SHIFT FUNCTIONS*/

void CPU::RLCA() {
	uint8_t bit = mmu->getBit(A, 7);
	A <<= 1;
	clearFlag(FLAG_Z);
	clearFlag(FLAG_N);
	clearFlag(FLAG_H);
	if (bit) {
		mmu->setBit(A, 0);
		setFlag(FLAG_C);
	}
	else {
		mmu->clearBit(A, 0);
		clearFlag(FLAG_C);
	}
}

void CPU::RRCA() {
	uint8_t bit = mmu->getBit(A, 0);
	A >>= 1;
	clearFlag(FLAG_Z);
	clearFlag(FLAG_N);
	clearFlag(FLAG_H);
	if (bit) {
		mmu->setBit(A, 7);
		setFlag(FLAG_C);
	}
	else {
		mmu->clearBit(A, 7);
		clearFlag(FLAG_C);
	}
}

/* 
	The only difference between this function and RLCA
	is to ignore assigning bit 7 to bit 0. The 'C' in 
	'RLCA' is circular!
*/
void CPU::RLA() {
	uint8_t bit = mmu->getBit(A, 7);
	A <<= 1;
	clearFlag(FLAG_Z);
	clearFlag(FLAG_N);
	clearFlag(FLAG_H);
	if (bit) {
		setFlag(FLAG_C);
	}
	else {
		clearFlag(FLAG_C);
	}
}

void CPU::RRA() {
	uint8_t bit = mmu->getBit(A, 0);
	A >>= 1;
	clearFlag(FLAG_Z);
	clearFlag(FLAG_N);
	clearFlag(FLAG_H);
	if (bit) {
		setFlag(FLAG_C);
	}
	else {
		clearFlag(FLAG_C);
	}
}

void CPU::RLC(uint8_t * reg) {
	uint8_t bit = mmu->getBit(*reg, 7);
	*reg <<= 1;
	// Set flag Z if operand is 0
	*reg ? clearFlag(FLAG_Z) : setFlag(FLAG_Z);
	if (bit) {
		setFlag(FLAG_C);
		mmu->setBit(*reg, 0);
	}
	else {
		clearFlag(FLAG_C);
		mmu->clearBit(*reg, 0);
	}
}

void CPU::RL(uint8_t * reg) {
	uint8_t bit = mmu->getBit(*reg, 7);
	uint8_t carry = getFlag(FLAG_C);
	carry ? mmu->setBit(*reg, 0) : mmu->clearBit(*reg, 0);
	*reg <<= 1;
	*reg ? clearFlag(FLAG_Z) : setFlag(FLAG_Z);
	clearFlag(FLAG_N);
	clearFlag(FLAG_H);
	if (bit) {
		setFlag(FLAG_C);
	}
	else {
		clearFlag(FLAG_C);
	}
}

void CPU::RR(uint8_t * reg) {
	uint8_t bit = mmu->getBit(*reg, 0);
	uint8_t carry = getFlag(FLAG_C);
	carry ? mmu->setBit(*reg, 7) : mmu->clearBit(*reg, 7);
	*reg >>= 1;
	*reg ? clearFlag(FLAG_Z) : setFlag(FLAG_Z);
	clearFlag(FLAG_N);
	clearFlag(FLAG_H);
	if (bit) {
		setFlag(FLAG_C);
	}
	else {
		clearFlag(FLAG_C);
	}
}

void CPU::RRC(uint8_t* reg) {
	uint8_t bit = mmu->getBit(*reg, 0);
	*reg >>= 1;
	// Set flag Z if operand is 0
	*reg ? clearFlag(FLAG_Z) : setFlag(FLAG_Z);
	if (bit) {
		setFlag(FLAG_C);
		mmu->setBit(*reg, 7);
	}
	else {
		clearFlag(FLAG_C);
		mmu->clearBit(*reg, 7);
	}
}

void CPU::SLA(uint8_t * reg) {
	*reg <<= 1;
	uint8_t bit = mmu->getBit(*reg, 7);
	mmu->clearBit(*reg, 0);
	*reg ? clearFlag(FLAG_Z) : setFlag(FLAG_Z);
	clearFlag(FLAG_N);
	clearFlag(FLAG_H);
	if (bit) {
		setFlag(FLAG_C);
	}
	else {
		clearFlag(FLAG_C);
	}
}

void CPU::SRA(uint8_t * reg) {
	*reg >>= 1;
	uint8_t bit = mmu->getBit(*reg, 0);
	// Bit 7 is unchanged
	*reg ? clearFlag(FLAG_Z) : setFlag(FLAG_Z);
	clearFlag(FLAG_N);
	clearFlag(FLAG_H);
	if (bit) {
		setFlag(FLAG_C);
	}
	else {
		clearFlag(FLAG_C);
	}
}

void CPU::SRL(uint8_t * reg) {
	*reg >>= 1;
	uint8_t bit = mmu->getBit(*reg, 0);
	mmu->clearBit(*reg, 7);
	*reg ? clearFlag(FLAG_Z) : setFlag(FLAG_Z);
	clearFlag(FLAG_N);
	clearFlag(FLAG_H);
	if (bit) {
		setFlag(FLAG_C);
	}
	else {
		clearFlag(FLAG_C);
	}
}

void CPU::SWAP(uint8_t * reg) {
	*reg = ((*reg & 0x0F) << 4) | ((*reg & 0xF0) >> 4);
	*reg ? clearFlag(FLAG_Z) : setFlag(FLAG_Z);
	clearFlag(FLAG_N);
	clearFlag(FLAG_H);
	clearFlag(FLAG_C);
}

/* BIT OPERATIONS */

void CPU::BIT(uint8_t bit, Register reg) {
	uint8_t b = mmu->getBit(reg.getRegister(), bit);
	b == 0 ? setFlag(FLAG_Z) : clearFlag(FLAG_Z);
	clearFlag(FLAG_N);
	setFlag(FLAG_H);
}

void CPU::SET(uint8_t bit, Register reg) {
	uint8_t r = reg.getRegister();
	r |= 1UL << bit;
}

void CPU::RES(uint8_t bit, Register reg) {
	uint8_t r = reg.getRegister();
	r &= ~(1UL << bit);
}

/* JUMP INSTRUCTIONS */

void CPU::JP() {
	uint16_t low = mmu->get(pc + 1);
	uint16_t high = mmu->get(pc + 2) << 8;
	pc = (high | low) - 1;
}

void CPU::JP_HL() {
	pc = HL.getRegister();
}

void CPU::JP(int condition) {
	bool match = false;
	switch (condition) {
	case 0:
		getFlag(FLAG_Z) ? match : match = true;
	case 1:
		getFlag(FLAG_Z) ? match = true : match;
	case 2:
		getFlag(FLAG_C) ? match : match = true;
	case 3:
		getFlag(FLAG_C) ? match = true : match;
	default:
		break;
	}
	if (match) {
		JP();
	}
	else {
		pc++;
	}
}

void CPU::JR() {
	pc += 1 + (static_cast <int8_t> (mmu->get(pc + 1)));
}

void CPU::CALL() {
	mmu->set(sp - 1, static_cast<uint8_t>(((pc + 3) >> 8) & 0xFF));
	mmu->set(sp - 2, static_cast<uint8_t>((pc + 3) & 0xFF));
	sp -= 2;
	uint16_t imm = mmu->formWord(mmu->get(pc + 2), mmu->get(pc + 1));
	pc = imm - 1;
}

void CPU::RET() {
	uint8_t low = mmu->get(sp);
	sp++;
	uint8_t high = mmu->get(sp);
	sp++;
	pc = mmu->formWord(high, low) - 1;
}

void CPU::RETI() {
	PrintMessage(Debug, "Implement RETI!");
}

void CPU::RST(uint8_t vec) {
	PUSHSTACK16(pc);
	uint8_t pl = RSTJumpVectors[+vec];
	pc = mmu->formWord(0, pl);
}

void CPU::DAA() {
	PrintMessage(Debug, "Implement DAA!");
}

void CPU::CPL() {
	A = ~A;
	setFlag(FLAG_N);
	setFlag(FLAG_H);
}

void CPU::NOP() {
}

void CPU::CCF() {
	mmu->toggleBit(F, FLAG_C);
}

void CPU::SCF() {
	setFlag(FLAG_C);
}

void CPU::DI() {
	ime = false;
}

void CPU::EI() {
	ime = true;
}

void CPU::HALT() {
	halted = true;
}

void CPU::STOP() {
	// Stop the system clock and the oscillator circuit
	// Stop the LCD controller
	// Before STOP is called:
	//	- Reset interrupt enable flags
	//	- I/O P10 - P13 
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

	this->extendedOpcodes[0x00] = &CPU::extendedOpcode0x00;
	this->extendedOpcodes[0x01] = &CPU::extendedOpcode0x01;
	this->extendedOpcodes[0x02] = &CPU::extendedOpcode0x02;
	this->extendedOpcodes[0x03] = &CPU::extendedOpcode0x03;
	this->extendedOpcodes[0x04] = &CPU::extendedOpcode0x04;
	this->extendedOpcodes[0x05] = &CPU::extendedOpcode0x05;
	this->extendedOpcodes[0x06] = &CPU::extendedOpcode0x06;
	this->extendedOpcodes[0x07] = &CPU::extendedOpcode0x07;
	this->extendedOpcodes[0x08] = &CPU::extendedOpcode0x08;
	this->extendedOpcodes[0x09] = &CPU::extendedOpcode0x09;
	this->extendedOpcodes[0x0A] = &CPU::extendedOpcode0x0A;
	this->extendedOpcodes[0x0B] = &CPU::extendedOpcode0x0B;
	this->extendedOpcodes[0x0C] = &CPU::extendedOpcode0x0C;
	this->extendedOpcodes[0x0D] = &CPU::extendedOpcode0x0D;
	this->extendedOpcodes[0x0E] = &CPU::extendedOpcode0x0E;
	this->extendedOpcodes[0x0F] = &CPU::extendedOpcode0x0F;
	this->extendedOpcodes[0x10] = &CPU::extendedOpcode0x10;
	this->extendedOpcodes[0x11] = &CPU::extendedOpcode0x11;
	this->extendedOpcodes[0x12] = &CPU::extendedOpcode0x12;
	this->extendedOpcodes[0x13] = &CPU::extendedOpcode0x13;
	this->extendedOpcodes[0x14] = &CPU::extendedOpcode0x14;
	this->extendedOpcodes[0x15] = &CPU::extendedOpcode0x15;
	this->extendedOpcodes[0x16] = &CPU::extendedOpcode0x16;
	this->extendedOpcodes[0x17] = &CPU::extendedOpcode0x17;
	this->extendedOpcodes[0x18] = &CPU::extendedOpcode0x18;
	this->extendedOpcodes[0x19] = &CPU::extendedOpcode0x19;
	this->extendedOpcodes[0x1A] = &CPU::extendedOpcode0x1A;
	this->extendedOpcodes[0x1B] = &CPU::extendedOpcode0x1B;
	this->extendedOpcodes[0x1C] = &CPU::extendedOpcode0x1C;
	this->extendedOpcodes[0x1D] = &CPU::extendedOpcode0x1D;
	this->extendedOpcodes[0x1E] = &CPU::extendedOpcode0x1E;
	this->extendedOpcodes[0x1F] = &CPU::extendedOpcode0x1F;
	this->extendedOpcodes[0x20] = &CPU::extendedOpcode0x20;
	this->extendedOpcodes[0x21] = &CPU::extendedOpcode0x21;
	this->extendedOpcodes[0x22] = &CPU::extendedOpcode0x22;
	this->extendedOpcodes[0x23] = &CPU::extendedOpcode0x23;
	this->extendedOpcodes[0x24] = &CPU::extendedOpcode0x24;
	this->extendedOpcodes[0x25] = &CPU::extendedOpcode0x25;
	this->extendedOpcodes[0x26] = &CPU::extendedOpcode0x26;
	this->extendedOpcodes[0x27] = &CPU::extendedOpcode0x27;
	this->extendedOpcodes[0x28] = &CPU::extendedOpcode0x28;
	this->extendedOpcodes[0x29] = &CPU::extendedOpcode0x29;
	this->extendedOpcodes[0x2A] = &CPU::extendedOpcode0x2A;
	this->extendedOpcodes[0x2B] = &CPU::extendedOpcode0x2B;
	this->extendedOpcodes[0x2C] = &CPU::extendedOpcode0x2C;
	this->extendedOpcodes[0x2D] = &CPU::extendedOpcode0x2D;
	this->extendedOpcodes[0x2E] = &CPU::extendedOpcode0x2E;
	this->extendedOpcodes[0x2F] = &CPU::extendedOpcode0x2F;
	this->extendedOpcodes[0x30] = &CPU::extendedOpcode0x30;
	this->extendedOpcodes[0x31] = &CPU::extendedOpcode0x31;
	this->extendedOpcodes[0x32] = &CPU::extendedOpcode0x32;
	this->extendedOpcodes[0x33] = &CPU::extendedOpcode0x33;
	this->extendedOpcodes[0x34] = &CPU::extendedOpcode0x34;
	this->extendedOpcodes[0x35] = &CPU::extendedOpcode0x35;
	this->extendedOpcodes[0x36] = &CPU::extendedOpcode0x36;
	this->extendedOpcodes[0x37] = &CPU::extendedOpcode0x37;
	this->extendedOpcodes[0x38] = &CPU::extendedOpcode0x38;
	this->extendedOpcodes[0x39] = &CPU::extendedOpcode0x39;
	this->extendedOpcodes[0x3A] = &CPU::extendedOpcode0x3A;
	this->extendedOpcodes[0x3B] = &CPU::extendedOpcode0x3B;
	this->extendedOpcodes[0x3C] = &CPU::extendedOpcode0x3C;
	this->extendedOpcodes[0x3D] = &CPU::extendedOpcode0x3D;
	this->extendedOpcodes[0x3E] = &CPU::extendedOpcode0x3E;
	this->extendedOpcodes[0x3F] = &CPU::extendedOpcode0x3F;
	this->extendedOpcodes[0x40] = &CPU::extendedOpcode0x40;
	this->extendedOpcodes[0x41] = &CPU::extendedOpcode0x41;
	this->extendedOpcodes[0x42] = &CPU::extendedOpcode0x42;
	this->extendedOpcodes[0x43] = &CPU::extendedOpcode0x43;
	this->extendedOpcodes[0x44] = &CPU::extendedOpcode0x44;
	this->extendedOpcodes[0x45] = &CPU::extendedOpcode0x45;
	this->extendedOpcodes[0x46] = &CPU::extendedOpcode0x46;
	this->extendedOpcodes[0x47] = &CPU::extendedOpcode0x47;
	this->extendedOpcodes[0x48] = &CPU::extendedOpcode0x48;
	this->extendedOpcodes[0x49] = &CPU::extendedOpcode0x49;
	this->extendedOpcodes[0x4A] = &CPU::extendedOpcode0x4A;
	this->extendedOpcodes[0x4B] = &CPU::extendedOpcode0x4B;
	this->extendedOpcodes[0x4C] = &CPU::extendedOpcode0x4C;
	this->extendedOpcodes[0x4D] = &CPU::extendedOpcode0x4D;
	this->extendedOpcodes[0x4E] = &CPU::extendedOpcode0x4E;
	this->extendedOpcodes[0x4F] = &CPU::extendedOpcode0x4F;
	this->extendedOpcodes[0x50] = &CPU::extendedOpcode0x50;
	this->extendedOpcodes[0x51] = &CPU::extendedOpcode0x51;
	this->extendedOpcodes[0x52] = &CPU::extendedOpcode0x52;
	this->extendedOpcodes[0x53] = &CPU::extendedOpcode0x53;
	this->extendedOpcodes[0x54] = &CPU::extendedOpcode0x54;
	this->extendedOpcodes[0x55] = &CPU::extendedOpcode0x55;
	this->extendedOpcodes[0x56] = &CPU::extendedOpcode0x56;
	this->extendedOpcodes[0x57] = &CPU::extendedOpcode0x57;
	this->extendedOpcodes[0x58] = &CPU::extendedOpcode0x58;
	this->extendedOpcodes[0x59] = &CPU::extendedOpcode0x59;
	this->extendedOpcodes[0x5A] = &CPU::extendedOpcode0x5A;
	this->extendedOpcodes[0x5B] = &CPU::extendedOpcode0x5B;
	this->extendedOpcodes[0x5C] = &CPU::extendedOpcode0x5C;
	this->extendedOpcodes[0x5D] = &CPU::extendedOpcode0x5D;
	this->extendedOpcodes[0x5E] = &CPU::extendedOpcode0x5E;
	this->extendedOpcodes[0x5F] = &CPU::extendedOpcode0x5F;
	this->extendedOpcodes[0x60] = &CPU::extendedOpcode0x60;
	this->extendedOpcodes[0x61] = &CPU::extendedOpcode0x61;
	this->extendedOpcodes[0x62] = &CPU::extendedOpcode0x62;
	this->extendedOpcodes[0x63] = &CPU::extendedOpcode0x63;
	this->extendedOpcodes[0x64] = &CPU::extendedOpcode0x64;
	this->extendedOpcodes[0x65] = &CPU::extendedOpcode0x65;
	this->extendedOpcodes[0x66] = &CPU::extendedOpcode0x66;
	this->extendedOpcodes[0x67] = &CPU::extendedOpcode0x67;
	this->extendedOpcodes[0x68] = &CPU::extendedOpcode0x68;
	this->extendedOpcodes[0x69] = &CPU::extendedOpcode0x69;
	this->extendedOpcodes[0x6A] = &CPU::extendedOpcode0x6A;
	this->extendedOpcodes[0x6B] = &CPU::extendedOpcode0x6B;
	this->extendedOpcodes[0x6C] = &CPU::extendedOpcode0x6C;
	this->extendedOpcodes[0x6D] = &CPU::extendedOpcode0x6D;
	this->extendedOpcodes[0x6E] = &CPU::extendedOpcode0x6E;
	this->extendedOpcodes[0x6F] = &CPU::extendedOpcode0x6F;
	this->extendedOpcodes[0x70] = &CPU::extendedOpcode0x70;
	this->extendedOpcodes[0x71] = &CPU::extendedOpcode0x71;
	this->extendedOpcodes[0x72] = &CPU::extendedOpcode0x72;
	this->extendedOpcodes[0x73] = &CPU::extendedOpcode0x73;
	this->extendedOpcodes[0x74] = &CPU::extendedOpcode0x74;
	this->extendedOpcodes[0x75] = &CPU::extendedOpcode0x75;
	this->extendedOpcodes[0x76] = &CPU::extendedOpcode0x76;
	this->extendedOpcodes[0x77] = &CPU::extendedOpcode0x77;
	this->extendedOpcodes[0x78] = &CPU::extendedOpcode0x78;
	this->extendedOpcodes[0x79] = &CPU::extendedOpcode0x79;
	this->extendedOpcodes[0x7A] = &CPU::extendedOpcode0x7A;
	this->extendedOpcodes[0x7B] = &CPU::extendedOpcode0x7B;
	this->extendedOpcodes[0x7C] = &CPU::extendedOpcode0x7C;
	this->extendedOpcodes[0x7D] = &CPU::extendedOpcode0x7D;
	this->extendedOpcodes[0x7E] = &CPU::extendedOpcode0x7E;
	this->extendedOpcodes[0x7F] = &CPU::extendedOpcode0x7F;
	this->extendedOpcodes[0x80] = &CPU::extendedOpcode0x80;
	this->extendedOpcodes[0x81] = &CPU::extendedOpcode0x81;
	this->extendedOpcodes[0x82] = &CPU::extendedOpcode0x82;
	this->extendedOpcodes[0x83] = &CPU::extendedOpcode0x83;
	this->extendedOpcodes[0x84] = &CPU::extendedOpcode0x84;
	this->extendedOpcodes[0x85] = &CPU::extendedOpcode0x85;
	this->extendedOpcodes[0x86] = &CPU::extendedOpcode0x86;
	this->extendedOpcodes[0x87] = &CPU::extendedOpcode0x87;
	this->extendedOpcodes[0x88] = &CPU::extendedOpcode0x88;
	this->extendedOpcodes[0x89] = &CPU::extendedOpcode0x89;
	this->extendedOpcodes[0x8A] = &CPU::extendedOpcode0x8A;
	this->extendedOpcodes[0x8B] = &CPU::extendedOpcode0x8B;
	this->extendedOpcodes[0x8C] = &CPU::extendedOpcode0x8C;
	this->extendedOpcodes[0x8D] = &CPU::extendedOpcode0x8D;
	this->extendedOpcodes[0x8E] = &CPU::extendedOpcode0x8E;
	this->extendedOpcodes[0x8F] = &CPU::extendedOpcode0x8F;
	this->extendedOpcodes[0x90] = &CPU::extendedOpcode0x90;
	this->extendedOpcodes[0x91] = &CPU::extendedOpcode0x91;
	this->extendedOpcodes[0x92] = &CPU::extendedOpcode0x92;
	this->extendedOpcodes[0x93] = &CPU::extendedOpcode0x93;
	this->extendedOpcodes[0x94] = &CPU::extendedOpcode0x94;
	this->extendedOpcodes[0x95] = &CPU::extendedOpcode0x95;
	this->extendedOpcodes[0x96] = &CPU::extendedOpcode0x96;
	this->extendedOpcodes[0x97] = &CPU::extendedOpcode0x97;
	this->extendedOpcodes[0x98] = &CPU::extendedOpcode0x98;
	this->extendedOpcodes[0x99] = &CPU::extendedOpcode0x99;
	this->extendedOpcodes[0x9A] = &CPU::extendedOpcode0x9A;
	this->extendedOpcodes[0x9B] = &CPU::extendedOpcode0x9B;
	this->extendedOpcodes[0x9C] = &CPU::extendedOpcode0x9C;
	this->extendedOpcodes[0x9D] = &CPU::extendedOpcode0x9D;
	this->extendedOpcodes[0x9E] = &CPU::extendedOpcode0x9E;
	this->extendedOpcodes[0x9F] = &CPU::extendedOpcode0x9F;
	this->extendedOpcodes[0xA0] = &CPU::extendedOpcode0xA0;
	this->extendedOpcodes[0xA1] = &CPU::extendedOpcode0xA1;
	this->extendedOpcodes[0xA2] = &CPU::extendedOpcode0xA2;
	this->extendedOpcodes[0xA3] = &CPU::extendedOpcode0xA3;
	this->extendedOpcodes[0xA4] = &CPU::extendedOpcode0xA4;
	this->extendedOpcodes[0xA5] = &CPU::extendedOpcode0xA5;
	this->extendedOpcodes[0xA6] = &CPU::extendedOpcode0xA6;
	this->extendedOpcodes[0xA7] = &CPU::extendedOpcode0xA7;
	this->extendedOpcodes[0xA8] = &CPU::extendedOpcode0xA8;
	this->extendedOpcodes[0xA9] = &CPU::extendedOpcode0xA9;
	this->extendedOpcodes[0xAA] = &CPU::extendedOpcode0xAA;
	this->extendedOpcodes[0xAB] = &CPU::extendedOpcode0xAB;
	this->extendedOpcodes[0xAC] = &CPU::extendedOpcode0xAC;
	this->extendedOpcodes[0xAD] = &CPU::extendedOpcode0xAD;
	this->extendedOpcodes[0xAE] = &CPU::extendedOpcode0xAE;
	this->extendedOpcodes[0xAF] = &CPU::extendedOpcode0xAF;
	this->extendedOpcodes[0xB0] = &CPU::extendedOpcode0xB0;
	this->extendedOpcodes[0xB1] = &CPU::extendedOpcode0xB1;
	this->extendedOpcodes[0xB2] = &CPU::extendedOpcode0xB2;
	this->extendedOpcodes[0xB3] = &CPU::extendedOpcode0xB3;
	this->extendedOpcodes[0xB4] = &CPU::extendedOpcode0xB4;
	this->extendedOpcodes[0xB5] = &CPU::extendedOpcode0xB5;
	this->extendedOpcodes[0xB6] = &CPU::extendedOpcode0xB6;
	this->extendedOpcodes[0xB7] = &CPU::extendedOpcode0xB7;
	this->extendedOpcodes[0xB8] = &CPU::extendedOpcode0xB8;
	this->extendedOpcodes[0xB9] = &CPU::extendedOpcode0xB9;
	this->extendedOpcodes[0xBA] = &CPU::extendedOpcode0xBA;
	this->extendedOpcodes[0xBB] = &CPU::extendedOpcode0xBB;
	this->extendedOpcodes[0xBC] = &CPU::extendedOpcode0xBC;
	this->extendedOpcodes[0xBD] = &CPU::extendedOpcode0xBD;
	this->extendedOpcodes[0xBE] = &CPU::extendedOpcode0xBE;
	this->extendedOpcodes[0xBF] = &CPU::extendedOpcode0xBF;
	this->extendedOpcodes[0xC0] = &CPU::extendedOpcode0xC0;
	this->extendedOpcodes[0xC1] = &CPU::extendedOpcode0xC1;
	this->extendedOpcodes[0xC2] = &CPU::extendedOpcode0xC2;
	this->extendedOpcodes[0xC3] = &CPU::extendedOpcode0xC3;
	this->extendedOpcodes[0xC4] = &CPU::extendedOpcode0xC4;
	this->extendedOpcodes[0xC5] = &CPU::extendedOpcode0xC5;
	this->extendedOpcodes[0xC6] = &CPU::extendedOpcode0xC6;
	this->extendedOpcodes[0xC7] = &CPU::extendedOpcode0xC7;
	this->extendedOpcodes[0xC8] = &CPU::extendedOpcode0xC8;
	this->extendedOpcodes[0xC9] = &CPU::extendedOpcode0xC9;
	this->extendedOpcodes[0xCA] = &CPU::extendedOpcode0xCA;
	this->extendedOpcodes[0xCB] = &CPU::extendedOpcode0xCB;
	this->extendedOpcodes[0xCC] = &CPU::extendedOpcode0xCC;
	this->extendedOpcodes[0xCD] = &CPU::extendedOpcode0xCD;
	this->extendedOpcodes[0xCE] = &CPU::extendedOpcode0xCE;
	this->extendedOpcodes[0xCF] = &CPU::extendedOpcode0xCF;
	this->extendedOpcodes[0xD0] = &CPU::extendedOpcode0xD0;
	this->extendedOpcodes[0xD1] = &CPU::extendedOpcode0xD1;
	this->extendedOpcodes[0xD2] = &CPU::extendedOpcode0xD2;
	this->extendedOpcodes[0xD3] = &CPU::extendedOpcode0xD3;
	this->extendedOpcodes[0xD4] = &CPU::extendedOpcode0xD4;
	this->extendedOpcodes[0xD5] = &CPU::extendedOpcode0xD5;
	this->extendedOpcodes[0xD6] = &CPU::extendedOpcode0xD6;
	this->extendedOpcodes[0xD7] = &CPU::extendedOpcode0xD7;
	this->extendedOpcodes[0xD8] = &CPU::extendedOpcode0xD8;
	this->extendedOpcodes[0xD9] = &CPU::extendedOpcode0xD9;
	this->extendedOpcodes[0xDA] = &CPU::extendedOpcode0xDA;
	this->extendedOpcodes[0xDB] = &CPU::extendedOpcode0xDB;
	this->extendedOpcodes[0xDC] = &CPU::extendedOpcode0xDC;
	this->extendedOpcodes[0xDD] = &CPU::extendedOpcode0xDD;
	this->extendedOpcodes[0xDE] = &CPU::extendedOpcode0xDE;
	this->extendedOpcodes[0xDF] = &CPU::extendedOpcode0xDF;
	this->extendedOpcodes[0xE0] = &CPU::extendedOpcode0xE0;
	this->extendedOpcodes[0xE1] = &CPU::extendedOpcode0xE1;
	this->extendedOpcodes[0xE2] = &CPU::extendedOpcode0xE2;
	this->extendedOpcodes[0xE3] = &CPU::extendedOpcode0xE3;
	this->extendedOpcodes[0xE4] = &CPU::extendedOpcode0xE4;
	this->extendedOpcodes[0xE5] = &CPU::extendedOpcode0xE5;
	this->extendedOpcodes[0xE6] = &CPU::extendedOpcode0xE6;
	this->extendedOpcodes[0xE7] = &CPU::extendedOpcode0xE7;
	this->extendedOpcodes[0xE8] = &CPU::extendedOpcode0xE8;
	this->extendedOpcodes[0xE9] = &CPU::extendedOpcode0xE9;
	this->extendedOpcodes[0xEA] = &CPU::extendedOpcode0xEA;
	this->extendedOpcodes[0xEB] = &CPU::extendedOpcode0xEB;
	this->extendedOpcodes[0xEC] = &CPU::extendedOpcode0xEC;
	this->extendedOpcodes[0xED] = &CPU::extendedOpcode0xED;
	this->extendedOpcodes[0xEE] = &CPU::extendedOpcode0xEE;
	this->extendedOpcodes[0xEF] = &CPU::extendedOpcode0xEF;
	this->extendedOpcodes[0xF0] = &CPU::extendedOpcode0xF0;
	this->extendedOpcodes[0xF1] = &CPU::extendedOpcode0xF1;
	this->extendedOpcodes[0xF2] = &CPU::extendedOpcode0xF2;
	this->extendedOpcodes[0xF3] = &CPU::extendedOpcode0xF3;
	this->extendedOpcodes[0xF4] = &CPU::extendedOpcode0xF4;
	this->extendedOpcodes[0xF5] = &CPU::extendedOpcode0xF5;
	this->extendedOpcodes[0xF6] = &CPU::extendedOpcode0xF6;
	this->extendedOpcodes[0xF7] = &CPU::extendedOpcode0xF7;
	this->extendedOpcodes[0xF8] = &CPU::extendedOpcode0xF8;
	this->extendedOpcodes[0xF9] = &CPU::extendedOpcode0xF9;
	this->extendedOpcodes[0xFA] = &CPU::extendedOpcode0xFA;
	this->extendedOpcodes[0xFB] = &CPU::extendedOpcode0xFB;
	this->extendedOpcodes[0xFC] = &CPU::extendedOpcode0xFC;
	this->extendedOpcodes[0xFD] = &CPU::extendedOpcode0xFD;
	this->extendedOpcodes[0xFE] = &CPU::extendedOpcode0xFE;
	this->extendedOpcodes[0xFF] = &CPU::extendedOpcode0xFF;
}

void CPU::Opcode0x00() {
	NOP();
}

void CPU::Opcode0x01() {
	BC.setRegister(mmu->formWord(mmu->get(pc + 2), mmu->get(pc + 1)));
	pc += 2;
}

void CPU::Opcode0x02() {
	LD(BC.getRegister(), A);
}

void CPU::Opcode0x03() {
	INC(BC);
}

void CPU::Opcode0x04() {
	INC(B);
}

void CPU::Opcode0x05() {
	DEC(B);
}

void CPU::Opcode0x06() {
	LD(B, mmu->get(pc + 1));
	pc++;
}

void CPU::Opcode0x07() {
	RLC(&A);
}
void CPU::Opcode0x08() {
	LD(mmu->get(mmu->formWord(mmu->get(pc), mmu->get(pc + 1))), sp);
	pc += 2;
}

void CPU::Opcode0x09() {
	ADD_HL(BC.getRegister());
}

void CPU::Opcode0x0A() {
	LD(A, mmu->get(BC.getRegister()));
}

void CPU::Opcode0x0B() {
	DEC(BC);
}

void CPU::Opcode0x0C() {
	INC(C);
}

void CPU::Opcode0x0D() {
	DEC(C);
}

void CPU::Opcode0x0E() {
	LD(C, mmu->get(pc + 1));
	pc++;
}

void CPU::Opcode0x0F() {
	RRCA();
}

void CPU::Opcode0x10() {
	STOP();
}

void CPU::Opcode0x11() {
	DE.setRegister(mmu->formWord(mmu->get(pc + 2), mmu->get(pc + 1)));
	pc += 2;
}

void CPU::Opcode0x12() {
	LD(DE.getRegister(), A);
}

void CPU::Opcode0x13() {
	INC(DE);
}

void CPU::Opcode0x14() {
	INC(D);
}

void CPU::Opcode0x15() {
	DEC(D);
}

void CPU::Opcode0x16() {
	LD(D, mmu->get(pc + 1));
	pc++;
}

void CPU::Opcode0x17() {
	RLA();
}

void CPU::Opcode0x18() {
	JR();
}

void CPU::Opcode0x19() {
	ADD_HL(DE.getRegister());
}

void CPU::Opcode0x1A() {
	LD(A, DE.getRegister());
}

void CPU::Opcode0x1B() {
	DEC(DE);
}

void CPU::Opcode0x1C() {
	INC(E);
}

void CPU::Opcode0x1D() {
	DEC(E);
}

void CPU::Opcode0x1E() {
	LD(E, mmu->get(pc + 1));
	pc++;
}

void CPU::Opcode0x1F() {
	RR(&A);
}

void CPU::Opcode0x20() {
	if (!getFlag(FLAG_Z)) {
		JR();
	}
	else {
		pc++;
	}
}

void CPU::Opcode0x21() {
	HL.setRegister(mmu->formWord(mmu->get(pc + 2), mmu->get(pc + 1)));
	pc += 2;
}

void CPU::Opcode0x22() {
	LD(HL.getRegister(), A);
	HL.setRegister(HL.getRegister() + 1);
}

void CPU::Opcode0x23() {
	INC(HL);
}

void CPU::Opcode0x24() {
	INC(H);
}

void CPU::Opcode0x25() {
	DEC(H);
}

void CPU::Opcode0x26() {
	LD(H, mmu->get(pc + 1));
	pc++;
}

void CPU::Opcode0x27() {
	DAA();
}

void CPU::Opcode0x28() {
	if (getFlag(FLAG_Z)) {
		JR();
	}
	else {
		pc++;
	}
}

void CPU::Opcode0x29() {
	ADD_HL(HL.getRegister());
}

void CPU::Opcode0x2A() {
	LD(A, HL.getRegister());
	HL.setRegister(HL.getRegister() + 1);
}

void CPU::Opcode0x2B() {
	DEC(HL);
}

void CPU::Opcode0x2C() {
	INC(L);
}

void CPU::Opcode0x2D() {
	DEC(L);
}

void CPU::Opcode0x2E() {
	LD(L, mmu->get(pc + 1));
	pc++;
}

void CPU::Opcode0x2F() {
	CPL();
}

void CPU::Opcode0x30() {
	if (!getFlag(FLAG_C)) {
		JR();
	}
	else {
		pc++;
	}
}

void CPU::Opcode0x31() {
	this->sp = mmu->formWord(mmu->get(pc + 2), mmu->get(pc + 1));
	pc += 2;
}

void CPU::Opcode0x32() {
	LD(HL.getRegister(), A);
	HL.setRegister(HL.getRegister() - 1);
}

void CPU::Opcode0x33() {
	INC_SP();
}

void CPU::Opcode0x34() {
	uint8_t reg = mmu->get(HL.getRegister());
	INC(reg);
}

void CPU::Opcode0x35() {
	uint8_t reg = mmu->get(HL.getRegister());
	DEC(reg);
}

void CPU::Opcode0x36() {
	LD(HL.getRegister(), mmu->get(pc));
	pc++;
}

void CPU::Opcode0x37() {
	SCF();
}

void CPU::Opcode0x38() {
	if (getFlag(FLAG_C)) {
		JR();
	}
	else {
		pc++;
	}
}

void CPU::Opcode0x39() {
	ADD_HL(sp);
}

void CPU::Opcode0x3A() {
	LD(A, HL.getRegister());
	HL.setRegister(HL.getRegister() - 1);
}

void CPU::Opcode0x3B() {
	DEC_SP();
}

void CPU::Opcode0x3C() {
	INC(A);
}

void CPU::Opcode0x3D() {
	DEC(A);
}

void CPU::Opcode0x3E() {
	LD(A, mmu->get(pc + 1));
	pc++;
}

void CPU::Opcode0x3F() {
	CCF();
}

void CPU::Opcode0x40() {
	LD(B, B);
}

void CPU::Opcode0x41() {
	LD(B, C);
}

void CPU::Opcode0x42() {
	LD(B, D);
}

void CPU::Opcode0x43() {
	LD(B, E);
}

void CPU::Opcode0x44() {
	LD(B, H);
}

void CPU::Opcode0x45() {
	LD(B, L);
}

void CPU::Opcode0x46() {
	LD(B, mmu->get(HL.getRegister()));
}

void CPU::Opcode0x47() {
	LD(B, A);
}

void CPU::Opcode0x48() {
	LD(C, B);
}

void CPU::Opcode0x49() {
	LD(C, C);
}

void CPU::Opcode0x4A() {
	LD(C, D);
}

void CPU::Opcode0x4B() {
	LD(C, E);
}

void CPU::Opcode0x4C() {
	LD(C, H);
}

void CPU::Opcode0x4D() {
	LD(C, L);
}

void CPU::Opcode0x4E() {
	LD(E, HL.getRegister());
}

void CPU::Opcode0x4F() {
	LD(C, A);
}

void CPU::Opcode0x50() {
	LD(D, B);
}

void CPU::Opcode0x51() {
	LD(D, C);
}

void CPU::Opcode0x52() {
	LD(D, D);
}

void CPU::Opcode0x53() {
	LD(D, E);
}

void CPU::Opcode0x54() {
	LD(D, H);
}

void CPU::Opcode0x55() {
	LD(D, L);
}

void CPU::Opcode0x56() {
	LD(D, HL.getRegister());
}

void CPU::Opcode0x57() {
	LD(D, A);
}

void CPU::Opcode0x58() {
	LD(E, B);
}

void CPU::Opcode0x59() {
	LD(E, C);
}

void CPU::Opcode0x5A() {
	LD(E, D);
}

void CPU::Opcode0x5B() {
	LD(E, E);
}

void CPU::Opcode0x5C() {
	LD(E, H);
}

void CPU::Opcode0x5D() {
	LD(E, L);
}

void CPU::Opcode0x5E() {
	LD(E, HL.getRegister());
}

void CPU::Opcode0x5F() {
	LD(E, A);
}

void CPU::Opcode0x60() {
	LD(H, B);
}

void CPU::Opcode0x61() {
	LD(H, C);
}

void CPU::Opcode0x62() {
	LD(H, D);
}

void CPU::Opcode0x63() {
	LD(H, E);
}

void CPU::Opcode0x64() {
	LD(H, H);
}

void CPU::Opcode0x65() {
	LD(H, L);
}

void CPU::Opcode0x66() {
	LD(H, HL.getRegister());
}

void CPU::Opcode0x67() {
	LD(H, A);
}

void CPU::Opcode0x68() {
	LD(L, B);
}

void CPU::Opcode0x69() {
	LD(L, C);
}

void CPU::Opcode0x6A() {
	LD(L, D);
}

void CPU::Opcode0x6B() {
	LD(L, E);
}

void CPU::Opcode0x6C() {
	LD(L, H);
}

void CPU::Opcode0x6D() {
	LD(L, L);
}

void CPU::Opcode0x6E() {
	LD(L, HL.getRegister());
}

void CPU::Opcode0x6F() {
	LD(L, A);
}

void CPU::Opcode0x70() {
	LD(HL.getRegister(), B);
}

void CPU::Opcode0x71() {
	LD(HL.getRegister(), C);
}

void CPU::Opcode0x72() {
	LD(HL.getRegister(), D);
}

void CPU::Opcode0x73() {
	LD(HL.getRegister(), E);
}

void CPU::Opcode0x74() {
	LD(HL.getRegister(), H);
}

void CPU::Opcode0x75() {
	LD(HL.getRegister(), L);
}

void CPU::Opcode0x76() {
	HALT();
}

void CPU::Opcode0x77() {
	LD(HL.getRegister(), A);
}

void CPU::Opcode0x78() {
	LD(A, B);
}

void CPU::Opcode0x79() {
	LD(A, C);
}

void CPU::Opcode0x7A() {
	LD(A, D);
}

void CPU::Opcode0x7B() {
	LD(A, E);
}

void CPU::Opcode0x7C() {
	LD(A, H);
}

void CPU::Opcode0x7D() {
	LD(A, L);
}

void CPU::Opcode0x7E() {
	LD(A, HL.getRegister());
}

void CPU::Opcode0x7F() {
	LD(A, A);
}

void CPU::Opcode0x80() {
	ADD(B);
}

void CPU::Opcode0x81() {
	ADD(C);
}

void CPU::Opcode0x82() {
	ADD(D);
}

void CPU::Opcode0x83() {
	ADD(E);
}

void CPU::Opcode0x84() {
	ADD(H);
}

void CPU::Opcode0x85() {
	ADD(L);
}

void CPU::Opcode0x86() {
	ADD(mmu->get(pc));
	pc++;
}

void CPU::Opcode0x87() {
	ADD(A);
}

void CPU::Opcode0x88() {
	ADC(B);
}

void CPU::Opcode0x89() {
	ADC(C);
}

void CPU::Opcode0x8A() {
	ADC(D);
}

void CPU::Opcode0x8B() {
	ADC(E);
}

void CPU::Opcode0x8C() {
	ADC(H);
}

void CPU::Opcode0x8D() {
	ADC(L);
}

void CPU::Opcode0x8E() {
	ADC(mmu->get(HL.getRegister()));
}

void CPU::Opcode0x8F() {
	ADC(A);
}

void CPU::Opcode0x90() {
	SUB(B);
}

void CPU::Opcode0x91() {
	SUB(C);
}

void CPU::Opcode0x92() {
	SUB(D);
}

void CPU::Opcode0x93() {
	SUB(E);
}

void CPU::Opcode0x94() {
	SUB(H);
}

void CPU::Opcode0x95() {
	SUB(L);
}

void CPU::Opcode0x96() {
	SUB(mmu->get(HL.getRegister()));
}

void CPU::Opcode0x97() {
	SUB(A);
}

void CPU::Opcode0x98() {
	SBC(B);
}

void CPU::Opcode0x99() {
	SBC(C);
}

void CPU::Opcode0x9A() {
	SBC(D);
}

void CPU::Opcode0x9B() {
	SBC(E);
}

void CPU::Opcode0x9C() {
	SBC(H);
}

void CPU::Opcode0x9D() {
	SBC(L);
}

void CPU::Opcode0x9E() {
	SBC(mmu->get(HL.getRegister()));
}

void CPU::Opcode0x9F() {
	SBC(A);
}

void CPU::Opcode0xA0() {
	AND(B);
}

void CPU::Opcode0xA1() {
	AND(C);
}

void CPU::Opcode0xA2() {
	AND(D);
}

void CPU::Opcode0xA3() {
	AND(E);
}

void CPU::Opcode0xA4() {
	AND(H);
}

void CPU::Opcode0xA5() {
	AND(L);
}

void CPU::Opcode0xA6() {
	AND(mmu->get(HL.getRegister()));
}

void CPU::Opcode0xA7() {
	AND(A);
}

void CPU::Opcode0xA8() {
	XOR(B);
}

void CPU::Opcode0xA9() {
	XOR(C);
}

void CPU::Opcode0xAA() {
	XOR(D);
}

void CPU::Opcode0xAB() {
	XOR(E);
}

void CPU::Opcode0xAC() {
	XOR(H);
}

void CPU::Opcode0xAD() {
	XOR(L);
}

void CPU::Opcode0xAE() {
	XOR(mmu->get(HL.getRegister()));
}

void CPU::Opcode0xAF() {
	XOR(A);
}

void CPU::Opcode0xB0() {
	OR(B);
}

void CPU::Opcode0xB1() {
	OR(C);
}

void CPU::Opcode0xB2() {
	OR(D);
}

void CPU::Opcode0xB3() {
	OR(E);
}

void CPU::Opcode0xB4() {
	OR(H);
}

void CPU::Opcode0xB5() {
	OR(L);
}

void CPU::Opcode0xB6() {
	OR(mmu->get(HL.getRegister()));
}

void CPU::Opcode0xB7() {
	OR(A);
}

void CPU::Opcode0xB8() {
	CP(B);
}

void CPU::Opcode0xB9() {
	CP(C);
}

void CPU::Opcode0xBA() {
	CP(D);
}

void CPU::Opcode0xBB() {
	CP(E);
}

void CPU::Opcode0xBC() {
	CP(H);
}

void CPU::Opcode0xBD() {
	CP(L);
}

void CPU::Opcode0xBE() {
	CP(mmu->get(HL.getRegister()));
}

void CPU::Opcode0xBF() {
	CP(A);
}

void CPU::Opcode0xC0() {
	if (!getFlag(FLAG_Z)) {
		RET();
	}
	else {
		pc++;
	}
}

void CPU::Opcode0xC1() {
	POPSTACK(BC);
}

void CPU::Opcode0xC2() {
	if (!getFlag(FLAG_Z)) {
		JP();
	}
	else {
		pc += 2;
	}
}

void CPU::Opcode0xC3() {
	JP();
}

void CPU::Opcode0xC4() {
	if (!getFlag(FLAG_Z)) {
		CALL();
	}
	else {
		pc += 2;
	}
}

void CPU::Opcode0xC5() {
	PUSHSTACK16(BC.getRegister());
}

void CPU::Opcode0xC6() {
	ADD(mmu->get(pc));
	pc++;
}

void CPU::Opcode0xC7() {
	RST(0);
}

void CPU::Opcode0xC8() {
	if (getFlag(FLAG_Z)) {
		RET();
	}
}

void CPU::Opcode0xC9() {
	RET();
}

void CPU::Opcode0xCA() {
	if (getFlag(FLAG_Z)) {
		JP();
	}
	else {
		pc += 2;
	}
}

void CPU::Opcode0xCB() {
	extended = true;
}

void CPU::Opcode0xCC() {
	if (getFlag(FLAG_Z)) {
		CALL();
	}
	else {
		pc += 2;
	}
}

void CPU::Opcode0xCD() {
	CALL();
}

void CPU::Opcode0xCE() {
	ADC(mmu->get(pc));
	pc++;
}

void CPU::Opcode0xCF() {
	RST(1);
}

void CPU::Opcode0xD0() {
	if (!getFlag(FLAG_C)) {
		RET();
	}
}

void CPU::Opcode0xD1() {
	POPSTACK(DE);
}

void CPU::Opcode0xD2() {
	if (!getFlag(FLAG_C)) {
		JP();
	}
	else {
		pc += 2;
	}
}

void CPU::Opcode0xD3() {

}

void CPU::Opcode0xD4() {
	if (!getFlag(FLAG_C)) {
		CALL();
	}
	else {
		pc += 2;
	}
}

void CPU::Opcode0xD5() {
	PUSHSTACK16(DE.getRegister());
}

void CPU::Opcode0xD6() {
	SUB(mmu->get(pc));
	pc++;
}

void CPU::Opcode0xD7() {
	RST(2);
}

void CPU::Opcode0xD8() {
	if (getFlag(FLAG_C)) {
		RET();
	}
}

void CPU::Opcode0xD9() {
	RET();
	ime = true;
}

void CPU::Opcode0xDA() {
	if (getFlag(FLAG_C)) {
		JP();
	}
	else {
		pc += 2;
	}
}

void CPU::Opcode0xDB() {

}

void CPU::Opcode0xDC() {
	if (getFlag(FLAG_C)) {
		CALL();
	}
	else {
		pc += 2;
	}
}

void CPU::Opcode0xDD() {

}

void CPU::Opcode0xDE() {
	SBC(mmu->get(pc));
	pc++;
}

void CPU::Opcode0xDF() {
	RST(3);
}

void CPU::Opcode0xE0() {
	LD(static_cast<uint16_t>(0xFF00 + mmu->get(pc)), A);
	pc++;
}

void CPU::Opcode0xE1() {
	POPSTACK(HL);
}

void CPU::Opcode0xE2() {
	LD(static_cast<uint16_t>(0xFF00 + C), A);
}

void CPU::Opcode0xE3() {

}

void CPU::Opcode0xE4() {

}

void CPU::Opcode0xE5() {
	PUSHSTACK16(HL.getRegister());
}

void CPU::Opcode0xE6() {
	AND(mmu->get(pc));
	pc++;
}

void CPU::Opcode0xE7() {
	RST(4);
}

void CPU::Opcode0xE8() {
	ADD_SP();
}

void CPU::Opcode0xE9() {
	JP_HL();
}

void CPU::Opcode0xEA() {
	mmu->set(mmu->formWord(mmu->get(pc + 2), mmu->get(pc + 1)), A);
	pc += 2;
}

void CPU::Opcode0xEB() {

}

void CPU::Opcode0xEC() {

}

void CPU::Opcode0xED() {

}

void CPU::Opcode0xEE() {
	XOR(mmu->get(pc));
	pc++;
}

void CPU::Opcode0xEF() {
	RST(7);
}

void CPU::Opcode0xF0() {
	/* 
		This value needs to be hardcoded until LCD support is added.

		See 0xFE.
	*/
	A = 0x90;
	//LD(A, static_cast<uint16_t>(0xFF00 + (mmu->get(pc + 1))));
	pc++;
}

void CPU::Opcode0xF1() {
	POPSTACK(AF);
}

void CPU::Opcode0xF2() {
	LD(A, static_cast<uint16_t>(0xFF00 + C));
}

void CPU::Opcode0xF3() {
	DI();
}

void CPU::Opcode0xF4() {

}

void CPU::Opcode0xF5() {
	PUSHSTACK16(AF.getRegister());
}

void CPU::Opcode0xF6() {
	OR(mmu->get(pc));
	pc++;
}

void CPU::Opcode0xF7() {
	RST(6);
}

void CPU::Opcode0xF8() {
	int16_t d = sp + mmu->get(pc);
	HL.setRegister(sp + mmu->get(pc));
	clearFlag(FLAG_Z);
	clearFlag(FLAG_N);
	if (d > 0x0FFF) {
		setFlag(FLAG_H);
		setFlag(FLAG_C);
	}
	else {
		clearFlag(FLAG_H);
		clearFlag(FLAG_C);
	}
	pc++;
}

void CPU::Opcode0xF9() {
	sp = HL.getRegister();
}

void CPU::Opcode0xFA() {
	LD(A, mmu->get(mmu->formWord(mmu->get(pc + 2), mmu->get(pc + 1))));
	pc += 2;
}

void CPU::Opcode0xFB() {
	EI();
}

void CPU::Opcode0xFC() {
	
}

void CPU::Opcode0xFD() {

}

void CPU::Opcode0xFE() {
	/* 
		This opcode is used in order to compare A to LY, which is the current 
		scanline the renderer is on. It automatically increments whenever the 
		scanline is finished. Since there is no LCD support however, LY will 
		be the wrong value and CP will set the wrong flags in the evaluation, 
		so opcode 0xF0 executed prior will need 0x90 hard coded until LCD is 
		supported.
	*/
	CP(mmu->get(pc + 1));
	pc++;
}

void CPU::Opcode0xFF() {
	RST(7);
}

void CPU::extendedOpcode0x00(){}
void CPU::extendedOpcode0x01(){}
void CPU::extendedOpcode0x02(){}
void CPU::extendedOpcode0x03(){}
void CPU::extendedOpcode0x04(){}
void CPU::extendedOpcode0x05(){}
void CPU::extendedOpcode0x06(){}
void CPU::extendedOpcode0x07(){}
void CPU::extendedOpcode0x08(){}
void CPU::extendedOpcode0x09(){}
void CPU::extendedOpcode0x0A(){}
void CPU::extendedOpcode0x0B(){}
void CPU::extendedOpcode0x0C(){}
void CPU::extendedOpcode0x0D(){}
void CPU::extendedOpcode0x0E(){}
void CPU::extendedOpcode0x0F(){}
void CPU::extendedOpcode0x10(){}
void CPU::extendedOpcode0x11(){}
void CPU::extendedOpcode0x12(){}
void CPU::extendedOpcode0x13(){}
void CPU::extendedOpcode0x14(){}
void CPU::extendedOpcode0x15(){}
void CPU::extendedOpcode0x16(){}
void CPU::extendedOpcode0x17(){}
void CPU::extendedOpcode0x18(){}
void CPU::extendedOpcode0x19(){}
void CPU::extendedOpcode0x1A(){}
void CPU::extendedOpcode0x1B(){}
void CPU::extendedOpcode0x1C(){}
void CPU::extendedOpcode0x1D(){}
void CPU::extendedOpcode0x1E(){}
void CPU::extendedOpcode0x1F(){}
void CPU::extendedOpcode0x20(){}
void CPU::extendedOpcode0x21(){}
void CPU::extendedOpcode0x22(){}
void CPU::extendedOpcode0x23(){}
void CPU::extendedOpcode0x24(){}
void CPU::extendedOpcode0x25(){}
void CPU::extendedOpcode0x26(){}
void CPU::extendedOpcode0x27(){}
void CPU::extendedOpcode0x28(){}
void CPU::extendedOpcode0x29(){}
void CPU::extendedOpcode0x2A(){}
void CPU::extendedOpcode0x2B(){}
void CPU::extendedOpcode0x2C(){}
void CPU::extendedOpcode0x2D(){}
void CPU::extendedOpcode0x2E(){}
void CPU::extendedOpcode0x2F(){}
void CPU::extendedOpcode0x30(){}
void CPU::extendedOpcode0x31(){}
void CPU::extendedOpcode0x32(){}
void CPU::extendedOpcode0x33(){}
void CPU::extendedOpcode0x34(){}
void CPU::extendedOpcode0x35(){}
void CPU::extendedOpcode0x36(){}
void CPU::extendedOpcode0x37(){}
void CPU::extendedOpcode0x38(){}
void CPU::extendedOpcode0x39(){}
void CPU::extendedOpcode0x3A(){}
void CPU::extendedOpcode0x3B(){}
void CPU::extendedOpcode0x3C(){}
void CPU::extendedOpcode0x3D(){}
void CPU::extendedOpcode0x3E(){}
void CPU::extendedOpcode0x3F(){}
void CPU::extendedOpcode0x40(){}
void CPU::extendedOpcode0x41(){}
void CPU::extendedOpcode0x42(){}
void CPU::extendedOpcode0x43(){}
void CPU::extendedOpcode0x44(){}
void CPU::extendedOpcode0x45(){}
void CPU::extendedOpcode0x46(){}
void CPU::extendedOpcode0x47(){}
void CPU::extendedOpcode0x48(){}
void CPU::extendedOpcode0x49(){}
void CPU::extendedOpcode0x4A(){}
void CPU::extendedOpcode0x4B(){}
void CPU::extendedOpcode0x4C(){}
void CPU::extendedOpcode0x4D(){}
void CPU::extendedOpcode0x4E(){}
void CPU::extendedOpcode0x4F(){}
void CPU::extendedOpcode0x50(){}
void CPU::extendedOpcode0x51(){}
void CPU::extendedOpcode0x52(){}
void CPU::extendedOpcode0x53(){}
void CPU::extendedOpcode0x54(){}
void CPU::extendedOpcode0x55(){}
void CPU::extendedOpcode0x56(){}
void CPU::extendedOpcode0x57(){}
void CPU::extendedOpcode0x58(){}
void CPU::extendedOpcode0x59(){}
void CPU::extendedOpcode0x5A(){}
void CPU::extendedOpcode0x5B(){}
void CPU::extendedOpcode0x5C(){}
void CPU::extendedOpcode0x5D(){}
void CPU::extendedOpcode0x5E(){}
void CPU::extendedOpcode0x5F(){}
void CPU::extendedOpcode0x60(){}
void CPU::extendedOpcode0x61(){}
void CPU::extendedOpcode0x62(){}
void CPU::extendedOpcode0x63(){}
void CPU::extendedOpcode0x64(){}
void CPU::extendedOpcode0x65(){}
void CPU::extendedOpcode0x66(){}
void CPU::extendedOpcode0x67(){}
void CPU::extendedOpcode0x68(){}
void CPU::extendedOpcode0x69(){}
void CPU::extendedOpcode0x6A(){}
void CPU::extendedOpcode0x6B(){}
void CPU::extendedOpcode0x6C(){}
void CPU::extendedOpcode0x6D(){}
void CPU::extendedOpcode0x6E(){}
void CPU::extendedOpcode0x6F(){}
void CPU::extendedOpcode0x70(){}
void CPU::extendedOpcode0x71(){}
void CPU::extendedOpcode0x72(){}
void CPU::extendedOpcode0x73(){}
void CPU::extendedOpcode0x74(){}
void CPU::extendedOpcode0x75(){}
void CPU::extendedOpcode0x76(){}
void CPU::extendedOpcode0x77(){}
void CPU::extendedOpcode0x78(){}
void CPU::extendedOpcode0x79(){}
void CPU::extendedOpcode0x7A(){}
void CPU::extendedOpcode0x7B(){}
void CPU::extendedOpcode0x7C(){}
void CPU::extendedOpcode0x7D(){}
void CPU::extendedOpcode0x7E(){}
void CPU::extendedOpcode0x7F(){}
void CPU::extendedOpcode0x80(){}
void CPU::extendedOpcode0x81(){}
void CPU::extendedOpcode0x82(){}
void CPU::extendedOpcode0x83(){}
void CPU::extendedOpcode0x84(){}
void CPU::extendedOpcode0x85(){}
void CPU::extendedOpcode0x86(){}
void CPU::extendedOpcode0x87(){}
void CPU::extendedOpcode0x88(){}
void CPU::extendedOpcode0x89(){}
void CPU::extendedOpcode0x8A(){}
void CPU::extendedOpcode0x8B(){}
void CPU::extendedOpcode0x8C(){}
void CPU::extendedOpcode0x8D(){}
void CPU::extendedOpcode0x8E(){}
void CPU::extendedOpcode0x8F(){}
void CPU::extendedOpcode0x90(){}
void CPU::extendedOpcode0x91(){}
void CPU::extendedOpcode0x92(){}
void CPU::extendedOpcode0x93(){}
void CPU::extendedOpcode0x94(){}
void CPU::extendedOpcode0x95(){}
void CPU::extendedOpcode0x96(){}
void CPU::extendedOpcode0x97(){}
void CPU::extendedOpcode0x98(){}
void CPU::extendedOpcode0x99(){}
void CPU::extendedOpcode0x9A(){}
void CPU::extendedOpcode0x9B(){}
void CPU::extendedOpcode0x9C(){}
void CPU::extendedOpcode0x9D(){}
void CPU::extendedOpcode0x9E(){}
void CPU::extendedOpcode0x9F(){}
void CPU::extendedOpcode0xA0(){}
void CPU::extendedOpcode0xA1(){}
void CPU::extendedOpcode0xA2(){}
void CPU::extendedOpcode0xA3(){}
void CPU::extendedOpcode0xA4(){}
void CPU::extendedOpcode0xA5(){}
void CPU::extendedOpcode0xA6(){}
void CPU::extendedOpcode0xA7(){}
void CPU::extendedOpcode0xA8(){}
void CPU::extendedOpcode0xA9(){}
void CPU::extendedOpcode0xAA(){}
void CPU::extendedOpcode0xAB(){}
void CPU::extendedOpcode0xAC(){}
void CPU::extendedOpcode0xAD(){}
void CPU::extendedOpcode0xAE(){}
void CPU::extendedOpcode0xAF(){}
void CPU::extendedOpcode0xB0(){}
void CPU::extendedOpcode0xB1(){}
void CPU::extendedOpcode0xB2(){}
void CPU::extendedOpcode0xB3(){}
void CPU::extendedOpcode0xB4(){}
void CPU::extendedOpcode0xB5(){}
void CPU::extendedOpcode0xB6(){}
void CPU::extendedOpcode0xB7(){}
void CPU::extendedOpcode0xB8(){}
void CPU::extendedOpcode0xB9(){}
void CPU::extendedOpcode0xBA(){}
void CPU::extendedOpcode0xBB(){}
void CPU::extendedOpcode0xBC(){}
void CPU::extendedOpcode0xBD(){}
void CPU::extendedOpcode0xBE(){}
void CPU::extendedOpcode0xBF(){}
void CPU::extendedOpcode0xC0(){}
void CPU::extendedOpcode0xC1(){}
void CPU::extendedOpcode0xC2(){}
void CPU::extendedOpcode0xC3(){}
void CPU::extendedOpcode0xC4(){}
void CPU::extendedOpcode0xC5(){}
void CPU::extendedOpcode0xC6(){}
void CPU::extendedOpcode0xC7(){}
void CPU::extendedOpcode0xC8(){}
void CPU::extendedOpcode0xC9(){}
void CPU::extendedOpcode0xCA(){}
void CPU::extendedOpcode0xCB(){}
void CPU::extendedOpcode0xCC(){}
void CPU::extendedOpcode0xCD(){}
void CPU::extendedOpcode0xCE(){}
void CPU::extendedOpcode0xCF(){}
void CPU::extendedOpcode0xD0(){}
void CPU::extendedOpcode0xD1(){}
void CPU::extendedOpcode0xD2(){}
void CPU::extendedOpcode0xD3(){}
void CPU::extendedOpcode0xD4(){}
void CPU::extendedOpcode0xD5(){}
void CPU::extendedOpcode0xD6(){}
void CPU::extendedOpcode0xD7(){}
void CPU::extendedOpcode0xD8(){}
void CPU::extendedOpcode0xD9(){}
void CPU::extendedOpcode0xDA(){}
void CPU::extendedOpcode0xDB(){}
void CPU::extendedOpcode0xDC(){}
void CPU::extendedOpcode0xDD(){}
void CPU::extendedOpcode0xDE(){}
void CPU::extendedOpcode0xDF(){}
void CPU::extendedOpcode0xE0(){}
void CPU::extendedOpcode0xE1(){}
void CPU::extendedOpcode0xE2(){}
void CPU::extendedOpcode0xE3(){}
void CPU::extendedOpcode0xE4(){}
void CPU::extendedOpcode0xE5(){}
void CPU::extendedOpcode0xE6(){}
void CPU::extendedOpcode0xE7(){}
void CPU::extendedOpcode0xE8(){}
void CPU::extendedOpcode0xE9(){}
void CPU::extendedOpcode0xEA(){}
void CPU::extendedOpcode0xEB(){}
void CPU::extendedOpcode0xEC(){}
void CPU::extendedOpcode0xED(){}
void CPU::extendedOpcode0xEE(){}
void CPU::extendedOpcode0xEF(){}
void CPU::extendedOpcode0xF0(){}
void CPU::extendedOpcode0xF1(){}
void CPU::extendedOpcode0xF2(){}
void CPU::extendedOpcode0xF3(){}
void CPU::extendedOpcode0xF4(){}
void CPU::extendedOpcode0xF5(){}
void CPU::extendedOpcode0xF6(){}
void CPU::extendedOpcode0xF7(){}
void CPU::extendedOpcode0xF8(){}
void CPU::extendedOpcode0xF9(){}
void CPU::extendedOpcode0xFA(){}
void CPU::extendedOpcode0xFB(){}
void CPU::extendedOpcode0xFC(){}
void CPU::extendedOpcode0xFD(){}
void CPU::extendedOpcode0xFE(){}
void CPU::extendedOpcode0xFF(){}
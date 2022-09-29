#include "cpu.h"

Register::Register() {
	this->low = NULL;
	this->high = NULL;
}

Register::~Register() {}

Register::Register(uint8_t* l, uint8_t* h) {
	this->low = l;
	this->high = h;
}

uint16_t Register::getRegister() {
	return *this->high << 8 | (*this->low & 0x00FF);
}

void Register::setRegister(uint16_t value) {
	*low = value & 0x00FF;
	*high = value >> 8;
}
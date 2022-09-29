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

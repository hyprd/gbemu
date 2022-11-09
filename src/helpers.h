#pragma once
#include <string>

enum DType {
	Debug,
	Info,
	Error
};

void PrintMessage(DType type, std::string message);
void PrintHex(uint8_t decimal);
void PrintHex16(uint16_t decimal);
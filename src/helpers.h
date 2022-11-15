#pragma once
#include <string>
#include <iostream>
#include <bitset>

enum DType {
	Debug,
	Info,
	Error
};

void PrintMessage(DType type, std::string message);
void PrintHex(uint8_t decimal);
void PrintHex16(uint16_t decimal);
void PrintBinary8(uint8_t hex);
void PrintBinary16(uint16_t hex);
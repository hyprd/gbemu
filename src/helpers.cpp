#include "helpers.h"

void PrintMessage(DType type, std::string message) {
	std::string t;
	switch (type) {
	case Debug:
		t = "DEBUG";
		break;
	case Info:
		t = "INFO";
		break;
	case Error:
		t = "ERROR";
		break;
	default:
		t = "UNKNOWN";
		break;
	}
	printf("[%s]%s\n", t.c_str(), message.c_str());
}

void PrintHex(uint8_t decimal) {
	printf("%02X ", decimal);
}

void PrintHex16(uint16_t decimal) {
	printf("%04X ", decimal);
}

void PrintBinary8(uint8_t hex) {
	std::cout << std::bitset<8>(hex) << " ";
}

void PrintBinary16(uint16_t hex) {
	std::cout << std::bitset<16>(hex) << " ";
}
#pragma once
//#include "definitions.h"
#include <string>

enum DType {
	Debug,
	Info,
	Error
};

void PrintMessage(DType type, std::string message);
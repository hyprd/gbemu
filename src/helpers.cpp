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
	printf("[%s]%s", t.c_str(), message.c_str());
}
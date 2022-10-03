#include "opcodes.h"

void bindOpcodes(CPU* cpu) {
	cpu->opcodes[0x00] = &Opcode0x00;
}

void Opcode0x00() {

}
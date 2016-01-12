#pragma once
//#include "Gameboy.h"
#include <functional>
#include <vector>
#include "BPrefix.h"
class Gameboy;
class CBPrefix;
struct instruction {
	std::string dissasembly;
	unsigned char lenght;
	unsigned char cycles;
	std::function<void(Gameboy&, unsigned short)> fn;
};

class GB_Cpu {
public:
	CBPrefix *cbpref;
	uint64_t cycles{ 0 };
	std::vector<instruction> instructions;
	void buildInstructionsVector();
	uint8_t StepInstruction(Gameboy&machine);

	void vblancInterrupt(Gameboy&machine);
};
void checkbit(Gameboy&machine, uint8_t reg, uint8_t bit);

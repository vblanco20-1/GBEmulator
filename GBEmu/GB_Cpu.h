#pragma once
#include "Gameboy.h"
#include <functional>
#include <vector>


struct instruction {
	std::string dissasembly;
	unsigned char lenght;
	unsigned char cycles;
	std::function<void(Gameboy&, unsigned short)> fn;
};

class GB_Cpu {
public:
	std::vector<instruction> instructions;
	void buildInstructionsVector();
	void StepInstruction(Gameboy&machine);
};


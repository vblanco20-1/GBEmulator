#pragma once
#include "Gameboy.h"
#include <functional>
#include <vector>

void StepInstruction(Gameboy&machine);

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


	//{"NOOP" , 0 , [](Gameboy&mc, unsigned short op) {} }, //0x00
	//{"NOOP" , 0 , [](Gameboy&mc, unsigned short op) {} }, //0x01

#pragma once
#include <functional>
#include <iostream>
#include "Gameboy.h"
class Gameboy;
struct CBInst {
	std::string dissasembly;
	
	void(*fn)(Gameboy&);
};


class CBPrefix
{
public:

	void initFunctionArray();

	void execute(Gameboy&mc,uint8_t op);
	std::vector<CBInst> instructions;
	CBPrefix();
	~CBPrefix();
};


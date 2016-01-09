#pragma once
#include "Gameboy.h"

namespace GBTests {
	

	 bool TestStack();
	 bool TestAll();
	 bool TestFlags();
	 void DissassembleRom(std::string romfilename, std::string outfilename);
}
#include <SDL.h>
#include <SDL_render.h>
#include <iostream>
#include <vector>

#include "Screen.h"
#include "Gameboy.h"
#include "GB_Cpu.h"
#include "Tests.h"
using namespace std;

int main(int argc, char** argv)
{
	const bool bRunTests = false;
	if (bRunTests)
	{
		if (!GBTests::TestAll())
		{
			cout << "TESTS FAILED" << endl;
		}
	}
	
	Screen screen;
	Gameboy machine;
	GB_Cpu cpu;
	machine.LoadROM("Roms/Tetris.gbc");
	cpu.buildInstructionsVector();
	while (true)
	{
		cpu.StepInstruction(machine);
	}
	return 0;
}
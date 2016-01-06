#include <SDL.h>
#include <SDL_render.h>
#include <iostream>
#include <vector>

#include "Screen.h"
#include "Gameboy.h"
#include "GB_Cpu.h"
using namespace std;

int main(int argc, char** argv)
{

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
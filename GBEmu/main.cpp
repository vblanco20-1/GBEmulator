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
	
	
	Gameboy machine;
	
	//GBTests::DissassembleRom("Roms/Opus5.gb", "dissasembly.txt");
	//machine.LoadROM("Roms/Opus5.gb");
	machine.LoadROM("Roms/03-op sp,hl.gb");
	machine.Run();
	return 0;
}
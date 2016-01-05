#include <SDL.h>
#include <SDL_render.h>
#include <iostream>
#include <vector>

#include "Screen.h"
#include "Gameboy.h"
using namespace std;

int main(int argc, char** argv)
{

	Screen screen;
	Gameboy machine;
	machine.LoadROM("Roms/03-op sp,hl.gb");
	

	return 0;
}
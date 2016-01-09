#include "Gameboy.h"

#include <fstream>
#include <iostream>

using namespace std;

Gameboy::Gameboy()
{
	reset();
}


Gameboy::~Gameboy()
{
}

bool getbit(uint8_t byte, int nbit)
{
	return (byte >> nbit) & 1;
}


void Gameboy::writeByte(byte b, unsigned short idx)
{
	if (idx == 0xFF40)
	{
		
		
		bool bit6 = getbit(b, 6);
		bool bit4 = getbit(b, 4);
		bool bit3 = getbit(b, 3);
		cout << "LCDC register" << endl;


	}
	if (idx >= 0x8000 && idx <= 0xA000)
	{
		cout << "Graphics edit "<< hex << b << endl;

	}
	if (idx >= 0x0000 && idx < 0x8000)
	{
		cout << "CANT EDIT THIS ITS ROM" << hex << uint8_t(b) << endl;
		return;
	}

	if (idx >= 0xff51 && idx < 0xff56)
	{
		cout << "HDMA" << hex << uint8_t(b) << endl;
		return;
	}
	if (idx == 0xff43)
	{
		//scrollx
		GBScreen.SetScrollX(b);
	}
	if (idx == 0xff44)
	{
		GBScreen.SetScrollY(b);
		//scrolly
	}
	if (idx == 0xff46) //DMAAA
	{
		uint16_t start = b << 8;

		for (int i = 0; i < 140; i++)
		{
			memory[0xfe00 + i] = memory[start + i];
		}

	}
	memory[idx] = b;
}

byte Gameboy::readByte(unsigned short idx)
{
	return memory[idx];
}
byte & Gameboy::getByte(unsigned short idx)
{
	return memory[idx];
}
unsigned short Gameboy::readShort(unsigned short idx)
{
	unsigned short byte1 = readByte(idx);
	unsigned short byte2 = readByte(idx + 1);
	return byte1 | (byte2 << 8);
}
void Gameboy::writeShort(unsigned short addr, unsigned short value)
{
	writeByte(value & 0x00FF ,addr);
	writeByte((value & 0xFF00) >> 8,addr+1);
}
bool Gameboy::LoadROM(std::string Address)
{
	ifstream rom;
	rom.open(Address, ios::binary);
	reset();
	if (rom.is_open())
	{
		rom.seekg(0, ios::end);
		auto size = rom.tellg();
		auto memblock = new char[size];
		rom.seekg(0, ios::beg);
		rom.read(memblock, size);
		memcpy(&memory[0], memblock, size);

		cout << "ROM successfully loaded: " << Address << endl;

		delete[] memblock;

		reset();
		return true;
	}

	return false;
}

void Gameboy::reset()
{
	Registers.a = 0x01;
	Registers.f = 0xb0;
	Registers.b = 0x00;
	Registers.c = 0x13;
	Registers.d = 0x00;
	Registers.e = 0xd8;
	Registers.h = 0x01;
	Registers.l = 0x4d;
	Registers.sp = 0xfffe;
	Registers.pc = 0x100;
	
	memory[0xff41] = 0;

	CPU.buildInstructionsVector();

	GBScreen.Clear();
}

void Gameboy::Run()
{
	int vblanclaunch = 4194304 / 60; //cycles per second /60
	int currentcycles =0;
	while (true)
	{
		int cycles = CPU.StepInstruction(*this);
		currentcycles += cycles;
		if (currentcycles > vblanclaunch) {
			currentcycles = 0;
			//vblanc interupt

			SDL_Event event;
			while (SDL_PollEvent(&event))
			{

			}
			if (bInterruptsEnabled)
			{
				CPU.vblancInterrupt(*this);
			}
			
			
			for (int x = 0; x < 50; x++)
			{
				std::vector<uint8_t> testtile(16);
				for (int i = 0; i < 16; i++)
				{
					testtile[i] = memory[0x4000 + i + x * 16];
				}
				GBScreen.DrawTile(testtile, (x % 10) * 8, (x / 10) * 8);
			}

			for (int x = 0; x < 50; x++)
			{
				std::vector<uint8_t> testtile(16);
				for (int i = 0; i < 16; i++)
				{
					testtile[i] = memory[0xfe00 + i + x * 16];
				}
				GBScreen.DrawTile(testtile, (x % 10) * 8, 50 + (x / 10) * 8);
			}

			for (int x = 0; x < 50; x++)
			{
				std::vector<uint8_t> testtile(16);
				for (int i = 0; i < 16; i++)
				{
					testtile[i] = memory[0x8000 + i + x * 16];
				}
				GBScreen.DrawTile(testtile, (x % 10) * 8, 100 + (x / 10) * 8);
			}

			GBScreen.DrawFrame();
		}
		
		
		
		
			
	}
}

void Gameboy::SetFlags(unsigned char flags)
{
	Registers.f = Registers.f | flags;
}

void Gameboy::ClearFlags(unsigned char flags)
{
	Registers.f = Registers.f & ~flags;
}

bool Gameboy::GetFlag(unsigned char flag)
{
	return ((Registers.f & flag) != 0) ;
}


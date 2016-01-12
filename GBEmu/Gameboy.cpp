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




void Gameboy::writeByte(byte b, unsigned short idx)
{
	if (idx == 0xFF40)
	{
		
		
		bool bit6 = getbit(b, 6);
		bool bit4 = getbit(b, 4);
		bool bit3 = getbit(b, 3);
		bool bit1 = getbit(b, 1);
		bool bit0 = getbit(b, 0);
		cout << "LCDC register" << endl;
		if (bit6)
		{
			//GBScreen.SetMapAdress(0x9C00);
		}
		else
		{
			//GBScreen.SetMapAdress(0x9800);
		}
		if (bit3)
		{
			GBScreen.SetMapAdress(0x9C00);
		}
		else 
		{
			GBScreen.SetMapAdress(0x9800);
		}
	}
	if (idx >= 0x8000 && idx <= 0xA000)
	{
		
		//cout << "Graphics edit "<< hex << b << endl;
		
		
		/*if (idx >= 0x9800 && idx <= 0x9C00)
		{
			if (b == 0)
			{
				cout << "writigng 0 in tile map" << hex << b << endl;
				return;
			}
		}*/
		GBScreen.writeByte(idx, b);
		//GBScreen.vram[idx - 0x8000] = b;
	}
	if (idx >= 0x0000 && idx < 0x4000)
	{
		cout << "CANT EDIT THIS ITS ROM" << hex << uint8_t(b) << endl;
		//return;
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
	if (idx == 0xff42)
	{
		GBScreen.SetScrollY(b);
		//scrolly
	}
	if (idx == 0xff46) //DMAAA
	{
		uint16_t start = b << 8;
		//cout << "DMA:" << start << endl;
		for (int i = 0; i < 140; i++)
		{
			memory[0xfe00 + i] = memory[start + i];
			GBScreen.writeByte(0xfe00 + i, memory[start + i]);
		}
	}
	if (idx == 0xff00) // input
	{
		const uint8_t* keys = SDL_GetKeyboardState(nullptr);
		memory[0xff00] = b;
		if (b == 0x20) // bit 5,p14
		{
			memory[0xff00] |= 0xf;
			if (keys[SDL_SCANCODE_DOWN]  )
			{
				memory[0xff00] ^= 1 << 3; //p13 DOWN
			}
			if (keys[SDL_SCANCODE_UP]) 
			{
				memory[0xff00] ^= 1 << 2; //p12 UP
			}
			if (keys[SDL_SCANCODE_LEFT])
			{
				memory[0xff00] ^= 1 << 1; //p11 left
			}
			if (keys[SDL_SCANCODE_RIGHT])
			{
				memory[0xff00] ^= 1 << 0; //p10 right
			}

			return;
		}
		else if (b == 0x10) //p15
		{		
			memory[0xff00] |= 0xf;
			if (keys[SDL_SCANCODE_SPACE])
			{
				memory[0xff00] ^= 1 << 3; //p13 Start
			}
			if (keys[SDL_SCANCODE_RETURN])
			{
				memory[0xff00] ^= 1 << 2; //p12 Select
			}
			if (keys[SDL_SCANCODE_S])
			{
				memory[0xff00] ^= 1 << 1; //p11 B
			}
			if (keys[SDL_SCANCODE_A])
			{
				memory[0xff00] ^= 1 << 0; //p10 A
			}

			return;

			
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
	

	memory[0xff05] = 0; //TIMA
	memory[0xff06] = 0; //TMA
	memory[0xff07] = 0; //TAC

	memory[0xff10] = 0x80;//NR10
	memory[0xff11] = 0xBF;//NR11
	memory[0xff12] = 0xF3;//NR12
	memory[0xff15] = 0xBF;//NR14

	memory[0xff16] = 0x3F;//NR21
	memory[0xff17] = 0x00;//NR22
	memory[0xff19] = 0xBF;//NR24

	memory[0xff1A] = 0x7F;//NR30
	memory[0xff1B] = 0xFF;//NR31
	memory[0xff1C] = 0x9F;//NR32
	memory[0xff1E] = 0xBF;//NR33

	memory[0xff20] = 0xFF;//NR41
	memory[0xff21] = 0x00;//NR42
	memory[0xff22] = 0x00;//NR43

	memory[0xff23] = 0xBF;//NR30
	memory[0xff24] = 0x77;//NR50
	memory[0xff25] = 0xF3;//NR51
	memory[0xff26] = 0xF1;//NR41

	memory[0xff40] = 0x91;//LDLC
	memory[0xff42] = 0x00;//SCY
	memory[0xff43] = 0x00;//SCX
	memory[0xff44] = 0x01;//LCY

	memory[0xff47] = 0xFC;//BGP
	memory[0xff48] = 0xFF;//OBP0
	memory[0xff49] = 0xFF;//OBP1
	memory[0xff4A] = 0x00;//WY
	memory[0xff4B] = 0x00;//WX
	memory[0xffff] = 0x00;//IE

	memory[0xff41] = 0;

	CPU.buildInstructionsVector();

	GBScreen.Clear();
}

void Gameboy::Run()
{
	int vblanclaunch = 4194304 / 60; //cycles per second /60
	int currentcycles =0;
	int ldlyc =0 ;
	while (true)
	{
		int cycles = CPU.StepInstruction(*this);
		currentcycles += cycles;
		ldlyc += cycles;
		if (ldlyc > 40)
		{
			ldlyc = 0;
			memory[0xff44]++;
			if (memory[0xff44] > 153)
			{
				memory[0xff44] = 0;
			}
		}

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
			
			
			/*
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
			}*/
			//cout << "VBlanc frame" << endl;
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

void Gameboy::UpdateInterrupts(int time)
{

}


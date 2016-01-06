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
	memory[idx] = b;
}

byte Gameboy::readByte(unsigned short idx)
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


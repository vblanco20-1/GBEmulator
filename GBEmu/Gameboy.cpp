#include "Gameboy.h"

#include <fstream>
#include <iostream>

using namespace std;

Gameboy::Gameboy()
{
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

bool Gameboy::LoadROM(std::string Address)
{
	ifstream rom;
	rom.open(Address, ios::binary);

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
		return true;
	}

	return false;
}


#include "Tests.h"
#include "GB_Cpu.h"
#include <iostream>
#include <fstream>
using namespace std;
bool GBTests::TestStack()
{
	Gameboy Machine;
	GB_Cpu cpu;
	cpu.buildInstructionsVector();
	unsigned short BC = Machine.Registers.bc;
	unsigned short DE = Machine.Registers.de;
	unsigned short HL = Machine.Registers.hl;
	unsigned short AF = Machine.Registers.af;

	Machine.memory[0x0000] = 0xC5; //push BC
	Machine.memory[0x0001] = 0xD5; //push DE
	Machine.memory[0x0002] = 0xE5; //push HL
	Machine.memory[0x0003] = 0xF5; //push AF


	Machine.memory[0x0004] = 0xF1; //pop AF	
	Machine.memory[0x0005] = 0xE1; //pop HL
	Machine.memory[0x0006] = 0xD1; //pop DE
	Machine.memory[0x0007] = 0xC1; //pop BC

	cpu.StepInstruction(Machine);
	cpu.StepInstruction(Machine);
	cpu.StepInstruction(Machine);
	cpu.StepInstruction(Machine);
	cpu.StepInstruction(Machine);
	cpu.StepInstruction(Machine);
	cpu.StepInstruction(Machine);
	cpu.StepInstruction(Machine);

	if ((Machine.Registers.bc != BC))
	{
		return false;
	}	
	if ((Machine.Registers.de != DE))
	{
		return false;
	}
	if ((Machine.Registers.af != AF))
	{
		return false;
	}
	if ((Machine.Registers.hl != HL))
	{
		return false;
	}

	return true;
	
}

bool GBTests::TestAll()
{
	if (!TestStack())
	{
		std::cout << "Testing the stack failed" << std::endl;
		return false;
	}
	if (!TestFlags())
	{
		std::cout << "Testing the flags failed" << std::endl;
		return false;
	}
	return true;
}

bool GBTests::TestFlags()
{
	Gameboy mc;
	mc.ClearFlags(Zero_Flag | Subtract_Flag| Carry_Flag | HalfCarry_Flag);

	if (mc.Registers.f != 0) return false;

	mc.SetFlags(Zero_Flag);

	if (!mc.GetFlag(Zero_Flag))
	{
		return false;
	}

	mc.SetFlags(Carry_Flag);

	if (!mc.GetFlag(Carry_Flag))
	{
		return false;
	}
	mc.ClearFlags(Zero_Flag);
	if (mc.GetFlag(Zero_Flag))
	{
		return false;
	}


	return true;
}

void GBTests::DissassembleRom(std::string romfilename, std::string outfilename)
{
	ifstream rom;
	rom.open(romfilename, ios::binary);
	
	if (rom.is_open())
	{
		rom.seekg(0, ios::end);
		auto size = rom.tellg();
		auto memblock = new char[1];
		rom.seekg(0, ios::beg);
		ofstream dissasembly;
		dissasembly.open(outfilename);
		GB_Cpu cpu;
		cpu.buildInstructionsVector();
		for (int i = 0; i < size; i++)
		{
			char ch;
			rom.read(&ch, 1);
			unsigned char chu = ch;
			dissasembly <<hex <<i <<":"<< cpu.instructions[chu].dissasembly << endl;
			i += cpu.instructions[chu].lenght - 1;

		}
		dissasembly.close();
		

		//cout << "ROM successfully loaded: " << Address << endl;

		delete[] memblock;

		
		
	}

	
}

bool GBTests::TestCheckbit()
{
	Gameboy mc;
	mc.ClearFlags(Zero_Flag);
	uint8_t val = 0;
	checkbit(mc, val, 1);


	if (!mc.GetFlag(Zero_Flag))
	{
		return false;
	}

	val = 1 << 0;
	checkbit(mc, val, 1);
}

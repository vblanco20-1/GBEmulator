#include "Tests.h"
#include "GB_Cpu.h"
#include <iostream>
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
	return true;
}

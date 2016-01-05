#include "GB_Cpu.h"


#define INSTLAMBDA  [](Gameboy&mc, unsigned short op)

void GB_Cpu::StepInstruction(Gameboy&machine)
{
	byte c = machine.readByte(machine.Registers.pc);

	machine.Registers.pc++;
}

void GB_Cpu::buildInstructionsVector()
{	
	//fill instruction array with NOOPs unimplemented
	instructions = std::vector<instruction>(256, { "UNIMPLEMENTED" , 0 ,0 , [](Gameboy&mc, unsigned short op) {} } );

	instructions[0x00] = { "NOOP"       ,0 ,1  ,INSTLAMBDA{} };
	instructions[0x01] = { "LD BC, n16" ,2 ,12 ,INSTLAMBDA{mc.Registers.bc = op; } };
	instructions[0x02] = { "LD (BC), A" ,0 ,8  ,INSTLAMBDA{mc.Registers.bc = mc.Registers.a; } };
	instructions[0x03] = { "INC BC    " ,0 ,8  ,INSTLAMBDA{mc.Registers.bc++; } };
	instructions[0x04] = { "INC B     " ,0 ,4  ,INSTLAMBDA{mc.Registers.b++ ; } };
	instructions[0x05] = { "DEC B     " ,0 ,4  ,INSTLAMBDA{mc.Registers.b-- ; } };
	instructions[0x06] = { "LD B, n8  " ,1 ,8  ,INSTLAMBDA{mc.Registers.b = byte( op); } };
}


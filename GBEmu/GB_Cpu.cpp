#include "GB_Cpu.h"


#define INSTLAMBDA  [](Gameboy&mc, unsigned short op)

void rlca(Gameboy&mc)
{
	unsigned char carry = (mc.Registers.a & 0x80) >> 7;
	//	if (carry) FLAGS_SET(FLAGS_CARRY);
	//	else FLAGS_CLEAR(FLAGS_CARRY);

	mc.Registers.a <<= 1;
	mc.Registers.a += carry;

	//	FLAGS_CLEAR(FLAGS_NEGATIVE | FLAGS_ZERO | FLAGS_HALFCARRY);
}
void inc(unsigned char& val) {
	val++;
}
void dec(unsigned char& val) {
	val--;
}



void GB_Cpu::StepInstruction(Gameboy&machine)
{
	byte c = machine.readByte(machine.Registers.pc);

	machine.Registers.pc++;
}



void GB_Cpu::buildInstructionsVector()
{	
	//fill instruction array with NOOPs unimplemented
	instructions = std::vector<instruction>(256, { "UNIMPLEMENTED" , 0 ,0 , [](Gameboy&mc, unsigned short op) {} } );

	instructions[0x00] = { "NOOP"       ,0 ,4  ,INSTLAMBDA{} };
	instructions[0x01] = { "LD BC, n16" ,2 ,12 ,INSTLAMBDA{mc.Registers.bc = op; } };
	instructions[0x02] = { "LD (BC), A" ,0 ,8  ,INSTLAMBDA{mc.writeByte(mc.Registers.a,mc.Registers.bc); }};
	
	

	//8 bit loads LD R n8
	instructions[0x06] = { "LD B, n8  " ,1 ,8  ,INSTLAMBDA{ mc.Registers.b = byte( op); } };
	instructions[0x0E] = { "LD C, n8  " ,1 ,8  ,INSTLAMBDA{ mc.Registers.c = byte(op); } };
	instructions[0x16] = { "LD D, n8  " ,1 ,8  ,INSTLAMBDA{ mc.Registers.d = byte(op); } };
	instructions[0x1E] = { "LD E, n8  " ,1 ,8  ,INSTLAMBDA{ mc.Registers.e = byte(op); } };
	instructions[0x26] = { "LD H, n8  " ,1 ,8  ,INSTLAMBDA{ mc.Registers.h = byte(op); } };
	instructions[0x2E] = { "LD L, n8  " ,1 ,8  ,INSTLAMBDA{ mc.Registers.l = byte(op); } };

	//8 bit loads LD R1 R2
	instructions[0x7F] = { "LD A, A  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.a = mc.Registers.a; } };
	instructions[0x78] = { "LD A, B  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.a = mc.Registers.b; } };
	instructions[0x79] = { "LD A, C  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.a = mc.Registers.c; } };
	instructions[0x7A] = { "LD A, D  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.a = mc.Registers.d; } };
	instructions[0x7B] = { "LD A, E  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.a = mc.Registers.e; } };
	instructions[0x7C] = { "LD A, H  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.a = mc.Registers.h; } };
	instructions[0x7D] = { "LD A, L  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.a = mc.Registers.l; } };
	//instructions[0x7E] = { "LD A, A  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.a = mc.Registers.a; } };
	instructions[0x40] = { "LD B, B  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.b = mc.Registers.b; } };


	instructions[0x07] = { "RLCA      " ,0 ,4  ,INSTLAMBDA{ rlca(mc); } };
	instructions[0x08] = { "LD n16, SP  ",2 ,8  ,INSTLAMBDA{ mc.Registers.sp = op; } };






	//INC nn
	instructions[0x03] = { "INC BC    " ,0 ,8  ,INSTLAMBDA{ mc.Registers.bc++; } };
	instructions[0x13] = { "INC DE    " ,0 ,8  ,INSTLAMBDA{ mc.Registers.de++; } };
	instructions[0x23] = { "INC HL    " ,0 ,8  ,INSTLAMBDA{ mc.Registers.hl++; } };
	instructions[0x33] = { "INC SP    " ,0 ,8  ,INSTLAMBDA{ mc.Registers.sp++; } };
	//DEC nn
	instructions[0x0B] = { "DEC BC    " ,0 ,8  ,INSTLAMBDA{ mc.Registers.bc--; } };
	instructions[0x1B] = { "DEC DE    " ,0 ,8  ,INSTLAMBDA{ mc.Registers.de--; } };
	instructions[0x2B] = { "DEC HL    " ,0 ,8  ,INSTLAMBDA{ mc.Registers.hl--; } };
	instructions[0x3B] = { "DEC SP    " ,0 ,8  ,INSTLAMBDA{ mc.Registers.sp--; } };

	//INC n
	instructions[0x3C] = { "INC A	  " ,0 ,4  ,INSTLAMBDA{ inc(mc.Registers.a); } };
	instructions[0x04] = { "INC B     " ,0 ,4  ,INSTLAMBDA{ inc(mc.Registers.b); } };	
	instructions[0x0C] = { "INC C     " ,0 ,4  ,INSTLAMBDA{ inc(mc.Registers.c); } };
	instructions[0x14] = { "INC D     " ,0 ,4  ,INSTLAMBDA{ inc(mc.Registers.d); } };
	instructions[0x1C] = { "INC E     " ,0 ,4  ,INSTLAMBDA{ inc(mc.Registers.e); } };
	instructions[0x24] = { "INC H     " ,0 ,4  ,INSTLAMBDA{ inc(mc.Registers.h); } };
	instructions[0x2C] = { "INC L     " ,0 ,4  ,INSTLAMBDA{ inc(mc.Registers.l); } };
	//DEC n	
	instructions[0x3D] = { "DEC A	  " ,0 ,4  ,INSTLAMBDA{ dec(mc.Registers.a); } };
	instructions[0x05] = { "DEC B     " ,0 ,4  ,INSTLAMBDA{ dec(mc.Registers.b); } };
	instructions[0x0D] = { "DEC C	  " ,0 ,4  ,INSTLAMBDA{ dec(mc.Registers.c); } };
	instructions[0x15] = { "DEC D     " ,0 ,4  ,INSTLAMBDA{ dec(mc.Registers.d); } };
	instructions[0x1D] = { "DEC E     " ,0 ,4  ,INSTLAMBDA{ dec(mc.Registers.e); } };
	instructions[0x25] = { "DEC H	  " ,0 ,4  ,INSTLAMBDA{ dec(mc.Registers.h); } };
	instructions[0x2D] = { "DEC L     " ,0 ,4  ,INSTLAMBDA{ dec(mc.Registers.l); } };
}

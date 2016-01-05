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
void add16(Gameboy&mc, unsigned short &A, unsigned short B)
{
	A += B;
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

	instructions[0x00] = { "NOOP"       ,1 ,4  ,INSTLAMBDA{} };
	instructions[0x01] = { "LD BC, n16" ,3 ,12 ,INSTLAMBDA{mc.Registers.bc = op; } };
	instructions[0x02] = { "LD (BC), A" ,1 ,8  ,INSTLAMBDA{mc.writeByte(mc.Registers.a,mc.Registers.bc); }};
	
	

	//8 bit loads LD R n8
	instructions[0x06] = { "LD B, n8  " ,2 ,8  ,INSTLAMBDA{ mc.Registers.b = byte(op); } };
	instructions[0x0E] = { "LD C, n8  " ,2 ,8  ,INSTLAMBDA{ mc.Registers.c = byte(op); } };
	instructions[0x16] = { "LD D, n8  " ,2 ,8  ,INSTLAMBDA{ mc.Registers.d = byte(op); } };
	instructions[0x1E] = { "LD E, n8  " ,2 ,8  ,INSTLAMBDA{ mc.Registers.e = byte(op); } };
	instructions[0x26] = { "LD H, n8  " ,2 ,8  ,INSTLAMBDA{ mc.Registers.h = byte(op); } };
	instructions[0x2E] = { "LD L, n8  " ,2 ,8  ,INSTLAMBDA{ mc.Registers.l = byte(op); } };
	instructions[0x36] = { "LD (HL), n8  " ,2 ,12  ,INSTLAMBDA{ mc.writeByte(byte(op), mc.Registers.hl); } };


	//8 bit loads LD R1 R2
	//LD A,r
	instructions[0x7F] = { "LD A, A  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.a = mc.Registers.a; } };
	instructions[0x78] = { "LD A, B  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.a = mc.Registers.b; } };
	instructions[0x79] = { "LD A, C  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.a = mc.Registers.c; } };
	instructions[0x7A] = { "LD A, D  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.a = mc.Registers.d; } };
	instructions[0x7B] = { "LD A, E  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.a = mc.Registers.e; } };
	instructions[0x7C] = { "LD A, H  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.a = mc.Registers.h; } };
	instructions[0x7D] = { "LD A, L  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.a = mc.Registers.l; } };
	instructions[0x7E] = { "LD A, (HL)",1 ,8  ,INSTLAMBDA{ mc.Registers.a = mc.readByte(mc.Registers.hl);} };
	//LD B,r
	instructions[0x40] = { "LD B, B  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.b = mc.Registers.b; } };
	instructions[0x41] = { "LD B, C  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.b = mc.Registers.c; } };
	instructions[0x42] = { "LD B, D  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.b = mc.Registers.d; } };
	instructions[0x43] = { "LD B, E  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.b = mc.Registers.e; } };
	instructions[0x44] = { "LD B, H  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.b = mc.Registers.h; } };
	instructions[0x45] = { "LD B, L  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.b = mc.Registers.l; } };
	instructions[0x46] = { "LD B, (HL)",1 ,8  ,INSTLAMBDA{ mc.Registers.b = mc.readByte(mc.Registers.hl); } };
	instructions[0x47] = { "LD B, A  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.b = mc.Registers.a; } };
	//LD C,r
	instructions[0x48] = { "LD C, B  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.c = mc.Registers.b; } };
	instructions[0x49] = { "LD C, C  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.c = mc.Registers.c; } };
	instructions[0x4A] = { "LD C, D  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.c = mc.Registers.d; } };
	instructions[0x4B] = { "LD C, E  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.c = mc.Registers.e; } };
	instructions[0x4C] = { "LD C, H  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.c = mc.Registers.h; } };
	instructions[0x4D] = { "LD C, L  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.c = mc.Registers.l; } };
	instructions[0x4E] = { "LD C, (HL)",1 ,8  ,INSTLAMBDA{ mc.Registers.c = mc.readByte(mc.Registers.hl);  } };
	instructions[0x4F] = { "LD C, A  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.c = mc.Registers.a; } };
	//LD D,r 
	instructions[0x50] = { "LD D, B  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.d = mc.Registers.b; } };
	instructions[0x51] = { "LD D, C  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.d = mc.Registers.c; } };
	instructions[0x52] = { "LD D, D  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.d = mc.Registers.d; } };
	instructions[0x53] = { "LD D, E  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.d = mc.Registers.e; } };
	instructions[0x54] = { "LD D, H  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.d = mc.Registers.h; } };
	instructions[0x55] = { "LD D, L  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.d = mc.Registers.l; } };
	instructions[0x56] = { "LD D, (HL)",1 ,8  ,INSTLAMBDA{ mc.Registers.d = mc.readByte(mc.Registers.hl); } };
	instructions[0x57] = { "LD D, A  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.d = mc.Registers.a; } };
	//LD E,r 
	instructions[0x58] = { "LD E, B  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.e = mc.Registers.b; } };
	instructions[0x59] = { "LD E, C  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.e = mc.Registers.c; } };
	instructions[0x5A] = { "LD E, D  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.e = mc.Registers.d; } };
	instructions[0x5B] = { "LD E, E  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.e = mc.Registers.e; } };
	instructions[0x5C] = { "LD E, H  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.e = mc.Registers.h; } };
	instructions[0x5D] = { "LD E, L  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.e = mc.Registers.l; } };
	instructions[0x5E] = { "LD E, (HL)",1 ,8  ,INSTLAMBDA{ mc.Registers.e = mc.readByte(mc.Registers.hl); } };
	instructions[0x5F] = { "LD E, A  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.e = mc.Registers.a; } };
	//LD H,r 
	instructions[0x60] = { "LD H, B  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.h = mc.Registers.b; } };
	instructions[0x61] = { "LD H, C  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.h = mc.Registers.c; } };
	instructions[0x62] = { "LD H, D  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.h = mc.Registers.d; } };
	instructions[0x63] = { "LD H, E  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.h = mc.Registers.e; } };
	instructions[0x64] = { "LD H, H  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.h = mc.Registers.h; } };
	instructions[0x65] = { "LD H, L  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.h = mc.Registers.l; } };
	instructions[0x66] = { "LD H, (HL)",1 ,8  ,INSTLAMBDA{ mc.Registers.h = mc.readByte(mc.Registers.hl); } };
	instructions[0x67] = { "LD H, A  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.h = mc.Registers.a; } };
	//LD L,r 
	instructions[0x68] = { "LD L, B  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.l = mc.Registers.b; } };
	instructions[0x69] = { "LD L, C  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.l = mc.Registers.c; } };
	instructions[0x6A] = { "LD L, D  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.l = mc.Registers.d; } };
	instructions[0x6B] = { "LD L, E  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.l = mc.Registers.e; } };
	instructions[0x6C] = { "LD L, H  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.l = mc.Registers.h; } };
	instructions[0x6D] = { "LD L, L  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.l = mc.Registers.l; } };
	instructions[0x6E] = { "LD L, (HL)",1 ,8  ,INSTLAMBDA{ mc.Registers.l = mc.readByte(mc.Registers.hl); } };
	instructions[0x6F] = { "LD L, A  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.l = mc.Registers.a; } };

	//LD (HL),r
	instructions[0x70] = { "LD (HL), B  " ,1 ,8  ,INSTLAMBDA{ mc.writeByte(mc.Registers.b, mc.Registers.hl); } };
	instructions[0x71] = { "LD (HL), C  " ,1 ,8  ,INSTLAMBDA{ mc.writeByte(mc.Registers.c, mc.Registers.hl); } };
	instructions[0x72] = { "LD (HL), D  " ,1 ,8  ,INSTLAMBDA{ mc.writeByte(mc.Registers.d, mc.Registers.hl); } };
	instructions[0x73] = { "LD (HL), E  " ,1 ,8  ,INSTLAMBDA{ mc.writeByte(mc.Registers.e, mc.Registers.hl); } };
	instructions[0x74] = { "LD (HL), H  " ,1 ,8  ,INSTLAMBDA{ mc.writeByte(mc.Registers.h, mc.Registers.hl); } };
	instructions[0x75] = { "LD (HL), L  " ,1 ,8  ,INSTLAMBDA{ mc.writeByte(mc.Registers.l, mc.Registers.hl); } };

	
	instructions[0x07] = { "RLCA      " ,0 ,4  ,INSTLAMBDA{ rlca(mc); } };
	instructions[0x08] = { "LD n16, SP  ",2 ,8  ,INSTLAMBDA{ mc.Registers.sp = op; } };
	


	//8 bit arithmetic
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

	////// 16 bit arithmetic
	//ADD HL, n
	instructions[0x09] = { "ADD HL, BC   " ,0 ,8  ,INSTLAMBDA{ add16(mc,mc.Registers.hl,mc.Registers.bc); } };
	instructions[0x19] = { "ADD HL, DE   " ,0 ,8  ,INSTLAMBDA{ add16(mc,mc.Registers.hl,mc.Registers.de); } };
	instructions[0x29] = { "ADD HL, HL   " ,0 ,8  ,INSTLAMBDA{ add16(mc,mc.Registers.hl,mc.Registers.hl); } };
	instructions[0x39] = { "ADD HL, SP   " ,0 ,8  ,INSTLAMBDA{ add16(mc,mc.Registers.hl,mc.Registers.sp); } };
	instructions[0xE8] = { "ADD HL, BC   " ,1 ,16  ,INSTLAMBDA{ add16(mc,mc.Registers.hl,unsigned short(op & 0x00FF)); } };

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
}

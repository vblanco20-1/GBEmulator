#include "GB_Cpu.h"

#include <iostream>
#define INSTLAMBDA  [](Gameboy&mc, unsigned short op)
using namespace std;
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
	/*if (val == 0)
	{
		mc.SetFlags(Zero_Flag);
	}*/

}
void dec(unsigned char& val) {
	val--;
}
void add16(Gameboy&mc, unsigned short &A, unsigned short B)
{
	A += B;
}
void _xor(Gameboy&mc,unsigned short n)
{
	mc.Registers.a = mc.Registers.a ^ n;
	if (mc.Registers.a == 0)
	{
		mc.SetFlags(Zero_Flag);
	}
	mc.ClearFlags(HalfCarry_Flag | Carry_Flag | Subtract_Flag);
}
void add(Gameboy&mc, unsigned short n)
{
	unsigned int sum = mc.Registers.a + n;

	if( ( sum > 0xFFFF )!= 0)
	{
		mc.SetFlags(Carry_Flag);
	}
	if( sum > 8 )
	{
		mc.SetFlags(HalfCarry_Flag);
	}
	if (sum == 0)
	{
		mc.SetFlags(Zero_Flag);
	}

	mc.Registers.a = unsigned short(sum);
	mc.ClearFlags(Subtract_Flag);
}
void adc(Gameboy&mc, unsigned short n)
{}
void sub(Gameboy&mc, unsigned short n)
{
	//unsigned int sum = mc.Registers.a - n;

	/*if ((sum > 0xFFFF) != 0)
	{
		mc.SetFlags(Carry_Flag);
	}
	if (sum > 8)
	{
		mc.SetFlags(HalfCarry_Flag);
	}
	if (sum == 0)
	{
		mc.SetFlags(Zero_Flag);
	}

	mc.Registers.a = unsigned short(sum);
	mc.ClearFlags(Subtract_Flag);*/
}
void sbc(Gameboy&mc, unsigned short n)
{}
void _and(Gameboy&mc, unsigned short n)
{
	mc.Registers.a = mc.Registers.a & n;
	if (mc.Registers.a == 0)
	{
		mc.SetFlags(Zero_Flag);
	}
	mc.SetFlags(HalfCarry_Flag);
	mc.ClearFlags(Carry_Flag | Subtract_Flag);
}
void _or(Gameboy&mc, unsigned short n)
{
	mc.Registers.a = mc.Registers.a | n;
	if (mc.Registers.a == 0)
	{
		mc.SetFlags(Zero_Flag);
	}
}
void cp(Gameboy&mc, unsigned short n)
{
	if (mc.Registers.a == n)
	{
		mc.SetFlags(Zero_Flag);
	}
	mc.SetFlags(Subtract_Flag);
}


void push(Gameboy&mc, unsigned short data)
{
	mc.Registers.sp -= 2;
	mc.writeShort(mc.Registers.sp, data);	
}

void pop(Gameboy&mc, unsigned short&data)
{	
	data=mc.readShort(mc.Registers.sp);
	mc.Registers.sp += 2;
}
void call(Gameboy&mc, unsigned short addr)
{	
	push(mc, addr);
	mc.Registers.pc = addr;
}
void ret(Gameboy&mc)
{
	pop(mc, mc.Registers.sp);
}

void rst(Gameboy&mc, unsigned char n)
{
	push(mc,mc.Registers.pc);
	mc.Registers.pc = 0x0000 + n;
}

void unimplemented(unsigned char opcode = 0) {
	std::cout << "UNIMPLEMENTEEED" <<std::hex << opcode << std::endl;
	
}

void GB_Cpu::StepInstruction(Gameboy&machine)
{
	auto pc = machine.Registers.pc;
	byte c = machine.readByte(machine.Registers.pc);

	machine.Registers.pc++;
	auto & inst = instructions[c];
	unsigned short operands = 0;
	if (inst.lenght == 2)
	{
		operands = machine.readByte(pc + 1);
		machine.Registers.pc++;		
	}
	else if (inst.lenght == 3)
	{
		operands = machine.readShort(pc+1);		
		machine.Registers.pc+=2;
	}

	cout.flags(ios::right | ios::hex | ios::showbase);
	std::cout << inst.dissasembly << " " << uint8_t( c) <<" "<<  operands << std::endl;
	inst.fn(machine, operands);	
}
unsigned short turn(unsigned short n)
{
	unsigned char LS = n & 0x00FF;
	unsigned char MS = n & 0xFF00 >> 8;

	return (LS << 8) & MS;
}

void GB_Cpu::buildInstructionsVector()
{	
	//fill instruction array with NOOPs unimplemented
	instructions = std::vector<instruction>(256, { "UNIMPLEMENTED" , 1, 1 , [](Gameboy&mc, unsigned short op) {unimplemented(mc.readByte(mc.Registers.pc-1)); } });

	instructions[0x00] = { "NOOP"       ,1 ,4  ,INSTLAMBDA{} };
	instructions[0x01] = { "LD BC, n16" ,3 ,12 ,INSTLAMBDA{mc.Registers.bc = op; } };
	instructions[0x02] = { "LD (BC), A" ,1 ,8  ,INSTLAMBDA{mc.writeByte(mc.Registers.a,mc.Registers.bc); }};
	instructions[0x03] = { "INC BC    " ,1 ,8  ,INSTLAMBDA{ mc.Registers.bc++; } };
	instructions[0x04] = { "INC B     " ,1,4  ,INSTLAMBDA{ inc(mc.Registers.b); } };
	instructions[0x05] = { "DEC B     " ,1 ,4  ,INSTLAMBDA{ dec(mc.Registers.b); } };
	instructions[0x06] = { "LD B, n8  " ,2 ,8  ,INSTLAMBDA{ mc.Registers.b = byte(op); } };
	instructions[0x07] = { "RLCA      " ,1 ,4  ,INSTLAMBDA{ rlca(mc); } };

	instructions[0x08] = { "LD n16, SP ",2 ,8  ,INSTLAMBDA{ mc.Registers.sp = op; } };
	instructions[0x09] = { "ADD HL, BC" ,1 ,8  ,INSTLAMBDA{ add16(mc,mc.Registers.hl,mc.Registers.bc); } };	
	instructions[0x0A] = { "LD A,(BC)  ",1 ,8  ,INSTLAMBDA{ mc.Registers.a = mc.readByte(mc.Registers.bc);   } };
	instructions[0x0B] = { "DEC BC    " ,1 ,8  ,INSTLAMBDA{ mc.Registers.bc--; } };
	instructions[0x0C] = { "INC C     " ,1 ,4  ,INSTLAMBDA{ inc(mc.Registers.c); } };
	instructions[0x0D] = { "DEC C	  " ,1 ,4  ,INSTLAMBDA{ dec(mc.Registers.c); } };
	instructions[0x0E] = { "LD C, n8  " ,1 ,8  ,INSTLAMBDA{ mc.Registers.c = byte(op); } };
	instructions[0x0F] = { "RRCA      " ,1,4  ,INSTLAMBDA{ unimplemented(0xF); } };
	
	instructions[0x10] = { "STOP 0    " ,2 ,4  ,INSTLAMBDA{ unimplemented(op); } };
	instructions[0x11] = { "LD DE, n16" ,3 ,12  ,INSTLAMBDA{ mc.Registers.de = op; } };
	instructions[0x12] = { "LD (DE+), A",1 ,8  ,INSTLAMBDA{ unimplemented(); } };
	instructions[0x13] = { "INC DE    " ,1 ,8  ,INSTLAMBDA{ mc.Registers.de++; } };
	instructions[0x14] = { "INC D     " ,1 ,4  ,INSTLAMBDA{ inc(mc.Registers.d); } };
	instructions[0x15] = { "DEC D     " ,1 ,4  ,INSTLAMBDA{ dec(mc.Registers.d); } };
	instructions[0x16] = { "LD D, n8  " ,2 ,8  ,INSTLAMBDA{ mc.Registers.d = byte(op); } };
	instructions[0x17] = { "RLA       " ,1 ,4  ,INSTLAMBDA{ mc.Registers.d = byte(op); } };

	instructions[0x18] = { "JR n8     " ,2 ,12  ,INSTLAMBDA{ mc.Registers.pc += op; } };
	instructions[0x19] = { "ADD HL, DE" ,1 ,8  ,INSTLAMBDA{ add16(mc,mc.Registers.hl,mc.Registers.de); } };
	instructions[0x1A] = { "LD A,(DE) " ,1 ,8  ,INSTLAMBDA{ mc.Registers.a = mc.readByte(mc.Registers.de); } };
	instructions[0x1B] = { "DEC DE    " ,1 ,8  ,INSTLAMBDA{ mc.Registers.de--; } };
	instructions[0x1C] = { "INC E     " ,1 ,4  ,INSTLAMBDA{ inc(mc.Registers.e); } };
	instructions[0x1D] = { "DEC E     " ,1 ,4  ,INSTLAMBDA{ dec(mc.Registers.e); } };
	instructions[0x1E] = { "LD E, n8  " ,2 ,8  ,INSTLAMBDA{ mc.Registers.e = byte(op); } };
	instructions[0x1F] = { "RRA       " ,1 ,4  ,INSTLAMBDA{ unimplemented(); } };	
	
	instructions[0x20] = { "JR NZ, n8"  ,2 ,12  ,INSTLAMBDA{ if (!mc.GetFlag(Zero_Flag)) { mc.Registers.pc += op; }    } };
	instructions[0x21] = { "LD HL, n16" ,3 ,12  ,INSTLAMBDA{ mc.Registers.hl = op; } };
	instructions[0x22] = { "LD (HL+), A",1 ,8  ,INSTLAMBDA{ unimplemented(); } };
	instructions[0x23] = { "INC HL    " ,0 ,8  ,INSTLAMBDA{ mc.Registers.hl++; } };
	instructions[0x24] = { "INC H     " ,0 ,4  ,INSTLAMBDA{ inc(mc.Registers.h); } };
	instructions[0x25] = { "DEC H	  " ,0 ,4  ,INSTLAMBDA{ dec(mc.Registers.h); } };
	instructions[0x26] = { "LD H, n8  " ,2 ,8  ,INSTLAMBDA{ mc.Registers.h = byte(op); } };
	instructions[0x27] = { "DAA       " ,1 ,4  ,INSTLAMBDA{ unimplemented(); } };

	instructions[0x28] = { "JR Z, n8"  ,2 ,12  ,INSTLAMBDA{ if (mc.GetFlag(Zero_Flag)) { mc.Registers.pc += op; } } };
	instructions[0x29] = { "ADD HL, HL   " ,0 ,8  ,INSTLAMBDA{ add16(mc,mc.Registers.hl,mc.Registers.hl); } };
	instructions[0x2A] = { "LD A,(HL+)" ,1 ,8  ,INSTLAMBDA{ mc.Registers.a = mc.readByte(mc.Registers.hl); mc.Registers.hl++; } };
	instructions[0x2B] = { "DEC HL    " ,0 ,8  ,INSTLAMBDA{ mc.Registers.hl--; } };
	instructions[0x2C] = { "INC L     " ,0 ,4  ,INSTLAMBDA{ inc(mc.Registers.l); } };
	instructions[0x2D] = { "DEC L     " ,0 ,4  ,INSTLAMBDA{ dec(mc.Registers.l); } };
	instructions[0x2E] = { "LD L, n8  " ,2 ,8  ,INSTLAMBDA{ mc.Registers.l = byte(op); } };
	instructions[0x2F] = { "CPL       " ,1 ,4  ,INSTLAMBDA{ unimplemented(); } };

	instructions[0x30] = { "JR NC, n8"  ,2 ,12  ,INSTLAMBDA{ if (!mc.GetFlag(Carry_Flag)) { mc.Registers.pc += op; } } };
	instructions[0x31] = { "LD SP, n16" ,3 ,12  ,INSTLAMBDA{ mc.Registers.sp = op; } };
	instructions[0x32] = { "LD (HL-), A",1 ,8  ,INSTLAMBDA{ mc.writeByte(mc.Registers.a,mc.Registers.hl); mc.Registers.hl--; } };
	instructions[0x33] = { "INC SP    " ,1 ,8  ,INSTLAMBDA{ mc.Registers.sp++; } };
	instructions[0x34] = { "INC (HL)"   ,1 ,12  ,INSTLAMBDA{ unimplemented(); } };
	instructions[0x35] = { "INC (HL)"   ,1 ,12  ,INSTLAMBDA{ unimplemented(); } };
	instructions[0x36] = { "LD (HL), n8",2 ,12  ,INSTLAMBDA{ mc.writeByte(byte(op), mc.Registers.hl); } };
	instructions[0x37] = { "SCF       " ,1 ,4  ,INSTLAMBDA{ unimplemented(); } };

	instructions[0x38] = { "JR C, n8"  ,2 ,12  ,INSTLAMBDA{ if (mc.GetFlag(Carry_Flag)) { mc.Registers.pc += op; } } };
	instructions[0x39] = { "ADD HL, SP   " ,1 ,8  ,INSTLAMBDA{ add16(mc,mc.Registers.hl,mc.Registers.sp); } };
	instructions[0x3A] = { "LD A,(HL-)" ,1 ,8  ,INSTLAMBDA{ mc.Registers.a = mc.readByte(mc.Registers.hl); mc.Registers.hl--; } };
	instructions[0x3B] = { "DEC SP    " ,1 ,8  ,INSTLAMBDA{ mc.Registers.sp--; } };
	instructions[0x3C] = { "INC A	  " ,1 ,4  ,INSTLAMBDA{ inc(mc.Registers.a); } };
	instructions[0x3D] = { "DEC A	  " ,1 ,4  ,INSTLAMBDA{ dec(mc.Registers.a); } };
	instructions[0x3E] = { "LD A, n8  " ,2 ,8  ,INSTLAMBDA{ mc.Registers.a = byte(op); } };
	instructions[0x3F] = { "CCF       " ,1 ,4  ,INSTLAMBDA{ unimplemented(); } };
	
	instructions[0x40] = { "LD B, B  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.b = mc.Registers.b; } };
	instructions[0x41] = { "LD B, C  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.b = mc.Registers.c; } };
	instructions[0x42] = { "LD B, D  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.b = mc.Registers.d; } };
	instructions[0x43] = { "LD B, E  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.b = mc.Registers.e; } };
	instructions[0x44] = { "LD B, H  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.b = mc.Registers.h; } };
	instructions[0x45] = { "LD B, L  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.b = mc.Registers.l; } };
	instructions[0x46] = { "LD B, (HL)",1 ,8  ,INSTLAMBDA{ mc.Registers.b = mc.readByte(mc.Registers.hl); } };
	instructions[0x47] = { "LD B, A  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.b = mc.Registers.a; } };
	
	instructions[0x48] = { "LD C, B  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.c = mc.Registers.b; } };
	instructions[0x49] = { "LD C, C  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.c = mc.Registers.c; } };
	instructions[0x4A] = { "LD C, D  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.c = mc.Registers.d; } };
	instructions[0x4B] = { "LD C, E  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.c = mc.Registers.e; } };
	instructions[0x4C] = { "LD C, H  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.c = mc.Registers.h; } };
	instructions[0x4D] = { "LD C, L  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.c = mc.Registers.l; } };
	instructions[0x4E] = { "LD C, (HL)",1 ,8  ,INSTLAMBDA{ mc.Registers.c = mc.readByte(mc.Registers.hl);  } };
	instructions[0x4F] = { "LD C, A  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.c = mc.Registers.a; } };
	
	instructions[0x50] = { "LD D, B  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.d = mc.Registers.b; } };
	instructions[0x51] = { "LD D, C  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.d = mc.Registers.c; } };
	instructions[0x52] = { "LD D, D  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.d = mc.Registers.d; } };
	instructions[0x53] = { "LD D, E  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.d = mc.Registers.e; } };
	instructions[0x54] = { "LD D, H  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.d = mc.Registers.h; } };
	instructions[0x55] = { "LD D, L  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.d = mc.Registers.l; } };
	instructions[0x56] = { "LD D, (HL)",1 ,8  ,INSTLAMBDA{ mc.Registers.d = mc.readByte(mc.Registers.hl); } };
	instructions[0x57] = { "LD D, A  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.d = mc.Registers.a; } };
	
	instructions[0x58] = { "LD E, B  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.e = mc.Registers.b; } };
	instructions[0x59] = { "LD E, C  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.e = mc.Registers.c; } };
	instructions[0x5A] = { "LD E, D  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.e = mc.Registers.d; } };
	instructions[0x5B] = { "LD E, E  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.e = mc.Registers.e; } };
	instructions[0x5C] = { "LD E, H  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.e = mc.Registers.h; } };
	instructions[0x5D] = { "LD E, L  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.e = mc.Registers.l; } };
	instructions[0x5E] = { "LD E, (HL)",1 ,8  ,INSTLAMBDA{ mc.Registers.e = mc.readByte(mc.Registers.hl); } };
	instructions[0x5F] = { "LD E, A  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.e = mc.Registers.a; } };

	instructions[0x60] = { "LD H, B  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.h = mc.Registers.b; } };
	instructions[0x61] = { "LD H, C  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.h = mc.Registers.c; } };
	instructions[0x62] = { "LD H, D  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.h = mc.Registers.d; } };
	instructions[0x63] = { "LD H, E  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.h = mc.Registers.e; } };
	instructions[0x64] = { "LD H, H  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.h = mc.Registers.h; } };
	instructions[0x65] = { "LD H, L  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.h = mc.Registers.l; } };
	instructions[0x66] = { "LD H, (HL)",1 ,8  ,INSTLAMBDA{ mc.Registers.h = mc.readByte(mc.Registers.hl); } };
	instructions[0x67] = { "LD H, A  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.h = mc.Registers.a; } };
	
	instructions[0x68] = { "LD L, B  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.l = mc.Registers.b; } };
	instructions[0x69] = { "LD L, C  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.l = mc.Registers.c; } };
	instructions[0x6A] = { "LD L, D  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.l = mc.Registers.d; } };
	instructions[0x6B] = { "LD L, E  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.l = mc.Registers.e; } };
	instructions[0x6C] = { "LD L, H  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.l = mc.Registers.h; } };
	instructions[0x6D] = { "LD L, L  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.l = mc.Registers.l; } };
	instructions[0x6E] = { "LD L, (HL)",1 ,8  ,INSTLAMBDA{ mc.Registers.l = mc.readByte(mc.Registers.hl); } };
	instructions[0x6F] = { "LD L, A  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.l = mc.Registers.a; } };
	
	instructions[0x70] = { "LD (HL), B  " ,1 ,8  ,INSTLAMBDA{ mc.writeByte(mc.Registers.b, mc.Registers.hl); } };
	instructions[0x71] = { "LD (HL), C  " ,1 ,8  ,INSTLAMBDA{ mc.writeByte(mc.Registers.c, mc.Registers.hl); } };
	instructions[0x72] = { "LD (HL), D  " ,1 ,8  ,INSTLAMBDA{ mc.writeByte(mc.Registers.d, mc.Registers.hl); } };
	instructions[0x73] = { "LD (HL), E  " ,1 ,8  ,INSTLAMBDA{ mc.writeByte(mc.Registers.e, mc.Registers.hl); } };
	instructions[0x74] = { "LD (HL), H  " ,1 ,8  ,INSTLAMBDA{ mc.writeByte(mc.Registers.h, mc.Registers.hl); } };
	instructions[0x75] = { "LD (HL), L  " ,1 ,8  ,INSTLAMBDA{ mc.writeByte(mc.Registers.l, mc.Registers.hl); } };
	instructions[0x76] = { "HALT        " ,1 ,4  ,INSTLAMBDA{ unimplemented(0x76); } };
	instructions[0x77] = { "LD (HL), A  " ,1 ,8  ,INSTLAMBDA{ mc.writeByte(mc.Registers.a, mc.Registers.hl); } };

	instructions[0x78] = { "LD A, B  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.a = mc.Registers.b; } };
	instructions[0x79] = { "LD A, C  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.a = mc.Registers.c; } };
	instructions[0x7A] = { "LD A, D  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.a = mc.Registers.d; } };
	instructions[0x7B] = { "LD A, E  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.a = mc.Registers.e; } };
	instructions[0x7C] = { "LD A, H  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.a = mc.Registers.h; } };
	instructions[0x7D] = { "LD A, L  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.a = mc.Registers.l; } };
	instructions[0x7E] = { "LD A, (HL)",1 ,8  ,INSTLAMBDA{ mc.Registers.a = mc.readByte(mc.Registers.hl); } };
	instructions[0x7F] = { "LD A, A  " ,1 ,4  ,INSTLAMBDA{ mc.Registers.a = mc.Registers.a; } };

	instructions[0x80] = { "ADD B" ,1 ,4  ,INSTLAMBDA{ add(mc,mc.Registers.b); } };
	instructions[0x81] = { "ADD C" ,1 ,4  ,INSTLAMBDA{ add(mc,mc.Registers.c); } };
	instructions[0x82] = { "ADD D" ,1 ,4  ,INSTLAMBDA{ add(mc,mc.Registers.d); } };
	instructions[0x83] = { "ADD E" ,1 ,4  ,INSTLAMBDA{ add(mc,mc.Registers.e); } };
	instructions[0x84] = { "ADD H" ,1 ,4  ,INSTLAMBDA{ add(mc,mc.Registers.h); } };
	instructions[0x85] = { "ADD L" ,1 ,4  ,INSTLAMBDA{ add(mc,mc.Registers.l); } };
	instructions[0x86] = { "ADD (HL)",1 ,8,INSTLAMBDA{ add(mc,mc.readByte(mc.Registers.hl)); } };
	instructions[0x87] = { "ADD A" ,1 ,4  ,INSTLAMBDA{ add(mc,mc.Registers.a); } };
	
	instructions[0x88] = { "ADC B" ,1 ,4  ,INSTLAMBDA{ adc(mc,mc.Registers.b); } };
	instructions[0x89] = { "ADC C" ,1 ,4  ,INSTLAMBDA{ adc(mc,mc.Registers.c); } };
	instructions[0x8A] = { "ADC D" ,1 ,4  ,INSTLAMBDA{ adc(mc,mc.Registers.d); } };
	instructions[0x8B] = { "ADC E" ,1 ,4  ,INSTLAMBDA{ adc(mc,mc.Registers.e); } };
	instructions[0x8C] = { "ADC H" ,1 ,4  ,INSTLAMBDA{ adc(mc,mc.Registers.h); } };
	instructions[0x8D] = { "ADC L" ,1 ,4  ,INSTLAMBDA{ adc(mc,mc.Registers.l); } };
	instructions[0x8E] = { "ADC (HL)" ,1,8,INSTLAMBDA{ adc(mc,mc.readByte(mc.Registers.hl)); } };
	instructions[0x8F] = { "ADC A" ,1 ,4  ,INSTLAMBDA{ adc(mc,mc.Registers.a); } };

	instructions[0x90] = { "SUB B" ,1 ,4  ,INSTLAMBDA{ sub(mc,mc.Registers.b); } };
	instructions[0x91] = { "SUB C" ,1 ,4  ,INSTLAMBDA{ sub(mc,mc.Registers.c); } };
	instructions[0x92] = { "SUB D" ,1 ,4  ,INSTLAMBDA{ sub(mc,mc.Registers.d); } };
	instructions[0x93] = { "SUB E" ,1 ,4  ,INSTLAMBDA{ sub(mc,mc.Registers.e); } };
	instructions[0x94] = { "SUB H" ,1 ,4  ,INSTLAMBDA{ sub(mc,mc.Registers.h); } };
	instructions[0x95] = { "SUB L" ,1 ,4  ,INSTLAMBDA{ sub(mc,mc.Registers.l); } };
	instructions[0x96] = { "SUB (HL)",1 ,8,INSTLAMBDA{ sub(mc,mc.readByte(mc.Registers.hl)); } };
	instructions[0x97] = { "SUB A" ,1 ,4  ,INSTLAMBDA{ sub(mc,mc.Registers.a); } };
				   
	instructions[0x98] = { "SBC B" ,1 ,4  ,INSTLAMBDA{ sbc(mc,mc.Registers.b); } };
	instructions[0x99] = { "SBC C" ,1 ,4  ,INSTLAMBDA{ sbc(mc,mc.Registers.c); } };
	instructions[0x9A] = { "SBC D" ,1 ,4  ,INSTLAMBDA{ sbc(mc,mc.Registers.d); } };
	instructions[0x9B] = { "SBC E" ,1 ,4  ,INSTLAMBDA{ sbc(mc,mc.Registers.e); } };
	instructions[0x9C] = { "SBC H" ,1 ,4  ,INSTLAMBDA{ sbc(mc,mc.Registers.h); } };
	instructions[0x9D] = { "SBC L" ,1 ,4  ,INSTLAMBDA{ sbc(mc,mc.Registers.l); } };
	instructions[0x9E] = { "SBC (HL)" ,1,8,INSTLAMBDA{ sbc(mc,mc.readByte(mc.Registers.hl)); } };
	instructions[0x9F] = { "SBC A" ,1 ,4  ,INSTLAMBDA{ sbc(mc,mc.Registers.a); } };

	instructions[0xA0] = { "AND B" ,1 ,4  ,INSTLAMBDA{ _and(mc,mc.Registers.b); } };
	instructions[0xA1] = { "AND C" ,1 ,4  ,INSTLAMBDA{ _and(mc,mc.Registers.c); } };
	instructions[0xA2] = { "AND D" ,1 ,4  ,INSTLAMBDA{ _and(mc,mc.Registers.d); } };
	instructions[0xA3] = { "AND E" ,1 ,4  ,INSTLAMBDA{ _and(mc,mc.Registers.e); } };
	instructions[0xA4] = { "AND H" ,1 ,4  ,INSTLAMBDA{ _and(mc,mc.Registers.h); } };
	instructions[0xA5] = { "AND L" ,1 ,4  ,INSTLAMBDA{ _and(mc,mc.Registers.l); } };
	instructions[0xA6] = { "AND (HL)",1 ,8,INSTLAMBDA{ _and(mc,mc.readByte(mc.Registers.hl)); } };
	instructions[0xA7] = { "AND A" ,1 ,4  ,INSTLAMBDA{ _and(mc,mc.Registers.a); } };

	instructions[0xA8] = { "XOR B" ,1 ,4  ,INSTLAMBDA{_xor(mc,mc.Registers.b); } };
	instructions[0xA9] = { "XOR C" ,1 ,4  ,INSTLAMBDA{_xor(mc,mc.Registers.c); } };
	instructions[0xAA] = { "XOR D" ,1 ,4  ,INSTLAMBDA{_xor(mc,mc.Registers.d); } };
	instructions[0xAB] = { "XOR E" ,1 ,4  ,INSTLAMBDA{_xor(mc,mc.Registers.e); } };
	instructions[0xAC] = { "XOR H" ,1 ,4  ,INSTLAMBDA{_xor(mc,mc.Registers.h); } };
	instructions[0xAD] = { "XOR L" ,1 ,4  ,INSTLAMBDA{_xor(mc,mc.Registers.l); } };
	instructions[0xAE] = { "XOR (HL)" ,1,8,INSTLAMBDA{ _xor(mc,mc.readByte(mc.Registers.hl)); } };
	instructions[0xAF] = { "XOR A" ,1 ,4  ,INSTLAMBDA{_xor(mc,mc.Registers.a); } };

	instructions[0xB0] = { "OR B" ,1 ,4  ,INSTLAMBDA{ _or(mc,mc.Registers.b); } };
	instructions[0xB1] = { "OR C" ,1 ,4  ,INSTLAMBDA{ _or(mc,mc.Registers.c); } };
	instructions[0xB2] = { "OR D" ,1 ,4  ,INSTLAMBDA{ _or(mc,mc.Registers.d); } };
	instructions[0xB3] = { "OR E" ,1 ,4  ,INSTLAMBDA{ _or(mc,mc.Registers.e); } };
	instructions[0xB4] = { "OR H" ,1 ,4  ,INSTLAMBDA{ _or(mc,mc.Registers.h); } };
	instructions[0xB5] = { "OR L" ,1 ,4  ,INSTLAMBDA{ _or(mc,mc.Registers.l); } };
	instructions[0xB6] = { "OR (HL)",1 ,8,INSTLAMBDA{ _or(mc,mc.readByte(mc.Registers.hl)); } };
	instructions[0xB7] = { "OR A" ,1 ,4  ,INSTLAMBDA{ _or(mc,mc.Registers.a); } };

	instructions[0xB8] = { "CP B" ,1 ,4  ,INSTLAMBDA{ cp(mc,mc.Registers.b); } };
	instructions[0xB9] = { "CP C" ,1 ,4  ,INSTLAMBDA{ cp(mc,mc.Registers.c); } };
	instructions[0xBA] = { "CP D" ,1 ,4  ,INSTLAMBDA{ cp(mc,mc.Registers.d); } };
	instructions[0xBB] = { "CP E" ,1 ,4  ,INSTLAMBDA{ cp(mc,mc.Registers.e); } };
	instructions[0xBC] = { "CP H" ,1 ,4  ,INSTLAMBDA{ cp(mc,mc.Registers.h); } };
	instructions[0xBD] = { "CP L" ,1 ,4  ,INSTLAMBDA{ cp(mc,mc.Registers.l); } };
	instructions[0xBE] = { "CP (HL)" ,1,8,INSTLAMBDA{ cp(mc,mc.readByte(mc.Registers.hl)); } };
	instructions[0xBF] = { "CP A" ,1 ,4  ,INSTLAMBDA{ cp(mc,mc.Registers.a); } };
	
	instructions[0xC0] = { "RET NZ   " ,1 ,20  ,INSTLAMBDA{ if (!mc.GetFlag(Zero_Flag)) { ret(mc); }  } };
	instructions[0xC1] = { "POP BC   " ,1 ,20  ,INSTLAMBDA{ pop(mc,mc.Registers.bc); } };
	instructions[0xC2] = { "JP NZ, a16 " ,3 ,16  ,INSTLAMBDA{ unimplemented(0xC2); } };
	instructions[0xC3] = { "JP a16 " ,3 ,16  ,INSTLAMBDA{ mc.Registers.pc = op; } };
	instructions[0xC4] = { "CALL NZ ,n16   " ,3 ,24  ,INSTLAMBDA{ if (!mc.GetFlag(Zero_Flag)) { call(mc,op); } } };
	instructions[0xC5] = { "PUSH BC ", 1, 16, INSTLAMBDA{ push(mc,mc.Registers.bc); } };
	instructions[0xC6] = { "ADD N8",2 ,8,INSTLAMBDA{ add(mc,op); } };
	instructions[0xC7] = { "RST 00H ", 1 ,16  ,INSTLAMBDA{ rst(mc,0x00); } };

	instructions[0xC8] = { "RET Z   " ,1 ,20  ,INSTLAMBDA{ if (mc.GetFlag(Zero_Flag)) { ret(mc); } } };
	instructions[0xC9] = { "RET     " ,1 ,16  ,INSTLAMBDA{  ret(mc);  } };
	instructions[0xCA] = { "JP Z, a16 " ,3 ,16  ,INSTLAMBDA{ unimplemented(); } };
	instructions[0xCB] = { "PREFIX CB " ,1 ,4  ,INSTLAMBDA{ /*unimplemented();*/ } };
	instructions[0xCC] = { "CALL Z ,n16   " ,3 ,24  ,INSTLAMBDA{ if (mc.GetFlag(Zero_Flag)) { call(mc,op); } } };
	instructions[0xCD] = { "CALL n16   " ,3 ,12  ,INSTLAMBDA{ call(mc,op); } };
	instructions[0xCE] = { "ADC A, n8" ,2 ,8  ,INSTLAMBDA{ adc(mc,op); } };
	instructions[0xCF] = { "RST 08H ", 1 ,16  ,INSTLAMBDA{ rst(mc,0x08); } };

	instructions[0xD0] = { "RET NC   " ,1 ,20  ,INSTLAMBDA{ if (!mc.GetFlag(Carry_Flag)) { ret(mc); } } };
	instructions[0xD1] = { "POP DE   " ,1 ,20  ,INSTLAMBDA{ pop(mc,mc.Registers.de); } };
	instructions[0xD2] = { "JP NC, a16 " ,3 ,16  ,INSTLAMBDA{ unimplemented(); } };
	instructions[0xD3] = { "LITERALLYNOTHING " ,1,0  ,INSTLAMBDA{ } };
	instructions[0xD4] = { "CALL NC ,n16   " ,3 ,24  ,INSTLAMBDA{ if (!mc.GetFlag(Carry_Flag)) { call(mc,op); } } };
	instructions[0xD5] = { "PUSH DE ", 1, 16, INSTLAMBDA{ push(mc,mc.Registers.de); } };
	instructions[0xD6] = { "SUB N8",2 ,8,INSTLAMBDA{ sub(mc,op); } };
	instructions[0xD7] = { "RST 10H ", 1 ,16  ,INSTLAMBDA{ rst(mc,0x10); } };

	instructions[0xD8] = { "RET C   " ,1 ,20  ,INSTLAMBDA{ if (mc.GetFlag(Carry_Flag)) { ret(mc); } } };
	instructions[0xD9] = { "RETI    " ,1 ,16  ,INSTLAMBDA{ ret(mc); mc.EnableInterrupts(); } };
	instructions[0xDA] = { "JP C,n16" ,1 ,16  ,INSTLAMBDA{ unimplemented(); } };
	instructions[0xDB] = { "LITERALLYNOTHING " ,1,0  ,INSTLAMBDA{} };
	instructions[0xDC] = { "CALL C ,n16   " ,3 ,24  ,INSTLAMBDA{ if (mc.GetFlag(Carry_Flag)) { call(mc,op); } } };
	instructions[0xDD] = { "LITERALLYNOTHING " ,1,0  ,INSTLAMBDA{} };
	instructions[0xDE] = { "SBC A, N8",2 ,8,INSTLAMBDA{ unimplemented(); } };
	instructions[0xDF] = { "RST 18H ", 1 ,16  ,INSTLAMBDA{ rst(mc,0x18); } };

	instructions[0xE0] = { "LDH (n), A ",2 ,12  ,INSTLAMBDA{ mc.writeByte(mc.Registers.a,0xFF00 + op); } };
	instructions[0xE1] = { "POP HL   " ,1 ,20  ,INSTLAMBDA{ pop(mc,mc.Registers.hl); } };
	instructions[0xE2] = { "LD (C), A", 2 ,8  ,INSTLAMBDA{ mc.writeByte(mc.Registers.a,mc.Registers.c + 0xFF00); } };
	instructions[0xE3] = { "LITERALLYNOTHING " ,1,0  ,INSTLAMBDA{} };
	instructions[0xE4] = { "LITERALLYNOTHING " ,1,0  ,INSTLAMBDA{} };
	instructions[0xE5] = { "PUSH HL ", 1, 16, INSTLAMBDA{ push(mc,mc.Registers.hl); } };
	instructions[0xE6] = { "AND N8",2 ,8,INSTLAMBDA{ _and(mc,op); } };
	instructions[0xE7] = { "RST 20H ", 1 ,16  ,INSTLAMBDA{ rst(mc,0x20); } };

	instructions[0xE8] = { "ADD HL, BC   " ,1 ,16  ,INSTLAMBDA{ add16(mc,mc.Registers.hl,unsigned short(op & 0x00FF)); } };
	instructions[0xE9] = { "JP (HL)" ,1 ,4  ,INSTLAMBDA{ mc.Registers.pc = mc.readShort(mc.Registers.hl); } };
	instructions[0xEA] = { "LD (n16), A ",3 ,16  ,INSTLAMBDA{ mc.writeByte(mc.Registers.a,op); } };
	instructions[0xEB] = { "LITERALLYNOTHING " ,1,0  ,INSTLAMBDA{} };
	instructions[0xEC] = { "LITERALLYNOTHING " ,1,0  ,INSTLAMBDA{} };
	instructions[0xED] = { "LITERALLYNOTHING " ,1,0  ,INSTLAMBDA{} };
	instructions[0xEE] = { "XOR  N8",2 ,8,INSTLAMBDA{ _xor(mc,op); } };
	instructions[0xEF] = { "RST 28H ", 1 ,16  ,INSTLAMBDA{ rst(mc,0x28); } };

	instructions[0xF0] = { "LDH A, (n8) ",2 ,12  ,INSTLAMBDA{ mc.Registers.a = mc.readByte(0xFF00 + op); } };
	instructions[0xF1] = { "POP AF   " ,1 ,20  ,INSTLAMBDA{ pop(mc,mc.Registers.af); } };
	instructions[0xF2] = { "LD A, (C)", 2 ,8  ,INSTLAMBDA{ mc.Registers.a = mc.readByte(0xFF +op); } };
	instructions[0xF3] = { "DI ", 1 ,4  ,INSTLAMBDA{ mc.DisableInterrupts();  } };
	instructions[0xF4] = { "LITERALLYNOTHING " ,1,0  ,INSTLAMBDA{} };
	instructions[0xF5] = { "PUSH AF ", 1, 16, INSTLAMBDA{ push(mc,mc.Registers.af); } };
	instructions[0xF6] = { "OR N8",2 ,8,INSTLAMBDA{ _or(mc,op); } };
	instructions[0xF7] = { "RST 30H ", 1 ,16  ,INSTLAMBDA{ rst(mc,0x30); } };

	instructions[0xF8] = { "LD HL, sp + r8", 2 ,12  ,INSTLAMBDA{ mc.Registers.hl = mc.Registers.sp + op; } };
	instructions[0xF9] = { "LD SP, HL", 1 ,8  ,INSTLAMBDA{ mc.Registers.sp = mc.Registers.hl; } };
	instructions[0xFA] = { "LD A, (n16)", 3 ,16  ,INSTLAMBDA{ mc.Registers.a = mc.readByte(op); } };
	instructions[0xFB] = { "EI ", 1 ,4  ,INSTLAMBDA{ mc.EnableInterrupts();  } };
	instructions[0xFC] = { "LITERALLYNOTHING " ,1,0  ,INSTLAMBDA{} };
	instructions[0xFD] = { "LITERALLYNOTHING " ,1,0  ,INSTLAMBDA{} };
	instructions[0xFE] = { "CP N8",2 ,8,INSTLAMBDA{ cp(mc, op); } };
	instructions[0xFF] = { "RST 38H ", 1 ,16  ,INSTLAMBDA{ rst(mc,0x38); } };
}

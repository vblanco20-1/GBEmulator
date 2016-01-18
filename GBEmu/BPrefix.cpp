#include "BPrefix.h"

void cbUnimplemented()
{

}
static uint8_t swap(Gameboy&mc, uint8_t val)
{
	val = ((val & 0xf) << 4) | ((val & 0xf0) >> 4);

	mc.ClearFlags(Subtract_Flag | HalfCarry_Flag | Carry_Flag);

	if (!val) mc.SetFlags(Zero_Flag);

	return val;
}
void checkbit(Gameboy&machine, uint8_t reg, uint8_t bit)
{
	machine.SetFlags(HalfCarry_Flag);
	machine.ClearFlags(Subtract_Flag | Zero_Flag);

	if ((reg &(1 << bit)) == 0)
	{
		machine.SetFlags(Zero_Flag);
	}

}
static void resbit(uint8_t & reg, uint8_t bit)
{
	uint8_t v = 1 << bit;
	
	reg &= ~v;
}
static void resbit_mem(Gameboy&mc,uint16_t addr, uint8_t bit)
{
	uint8_t val = mc.readByte(addr);
	resbit(val,bit);
	mc.writeByte(val, addr);
}
static void setbit(uint8_t & reg, uint8_t bit)
{
	uint8_t v = 1 << bit;

	reg |= v;
}

static void setbit_mem(Gameboy&mc, uint16_t addr, uint8_t bit)
{
	uint8_t val = mc.readByte(addr);
	setbit(val,bit);
	mc.writeByte(val, addr);
}
static uint8_t rlc(Gameboy&mc, uint8_t value) {
	int carry = (value & 0x80) >> 7;

	if (value & 0x80)  mc.SetFlags(Carry_Flag);
	else mc.ClearFlags(Carry_Flag);

	value <<= 1;
	value += carry;

	if (value) mc.ClearFlags(Zero_Flag);
	else mc.SetFlags(Zero_Flag);

	mc.ClearFlags(Subtract_Flag | HalfCarry_Flag);

	return value;
	
}
static uint8_t rrc(Gameboy&mc, uint8_t value) {

	int carry = value & 0x01;

	value >>= 1;

	if (carry) {
		mc.SetFlags(Carry_Flag);
		value |= 0x80;
	}
	else mc.ClearFlags(Carry_Flag);

	if (value) mc.ClearFlags(Zero_Flag);
	else mc.SetFlags(Zero_Flag);

	mc.ClearFlags(Subtract_Flag | HalfCarry_Flag);

	return value;
}
static uint8_t srl(Gameboy&mc, uint8_t value) {

	if (value & 1)
	{
		mc.SetFlags(Carry_Flag);
	}
	else
	{
		mc.ClearFlags(Carry_Flag);
	}

	value >>= 1;

	if (value) mc.ClearFlags(Zero_Flag);
	else mc.SetFlags(Zero_Flag);

	mc.ClearFlags(Subtract_Flag | HalfCarry_Flag);

	return value;
}
static uint8_t sra(Gameboy&mc, uint8_t value) {
	if (value & 0x01) mc.SetFlags(Carry_Flag);
	else mc.ClearFlags(Carry_Flag);

	value = (value & 0x80) | (value >> 1);

	if (value) mc.ClearFlags(Zero_Flag);
	else mc.SetFlags(Zero_Flag);


	mc.ClearFlags(Subtract_Flag | HalfCarry_Flag);


	return value;
	
}
static uint8_t sla(Gameboy&mc, uint8_t value) {


	if (value & (1 << 7))
	{
		mc.SetFlags(Carry_Flag);
	}
	else
	{
		mc.ClearFlags(Carry_Flag);
	}

	value <<= 1;

	if (value) mc.ClearFlags(Zero_Flag);
	else mc.SetFlags(Zero_Flag);

	mc.ClearFlags(Subtract_Flag | HalfCarry_Flag);

	return value;
}


static uint8_t rl(Gameboy&mc, uint8_t value) {
	//cout << "RL " << bitset<8>(value) << "c="<< mc.GetFlag(Carry_Flag);

	bool bit7 = (value & (1 << 7));

	int carry = mc.GetFlag(Carry_Flag) ? 1 : 0;

	value <<= 1;
	value |= carry;
	//value += carry;

	if (bit7) mc.SetFlags(Carry_Flag);
	else mc.ClearFlags(Carry_Flag);

	//int oldbit7 = getbit(value, 7);

	//value &= oldbit7;
	if (value) mc.ClearFlags(Zero_Flag);
	else mc.SetFlags(Zero_Flag);

	mc.ClearFlags(Subtract_Flag | HalfCarry_Flag);

	//cout << "      RL result " << bitset<8>(value) <<  " carry = " << mc.GetFlag(Carry_Flag) << endl;
	return value;
}

static uint8_t rr(Gameboy&mc, uint8_t value) {

	bool bit0 = (value & 0x01);

	value >>= 1;
	if (mc.GetFlag(Carry_Flag)) value |= 0x80;

	if (bit0) mc.SetFlags(Carry_Flag);
	else mc.ClearFlags(Carry_Flag);


	if (value) mc.ClearFlags(Zero_Flag);
	else mc.SetFlags(Zero_Flag);

	mc.ClearFlags(Subtract_Flag | HalfCarry_Flag);

	return value;
}


void CBPrefix::initFunctionArray()
{
	instructions = std::vector<CBInst>(256, { "UNIMPLEMENTED" , [](Gameboy&mc) {cbUnimplemented(); } });

	instructions[0x00] = { "RLC B    "  , [](Gameboy&mc) {mc.Registers.b=rlc(mc,mc.Registers.b); } };
	instructions[0x01] = { "RLC C    "	, [](Gameboy&mc) {mc.Registers.c=rlc(mc,mc.Registers.c); } };
	instructions[0x02] = { "RLC D    "	, [](Gameboy&mc) {mc.Registers.d=rlc(mc,mc.Registers.d); } };
	instructions[0x03] = { "RLC E    "	, [](Gameboy&mc) {mc.Registers.e=rlc(mc,mc.Registers.e); } };
	instructions[0x04] = { "RLC H    "	, [](Gameboy&mc) {mc.Registers.h=rlc(mc,mc.Registers.h); } };
	instructions[0x05] = { "RLC L    "	, [](Gameboy&mc) {mc.Registers.l=rlc(mc,mc.Registers.l); } };
	instructions[0x06] = { "RLC (HL) "	, [](Gameboy&mc) {mc.writeByte(  rlc(mc,mc.readByte(mc.Registers.hl)),mc.Registers.hl)  ; } };
	instructions[0x07] = { "RLC A    "	, [](Gameboy&mc) {mc.Registers.a=rlc(mc,mc.Registers.a ); } };

	instructions[0x08] = { "RRC B    "  , [](Gameboy&mc) {mc.Registers.b = rrc(mc,mc.Registers.b); } };
	instructions[0x09] = { "RRC C    "	, [](Gameboy&mc) {mc.Registers.c = rrc(mc,mc.Registers.c); } };
	instructions[0x0A] = { "RRC D    "	, [](Gameboy&mc) {mc.Registers.d = rrc(mc,mc.Registers.d); } };
	instructions[0x0B] = { "RRC E    "	, [](Gameboy&mc) {mc.Registers.e = rrc(mc,mc.Registers.e); } };
	instructions[0x0C] = { "RRC H    "	, [](Gameboy&mc) {mc.Registers.h = rrc(mc,mc.Registers.h); } };
	instructions[0x0D] = { "RRC L    "	, [](Gameboy&mc) {mc.Registers.l = rrc(mc,mc.Registers.l); } };
	instructions[0x0E] = { "RRC (HL) "	, [](Gameboy&mc) {mc.writeByte(    rrc(mc,mc.readByte(mc.Registers.hl)),mc.Registers.hl); } };
	instructions[0x0F] = { "RRC A    "	, [](Gameboy&mc) {mc.Registers.a = rrc(mc,mc.Registers.a); } };

	instructions[0x10] = { "RL B    "  ,  [](Gameboy&mc)  {mc.Registers.b = rl(mc,mc.Registers.b); } };
	instructions[0x11] = { "RL C    "	, [](Gameboy&mc) {mc.Registers.c = rl(mc,mc.Registers.c); } };
	instructions[0x12] = { "RL D    "	, [](Gameboy&mc) {mc.Registers.d = rl(mc,mc.Registers.d); } };
	instructions[0x13] = { "RL E    "	, [](Gameboy&mc) {mc.Registers.e = rl(mc,mc.Registers.e); } };
	instructions[0x14] = { "RL H    "	, [](Gameboy&mc) {mc.Registers.h = rl(mc,mc.Registers.h); } };
	instructions[0x15] = { "RL L    "	, [](Gameboy&mc) {mc.Registers.l = rl(mc,mc.Registers.l); } };
	instructions[0x16] = { "RL (HL) "	, [](Gameboy&mc) {mc.writeByte(rl(mc,mc.readByte(mc.Registers.hl)),mc.Registers.hl); } };
	instructions[0x17] = { "RL A    "	, [](Gameboy&mc) {mc.Registers.a = rl(mc,mc.Registers.a); } };
	
	instructions[0x18] = { "RR B    "  ,  [](Gameboy&mc) {mc.Registers.b = rr(mc,mc.Registers.b); } };
	instructions[0x19] = { "RR C    "	, [](Gameboy&mc) {mc.Registers.c = rr(mc,mc.Registers.c); } };
	instructions[0x1A] = { "RR D    "	, [](Gameboy&mc) {mc.Registers.d = rr(mc,mc.Registers.d); } };
	instructions[0x1B] = { "RR E    "	, [](Gameboy&mc) {mc.Registers.e = rr(mc,mc.Registers.e); } };
	instructions[0x1C] = { "RR H    "	, [](Gameboy&mc) {mc.Registers.h = rr(mc,mc.Registers.h); } };
	instructions[0x1D] = { "RR L    "	, [](Gameboy&mc) {mc.Registers.l = rr(mc,mc.Registers.l); } };
	instructions[0x1E] = { "RR (HL) "	, [](Gameboy&mc) {mc.writeByte(rr(mc,mc.readByte(mc.Registers.hl)),mc.Registers.hl); } };
	instructions[0x1F] = { "RR A    "	, [](Gameboy&mc) {mc.Registers.a = rr(mc,mc.Registers.a); } };

	instructions[0x20] = { "SLA B    "  , [](Gameboy&mc) {mc.Registers.b =sla(mc,mc.Registers.b); } };
	instructions[0x21] = { "SLA C    "	, [](Gameboy&mc) {mc.Registers.c =sla(mc,mc.Registers.c); } };
	instructions[0x22] = { "SLA D    "	, [](Gameboy&mc) {mc.Registers.d =sla(mc,mc.Registers.d); } };
	instructions[0x23] = { "SLA E    "	, [](Gameboy&mc) {mc.Registers.e =sla(mc,mc.Registers.e); } };
	instructions[0x24] = { "SLA H    "	, [](Gameboy&mc) {mc.Registers.h =sla(mc,mc.Registers.h); } };
	instructions[0x25] = { "SLA L    "	, [](Gameboy&mc) {mc.Registers.l =sla(mc,mc.Registers.l); } };
	instructions[0x26] = { "SLA (HL) "	, [](Gameboy&mc) {mc.writeByte(sla(mc,mc.readByte(mc.Registers.hl)),mc.Registers.hl); } };
	instructions[0x27] = { "SLA A    "	, [](Gameboy&mc) {mc.Registers.a = sla(mc,mc.Registers.a); } };
				   
	instructions[0x28] = { "SRA B    "  , [](Gameboy&mc) {mc.Registers.b = sra(mc,mc.Registers.b); } };
	instructions[0x29] = { "SRA C    "	, [](Gameboy&mc) {mc.Registers.c = sra(mc,mc.Registers.c); } };
	instructions[0x2A] = { "SRA D    "	, [](Gameboy&mc) {mc.Registers.d = sra(mc,mc.Registers.d); } };
	instructions[0x2B] = { "SRA E    "	, [](Gameboy&mc) {mc.Registers.e = sra(mc,mc.Registers.e); } };
	instructions[0x2C] = { "SRA H    "	, [](Gameboy&mc) {mc.Registers.h = sra(mc,mc.Registers.h); } };
	instructions[0x2D] = { "SRA L    "	, [](Gameboy&mc) {mc.Registers.l = sra(mc,mc.Registers.l); } };
	instructions[0x2E] = { "SRA (HL) "	, [](Gameboy&mc) {mc.writeByte(sra(mc,mc.readByte(mc.Registers.hl)),mc.Registers.hl); } };
	instructions[0x2F] = { "SRA A    "	, [](Gameboy&mc) {mc.Registers.a = sra(mc,mc.Registers.a); } };

	instructions[0x30] = { "SWAP B    " , [](Gameboy&mc) {mc.Registers.b = swap(mc,mc.Registers.b); } };
	instructions[0x31] = { "SWAP C    "	, [](Gameboy&mc) {mc.Registers.c = swap(mc,mc.Registers.c); } };
	instructions[0x32] = { "SWAP D    "	, [](Gameboy&mc) {mc.Registers.d = swap(mc,mc.Registers.d); } };
	instructions[0x33] = { "SWAP E    "	, [](Gameboy&mc) {mc.Registers.e = swap(mc,mc.Registers.e); } };
	instructions[0x34] = { "SWAP H    "	, [](Gameboy&mc) {mc.Registers.h = swap(mc,mc.Registers.h); } };
	instructions[0x35] = { "SWAP L    "	, [](Gameboy&mc) {mc.Registers.l = swap(mc,mc.Registers.l); } };
	instructions[0x36] = { "SWAP (HL) "	, [](Gameboy&mc) {mc.writeByte(swap(mc,mc.readByte(mc.Registers.hl)),mc.Registers.hl); } };
	instructions[0x37] = { "SWAP A    "	, [](Gameboy&mc) {mc.Registers.a = swap(mc,mc.Registers.a); } };
	
	instructions[0x38] = { "SRL B    "  , [](Gameboy&mc) {mc.Registers.b = srl(mc,mc.Registers.b); } };
	instructions[0x39] = { "SRL C    "	, [](Gameboy&mc) {mc.Registers.c = srl(mc,mc.Registers.c); } };
	instructions[0x3A] = { "SRL D    "	, [](Gameboy&mc) {mc.Registers.d = srl(mc,mc.Registers.d); } };
	instructions[0x3B] = { "SRL E    "	, [](Gameboy&mc) {mc.Registers.e = srl(mc,mc.Registers.e); } };
	instructions[0x3C] = { "SRL H    "	, [](Gameboy&mc) {mc.Registers.h = srl(mc,mc.Registers.h); } };
	instructions[0x3D] = { "SRL L    "	, [](Gameboy&mc) {mc.Registers.l = srl(mc,mc.Registers.l); } };
	instructions[0x3E] = { "SRL (HL) "	, [](Gameboy&mc) {mc.writeByte(srl(mc,mc.readByte(mc.Registers.hl)),mc.Registers.hl); } };
	instructions[0x3F] = { "SRL A    "	, [](Gameboy&mc) {mc.Registers.a = srl(mc,mc.Registers.a); } };

	instructions[0x40] = { "BIT 0 B    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.b,0); } };
	instructions[0x41] = { "BIT 0 C    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.c,0); } };
	instructions[0x42] = { "BIT 0 D    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.d,0); } };
	instructions[0x43] = { "BIT 0 E    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.e,0); } };
	instructions[0x44] = { "BIT 0 H    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.h,0); } };
	instructions[0x45] = { "BIT 0 L    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.l,0); } };
	instructions[0x46] = { "BIT 0 (HL) ", [](Gameboy&mc) {checkbit(mc,mc.readByte(mc.Registers.hl),0); } };
	instructions[0x47] = { "BIT 0 A    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.a,0); } };
	
	instructions[0x48] = { "BIT 1 B    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.b,1); } };
	instructions[0x49] = { "BIT 1 C    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.c,1); } };
	instructions[0x4A] = { "BIT 1 D    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.d,1); } };
	instructions[0x4B] = { "BIT 1 E    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.e,1); } };
	instructions[0x4C] = { "BIT 1 H    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.h,1); } };
	instructions[0x4D] = { "BIT 1 L    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.l,1); } };
	instructions[0x4E] = { "BIT 1 (HL) ", [](Gameboy&mc) {checkbit(mc,mc.readByte(mc.Registers.hl),1); } };
	instructions[0x4F] = { "BIT 1 A    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.a,1); } };

	instructions[0x50] = { "BIT 2 B    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.b,2); } };
	instructions[0x51] = { "BIT 2 C    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.c,2); } };
	instructions[0x52] = { "BIT 2 D    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.d,2); } };
	instructions[0x53] = { "BIT 2 E    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.e,2); } };
	instructions[0x54] = { "BIT 2 H    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.h,2); } };
	instructions[0x55] = { "BIT 2 L    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.l,2); } };
	instructions[0x56] = { "BIT 2 (HL) ", [](Gameboy&mc) {checkbit(mc,mc.readByte(mc.Registers.hl),2); } };
	instructions[0x57] = { "BIT 2 A    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.a,2); } };
	
	instructions[0x58] = { "BIT 3 B    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.b,3); } };
	instructions[0x59] = { "BIT 3 C    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.c,3); } };
	instructions[0x5A] = { "BIT 3 D    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.d,3); } };
	instructions[0x5B] = { "BIT 3 E    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.e,3); } };
	instructions[0x5C] = { "BIT 3 H    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.h,3); } };
	instructions[0x5D] = { "BIT 3 L    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.l,3); } };
	instructions[0x5E] = { "BIT 3 (HL) ", [](Gameboy&mc) {checkbit(mc,mc.readByte(mc.Registers.hl),3); } };
	instructions[0x5F] = { "BIT 3 A    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.a,3); } };

	instructions[0x60] = { "BIT 4 B    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.b,4); } };
	instructions[0x61] = { "BIT 4 C    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.c,4); } };
	instructions[0x62] = { "BIT 4 D    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.d,4); } };
	instructions[0x63] = { "BIT 4 E    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.e,4); } };
	instructions[0x64] = { "BIT 4 H    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.h,4); } };
	instructions[0x65] = { "BIT 4 L    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.l,4); } };
	instructions[0x66] = { "BIT 4 (HL) ", [](Gameboy&mc) {checkbit(mc,mc.readByte(mc.Registers.hl),4); } };
	instructions[0x67] = { "BIT 4 A    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.a,4); } };
	
	instructions[0x68] = { "BIT 5 B    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.b,5); } };
	instructions[0x69] = { "BIT 5 C    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.c,5); } };
	instructions[0x6A] = { "BIT 5 D    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.d,5); } };
	instructions[0x6B] = { "BIT 5 E    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.e,5); } };
	instructions[0x6C] = { "BIT 5 H    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.h,5); } };
	instructions[0x6D] = { "BIT 5 L    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.l,5); } };
	instructions[0x6E] = { "BIT 5 (HL) ", [](Gameboy&mc) {checkbit(mc,mc.readByte(mc.Registers.hl),5); } };
	instructions[0x6F] = { "BIT 5 A    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.a,5); } };

	instructions[0x70] = { "BIT 6 B    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.b,6); } };
	instructions[0x71] = { "BIT 6 C    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.c,6); } };
	instructions[0x72] = { "BIT 6 D    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.d,6); } };
	instructions[0x73] = { "BIT 6 E    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.e,6); } };
	instructions[0x74] = { "BIT 6 H    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.h,6); } };
	instructions[0x75] = { "BIT 6 L    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.l,6); } };
	instructions[0x76] = { "BIT 6 (HL) ", [](Gameboy&mc) {checkbit(mc,mc.readByte(mc.Registers.hl),6); } };
	instructions[0x77] = { "BIT 6 A    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.a,6); } };
	
	instructions[0x78] = { "BIT 7 B    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.b,7); } };
	instructions[0x79] = { "BIT 7 C    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.c,7); } };
	instructions[0x7A] = { "BIT 7 D    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.d,7); } };
	instructions[0x7B] = { "BIT 7 E    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.e,7); } };
	instructions[0x7C] = { "BIT 7 H    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.h,7); } };
	instructions[0x7D] = { "BIT 7 L    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.l,7); } };
	instructions[0x7E] = { "BIT 7 (HL) ", [](Gameboy&mc) {checkbit(mc,mc.readByte(mc.Registers.hl),7); } };
	instructions[0x7F] = { "BIT 7 A    ", [](Gameboy&mc) {checkbit(mc,mc.Registers.a,7); } };

	instructions[0x80] = { "RES 0 B    ", [](Gameboy&mc) {resbit(mc.Registers.b,0); } };
	instructions[0x81] = { "RES 0 C    ", [](Gameboy&mc) {resbit(mc.Registers.c,0); } };
	instructions[0x82] = { "RES 0 D    ", [](Gameboy&mc) {resbit(mc.Registers.d,0); } };
	instructions[0x83] = { "RES 0 E    ", [](Gameboy&mc) {resbit(mc.Registers.e,0); } };
	instructions[0x84] = { "RES 0 H    ", [](Gameboy&mc) {resbit(mc.Registers.h,0); } };
	instructions[0x85] = { "RES 0 L    ", [](Gameboy&mc) {resbit(mc.Registers.l,0); } };
	instructions[0x86] = { "RES 0 (HL) ", [](Gameboy&mc) {resbit_mem(mc,mc.Registers.hl,0); } };
	instructions[0x87] = { "RES 0 A    ", [](Gameboy&mc) {resbit(mc.Registers.a,0); } };
														 
	instructions[0x88] = { "RES 1 B    ", [](Gameboy&mc) {resbit(mc.Registers.b,1); } };
	instructions[0x89] = { "RES 1 C    ", [](Gameboy&mc) {resbit(mc.Registers.c,1); } };
	instructions[0x8A] = { "RES 1 D    ", [](Gameboy&mc) {resbit(mc.Registers.d,1); } };
	instructions[0x8B] = { "RES 1 E    ", [](Gameboy&mc) {resbit(mc.Registers.e,1); } };
	instructions[0x8C] = { "RES 1 H    ", [](Gameboy&mc) {resbit(mc.Registers.h,1); } };
	instructions[0x8D] = { "RES 1 L    ", [](Gameboy&mc) {resbit(mc.Registers.l,1); } };
	instructions[0x8E] = { "RES 1 (HL) ", [](Gameboy&mc) {resbit_mem(mc,mc.Registers.hl,1); } };
	instructions[0x8F] = { "RES 1 A    ", [](Gameboy&mc) {resbit(mc.Registers.a,1); } };
														 
	instructions[0x90] = { "RES 2 B    ", [](Gameboy&mc) {resbit(mc.Registers.b,2); } };
	instructions[0x91] = { "RES 2 C    ", [](Gameboy&mc) {resbit(mc.Registers.c,2); } };
	instructions[0x92] = { "RES 2 D    ", [](Gameboy&mc) {resbit(mc.Registers.d,2); } };
	instructions[0x93] = { "RES 2 E    ", [](Gameboy&mc) {resbit(mc.Registers.e,2); } };
	instructions[0x94] = { "RES 2 H    ", [](Gameboy&mc) {resbit(mc.Registers.h,2); } };
	instructions[0x95] = { "RES 2 L    ", [](Gameboy&mc) {resbit(mc.Registers.l,2); } };
	instructions[0x96] = { "RES 2 (HL) ", [](Gameboy&mc) {resbit_mem(mc,mc.Registers.hl,2); } };
	instructions[0x97] = { "RES 2 A    ", [](Gameboy&mc) {resbit(mc.Registers.a,2); } };
														 
	instructions[0x98] = { "RES 3 B    ", [](Gameboy&mc) {resbit(mc.Registers.b,3); } };
	instructions[0x99] = { "RES 3 C    ", [](Gameboy&mc) {resbit(mc.Registers.c,3); } };
	instructions[0x9A] = { "RES 3 D    ", [](Gameboy&mc) {resbit(mc.Registers.d,3); } };
	instructions[0x9B] = { "RES 3 E    ", [](Gameboy&mc) {resbit(mc.Registers.e,3); } };
	instructions[0x9C] = { "RES 3 H    ", [](Gameboy&mc) {resbit(mc.Registers.h,3); } };
	instructions[0x9D] = { "RES 3 L    ", [](Gameboy&mc) {resbit(mc.Registers.l,3); } };
	instructions[0x9E] = { "RES 3 (HL) ", [](Gameboy&mc) {resbit_mem(mc,mc.Registers.hl,3); } };
	instructions[0x9F] = { "RES 3 A    ", [](Gameboy&mc) {resbit(mc.Registers.a,3); } };
														 
	instructions[0xA0] = { "RES 4 B    ", [](Gameboy&mc) {resbit(mc.Registers.b,4); } };
	instructions[0xA1] = { "RES 4 C    ", [](Gameboy&mc) {resbit(mc.Registers.c,4); } };
	instructions[0xA2] = { "RES 4 D    ", [](Gameboy&mc) {resbit(mc.Registers.d,4); } };
	instructions[0xA3] = { "RES 4 E    ", [](Gameboy&mc) {resbit(mc.Registers.e,4); } };
	instructions[0xA4] = { "RES 4 H    ", [](Gameboy&mc) {resbit(mc.Registers.h,4); } };
	instructions[0xA5] = { "RES 4 L    ", [](Gameboy&mc) {resbit(mc.Registers.l,4); } };
	instructions[0xA6] = { "RES 4 (HL) ", [](Gameboy&mc) {resbit_mem(mc,mc.Registers.hl,4); } };
	instructions[0xA7] = { "RES 4 A    ", [](Gameboy&mc) {resbit(mc.Registers.a,4); } };
														  
	instructions[0xA8] = { "RES 5 B    ", [](Gameboy&mc) {resbit(mc.Registers.b,5); } };
	instructions[0xA9] = { "RES 5 C    ", [](Gameboy&mc) {resbit(mc.Registers.c,5); } };
	instructions[0xAA] = { "RES 5 D    ", [](Gameboy&mc) {resbit(mc.Registers.d,5); } };
	instructions[0xAB] = { "RES 5 E    ", [](Gameboy&mc) {resbit(mc.Registers.e,5); } };
	instructions[0xAC] = { "RES 5 H    ", [](Gameboy&mc) {resbit(mc.Registers.h,5); } };
	instructions[0xAD] = { "RES 5 L    ", [](Gameboy&mc) {resbit(mc.Registers.l,5); } };
	instructions[0xAE] = { "RES 5 (HL) ", [](Gameboy&mc) {resbit_mem(mc,mc.Registers.hl,5); } };
	instructions[0xAF] = { "RES 5 A    ", [](Gameboy&mc) {resbit(mc.Registers.a,5); } };
														 
	instructions[0xB0] = { "RES 6 B    ", [](Gameboy&mc) {resbit(mc.Registers.b,6); } };
	instructions[0xB1] = { "RES 6 C    ", [](Gameboy&mc) {resbit(mc.Registers.c,6); } };
	instructions[0xB2] = { "RES 6 D    ", [](Gameboy&mc) {resbit(mc.Registers.d,6); } };
	instructions[0xB3] = { "RES 6 E    ", [](Gameboy&mc) {resbit(mc.Registers.e,6); } };
	instructions[0xB4] = { "RES 6 H    ", [](Gameboy&mc) {resbit(mc.Registers.h,6); } };
	instructions[0xB5] = { "RES 6 L    ", [](Gameboy&mc) {resbit(mc.Registers.l,6); } };
	instructions[0xB6] = { "RES 6 (HL) ", [](Gameboy&mc) {resbit_mem(mc,mc.Registers.hl,6); } };
	instructions[0xB7] = { "RES 6 A    ", [](Gameboy&mc) {resbit(mc.Registers.a,6); } };
														  
	instructions[0xB8] = { "RES 7 B    ", [](Gameboy&mc) {resbit(mc.Registers.b,7); } };
	instructions[0xB9] = { "RES 7 C    ", [](Gameboy&mc) {resbit(mc.Registers.c,7); } };
	instructions[0xBA] = { "RES 7 D    ", [](Gameboy&mc) {resbit(mc.Registers.d,7); } };
	instructions[0xBB] = { "RES 7 E    ", [](Gameboy&mc) {resbit(mc.Registers.e,7); } };
	instructions[0xBC] = { "RES 7 H    ", [](Gameboy&mc) {resbit(mc.Registers.h,7); } };
	instructions[0xBD] = { "RES 7 L    ", [](Gameboy&mc) {resbit(mc.Registers.l,7); } };
	instructions[0xBE] = { "RES 7 (HL) ", [](Gameboy&mc) {resbit_mem(mc,mc.Registers.hl,6); } };
	instructions[0xBF] = { "RES 7 A    ", [](Gameboy&mc) {resbit(mc.Registers.a,7); } };

	instructions[0xC0] = { "SET 0 B    ", [](Gameboy&mc) {setbit(mc.Registers.b,0); } };
	instructions[0xC1] = { "SET 0 C    ", [](Gameboy&mc) {setbit(mc.Registers.c,0); } };
	instructions[0xC2] = { "SET 0 D    ", [](Gameboy&mc) {setbit(mc.Registers.d,0); } };
	instructions[0xC3] = { "SET 0 E    ", [](Gameboy&mc) {setbit(mc.Registers.e,0); } };
	instructions[0xC4] = { "SET 0 H    ", [](Gameboy&mc) {setbit(mc.Registers.h,0); } };
	instructions[0xC5] = { "SET 0 L    ", [](Gameboy&mc) {setbit(mc.Registers.l,0); } };
	instructions[0xC6] = { "SET 0 (HL) ", [](Gameboy&mc) {setbit_mem(mc,mc.Registers.hl,0); } };
	instructions[0xC7] = { "SET 0 A    ", [](Gameboy&mc) {setbit(mc.Registers.a,0); } };
	instructions[0xC8] = { "SET 1 B    ", [](Gameboy&mc) {setbit(mc.Registers.b,1); } };
	instructions[0xC9] = { "SET 1 C    ", [](Gameboy&mc) {setbit(mc.Registers.c,1); } };
	instructions[0xCA] = { "SET 1 D    ", [](Gameboy&mc) {setbit(mc.Registers.d,1); } };
	instructions[0xCB] = { "SET 1 E    ", [](Gameboy&mc) {setbit(mc.Registers.e,1); } };
	instructions[0xCC] = { "SET 1 H    ", [](Gameboy&mc) {setbit(mc.Registers.h,1); } };
	instructions[0xCD] = { "SET 1 L    ", [](Gameboy&mc) {setbit(mc.Registers.l,1); } };
	instructions[0xCE] = { "SET 1 (HL) ", [](Gameboy&mc) {setbit_mem(mc,mc.Registers.hl,1); } };
	instructions[0xCF] = { "SET 1 A    ", [](Gameboy&mc) {setbit(mc.Registers.a,1); } };

	instructions[0xD0] = { "SET 2 B    ", [](Gameboy&mc) {setbit(mc.Registers.b,2); } };
	instructions[0xD1] = { "SET 2 C    ", [](Gameboy&mc) {setbit(mc.Registers.c,2); } };
	instructions[0xD2] = { "SET 2 D    ", [](Gameboy&mc) {setbit(mc.Registers.d,2); } };
	instructions[0xD3] = { "SET 2 E    ", [](Gameboy&mc) {setbit(mc.Registers.e,2); } };
	instructions[0xD4] = { "SET 2 H    ", [](Gameboy&mc) {setbit(mc.Registers.h,2); } };
	instructions[0xD5] = { "SET 2 L    ", [](Gameboy&mc) {setbit(mc.Registers.l,2); } };
	instructions[0xD6] = { "SET 2 (HL) ", [](Gameboy&mc) {setbit_mem(mc,mc.Registers.hl,2); } };
	instructions[0xD7] = { "SET 2 A    ", [](Gameboy&mc) {setbit(mc.Registers.a,2); } };
	instructions[0xD8] = { "SET 3 B    ", [](Gameboy&mc) {setbit(mc.Registers.b,3); } };
	instructions[0xD9] = { "SET 3 C    ", [](Gameboy&mc) {setbit(mc.Registers.c,3); } };
	instructions[0xDA] = { "SET 3 D    ", [](Gameboy&mc) {setbit(mc.Registers.d,3); } };
	instructions[0xDB] = { "SET 3 E    ", [](Gameboy&mc) {setbit(mc.Registers.e,3); } };
	instructions[0xDC] = { "SET 3 H    ", [](Gameboy&mc) {setbit(mc.Registers.h,3); } };
	instructions[0xDD] = { "SET 3 L    ", [](Gameboy&mc) {setbit(mc.Registers.l,3); } };
	instructions[0xDE] = { "SET 3 (HL) ", [](Gameboy&mc) {setbit_mem(mc,mc.Registers.hl,3); } };
	instructions[0xDF] = { "SET 3 A    ", [](Gameboy&mc) {setbit(mc.Registers.a,3); } };

	instructions[0xE0] = { "SET 4 B    ", [](Gameboy&mc) {setbit(mc.Registers.b,4); } };
	instructions[0xE1] = { "SET 4 C    ", [](Gameboy&mc) {setbit(mc.Registers.c,4); } };
	instructions[0xE2] = { "SET 4 D    ", [](Gameboy&mc) {setbit(mc.Registers.d,4); } };
	instructions[0xE3] = { "SET 4 E    ", [](Gameboy&mc) {setbit(mc.Registers.e,4); } };
	instructions[0xE4] = { "SET 4 H    ", [](Gameboy&mc) {setbit(mc.Registers.h,4); } };
	instructions[0xE5] = { "SET 4 L    ", [](Gameboy&mc) {setbit(mc.Registers.l,4); } };
	instructions[0xE6] = { "SET 4 (HL) ", [](Gameboy&mc) {setbit_mem(mc,mc.Registers.hl,4); } };
	instructions[0xE7] = { "SET 4 A    ", [](Gameboy&mc) {setbit(mc.Registers.a,4); } };
	instructions[0xE8] = { "SET 5 B    ", [](Gameboy&mc) {setbit(mc.Registers.b,5); } };
	instructions[0xE9] = { "SET 5 C    ", [](Gameboy&mc) {setbit(mc.Registers.c,5); } };
	instructions[0xEA] = { "SET 5 D    ", [](Gameboy&mc) {setbit(mc.Registers.d,5); } };
	instructions[0xEB] = { "SET 5 E    ", [](Gameboy&mc) {setbit(mc.Registers.e,5); } };
	instructions[0xEC] = { "SET 5 H    ", [](Gameboy&mc) {setbit(mc.Registers.h,5); } };
	instructions[0xED] = { "SET 5 L    ", [](Gameboy&mc) {setbit(mc.Registers.l,5); } };
	instructions[0xEE] = { "SET 5 (HL) ", [](Gameboy&mc) {setbit_mem(mc,mc.Registers.hl,5); } };
	instructions[0xEF] = { "SET 5 A    ", [](Gameboy&mc) {setbit(mc.Registers.a,5); } };

	instructions[0xF0] = { "SET 6 B    ", [](Gameboy&mc) {setbit(mc.Registers.b,6); } };
	instructions[0xF1] = { "SET 6 C    ", [](Gameboy&mc) {setbit(mc.Registers.c,6); } };
	instructions[0xF2] = { "SET 6 D    ", [](Gameboy&mc) {setbit(mc.Registers.d,6); } };
	instructions[0xF3] = { "SET 6 E    ", [](Gameboy&mc) {setbit(mc.Registers.e,6); } };
	instructions[0xF4] = { "SET 6 H    ", [](Gameboy&mc) {setbit(mc.Registers.h,6); } };
	instructions[0xF5] = { "SET 6 L    ", [](Gameboy&mc) {setbit(mc.Registers.l,6); } };
	instructions[0xF6] = { "SET 6 (HL) ", [](Gameboy&mc) {setbit_mem(mc,mc.Registers.hl,6); } };
	instructions[0xF7] = { "SET 6 A    ", [](Gameboy&mc) {setbit(mc.Registers.a,6); } };
	instructions[0xF8] = { "SET 7 B    ", [](Gameboy&mc) {setbit(mc.Registers.b,7); } };
	instructions[0xF9] = { "SET 7 C    ", [](Gameboy&mc) {setbit(mc.Registers.c,7); } };
	instructions[0xFA] = { "SET 7 D    ", [](Gameboy&mc) {setbit(mc.Registers.d,7); } };
	instructions[0xFB] = { "SET 7 E    ", [](Gameboy&mc) {setbit(mc.Registers.e,7); } };
	instructions[0xFC] = { "SET 7 H    ", [](Gameboy&mc) {setbit(mc.Registers.h,7); } };
	instructions[0xFD] = { "SET 7 L    ", [](Gameboy&mc) {setbit(mc.Registers.l,7); } };
	instructions[0xFE] = { "SET 7 (HL) ", [](Gameboy&mc) {setbit_mem(mc,mc.Registers.hl,6); } };
	instructions[0xFF] = { "SET 7 A    ", [](Gameboy&mc) {setbit(mc.Registers.a,7); } };

}

void CBPrefix::execute(Gameboy&mc,uint8_t op)
{
	instructions[op].fn(mc);
}

CBPrefix::CBPrefix()
{
}


CBPrefix::~CBPrefix()
{
}

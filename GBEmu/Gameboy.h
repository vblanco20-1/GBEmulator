#pragma once
#include <string>
#include "GB_Cpu.h"
#include "Screen.h"
typedef unsigned char byte;


struct GBRegisterBank {
	struct {
		union {
			struct {
				unsigned char f;
				unsigned char a;
			};
			unsigned short af;
		};
	};

	struct {
		union {
			struct {
				unsigned char c;
				unsigned char b;
			};
			unsigned short bc;
		};
	};

	struct {
		union {
			struct {
				unsigned char e;
				unsigned char d;
			};
			unsigned short de;
		};
	};

	struct {
		union {
			struct {
				unsigned char l;
				unsigned char h;
			};
			unsigned short hl;
		};
	};

	unsigned short sp;
	unsigned short pc;
};

const unsigned char	Zero_Flag = 1 << 7;
const unsigned char	Subtract_Flag = 1 << 6;
const unsigned char	HalfCarry_Flag = 1 << 5;
const unsigned char	Carry_Flag     = 1 << 4;
class GB_Cpu;
class Gameboy
{
public:
	Gameboy();
	~Gameboy();

	GB_Cpu *CPU;
	Screen GBScreen;
	
	void writeByte(byte b, unsigned short idx);
	uint8_t readByte(unsigned short idx);
	uint8_t& getByte(unsigned short idx);
	unsigned short readShort(unsigned short idx);
	void writeShort(unsigned short addr, unsigned short value);
	bool LoadROM(std::string Address);
	void reset();
	void Run();
	void EnableInterrupts() { 
		bInterruptsEnabled = true; 
		writeByte(1,0xffff); 
	};
	void DisableInterrupts() {
		bInterruptsEnabled = false;
		writeByte(0, 0xffff);
	};
	void Stop();;
	void Halt();;

	

	//32k of memory
	byte memory[65536];
	
	GBRegisterBank Registers;
	bool bInterruptsEnabled;
	void SetFlags(unsigned char flags);
	void ClearFlags(unsigned char flags);
	bool GetFlag(unsigned char flag);

	void UpdateInterrupts(int time);
	void UpdateSTATInterrupt(int time);

	int vsyncCycles;
	int LDYCycles;

	uint8_t LY;
};
static bool getbit(uint8_t byte, int nbit) {
	return (byte >> nbit) & 1;
}

#pragma once
#include <string>

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

class Gameboy
{
public:
	Gameboy();
	~Gameboy();

	void writeByte(byte b, unsigned short idx);
	byte readByte(unsigned short idx);
	bool LoadROM(std::string Address);

	
	//32k of memory
	byte memory[65536];
	
	GBRegisterBank Registers;
};


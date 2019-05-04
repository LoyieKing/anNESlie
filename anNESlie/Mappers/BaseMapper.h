#pragma once
#include <ostream>
#include <istream>

#include "../Types.h"
//#include "../Emulator/Emulator.h"
//#include "../CPU/Core.h"
//#include "../PPU/Core.h"


class CPU;
class PPU;

namespace ROM
{
	class Cartridge;
}

namespace Mapper
{
	class BaseMapper
	{
	protected:

		int prgROMLength;
		Byte* prgROM;

		int prgRAMLength = 0x2000;
		Byte prgRAM[0x2000];

		int chrROMLength;
		Byte* chrROM;

		Word lastBankOffset;


		BaseMapper(ROM::Cartridge* _emulator);

	public:
		int Id;
		const char* Name;
		const char* Description;

		virtual void InitializeMemoryMap(CPU* cpu) = 0;
		virtual void InitializeMemoryMap(PPU* ppu);
		virtual void ProcessCycle(int scanline, int cycle) = 0;
		virtual void Save(std::ostream stream);
		virtual void Load(std::istream stream);
	};

}
#pragma once
#include <ostream>
#include <istream>

#include "../Emulator/Emulator.h"
#include "../CPU/Core.h"
#include "../PPU/Core.h"

namespace Mapper
{
	class BaseMapper
	{
	protected:
		Emulator* emulator;


		int prgROMLength;
		Byte* prgROM;

		int prgRAMLength = 0x2000;
		Byte prgRAM[0x2000];

		int chrROMLength;
		Byte* chrROM;

		Word lastBankOffset;


		BaseMapper(Emulator* _emulator);

	public:
		int Id;
		const char* Name;
		const char* Description;

		virtual void InitializeMemoryMap(CPU::CPUCore* cpu) = 0;
		virtual void InitializeMemoryMap(PPUCore* ppu);
		virtual void ProcessCycle(int scanline, int cycle) = 0;
		virtual void Save(std::ostream stream);
		virtual void Load(std::istream stream);
	};

}
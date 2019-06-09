#include "BaseMapper.h"

#include "../ROM/Cartridge.h"
#include "../CPU/CPU.h"
#include "../PPU/PPU.h"
#include <fstream>

Mapper::BaseMapper::BaseMapper(Emulator* _emulator)
{
	prgROMLength = _emulator->cartridge->PRGROMSize;
	prgROM = _emulator->cartridge->PRGROM;

	prgRAMLength = _emulator->cartridge->PRGROMSize;

	chrROM = _emulator->cartridge->CHRROM;
	chrROMLength = _emulator->cartridge->CHRROMSize;

	lastBankOffset = prgROMLength - 0x4000;
}

void Mapper::BaseMapper::InitializeMemoryMap(CPU* cpu)
{
}

void Mapper::BaseMapper::InitializeMemoryMap(PPU* ppu)
{
	ppu->memoryHandler.SetReadHandler(0x0000, 0x1FFF,
		[this](int address) {
			return chrROM[address];
		});
	ppu->memoryHandler.SetWriteHandler(0x0000, 0x1FFF,
		[this](int address, Byte value) {
			chrROM[address] = value;
		});
}

void Mapper::BaseMapper::ProcessCycle(int scanline, int cycle)
{
}

void Mapper::BaseMapper::Save(std::ostream& stream)
{
	stream.write((char*)prgRAM, prgRAMLength);
}

void Mapper::BaseMapper::Load(std::istream& stream)
{
	stream.read((char*)prgRAM, prgRAMLength);
}

Mapper::BaseMapper::~BaseMapper()
{
}

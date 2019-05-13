#include "BaseMapper.h"

#include "../ROM/Cartridge.h"
#include "../CPU/CPU.h"
#include "../PPU/PPU.h"

Mapper::BaseMapper::BaseMapper(Emulator* _emulator)
{
	prgROM = _emulator->cartridge->getPRGROM();
	prgRAMLength = _emulator->cartridge->getPRGROMSize();
	chrROM = _emulator->cartridge->getCHRROM();
	chrROMLength = _emulator->cartridge->getCHRROMSize();
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

void Mapper::BaseMapper::Save(std::ostream stream)
{
	stream.write((char*)prgRAM, prgRAMLength);
}

void Mapper::BaseMapper::Load(std::istream stream)
{
	stream.read((char*)prgRAM, prgRAMLength);
}

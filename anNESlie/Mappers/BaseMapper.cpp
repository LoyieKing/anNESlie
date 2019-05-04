#include "BaseMapper.h"

#include "../ROM/Cartridge.h"
#include "../CPU/CPU.h"
#include "../PPU/PPU.h"

Mapper::BaseMapper::BaseMapper(ROM::Cartridge* _cartridge)
{
	prgROM = _cartridge->getPRGROM();
	prgRAMLength = _cartridge->getPRGROMSize();
	chrROM = _cartridge->getCHRROM();
	chrROMLength = _cartridge->getCHRROMSize();
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

void Mapper::BaseMapper::Save(std::ostream stream)
{
	stream.write((char*)prgRAM, prgRAMLength);
}

void Mapper::BaseMapper::Load(std::istream stream)
{
	stream.read((char*)prgRAM, prgRAMLength);
}

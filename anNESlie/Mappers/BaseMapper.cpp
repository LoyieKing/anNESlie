#include "BaseMapper.h"

Mapper::BaseMapper::BaseMapper(Emulator* _emulator)
{
	emulator = _emulator;
	prgROM = emulator->Cartbridge.getPRGROM();
	chrROM = emulator->Cartbridge.getCHRROM();
}

void Mapper::BaseMapper::InitializeMemoryMap(PPUCore* ppu)
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

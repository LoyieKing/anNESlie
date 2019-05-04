#include "NROM.h"

#include "../CPU/CPU.h"

Mapper::NROM::NROM(Emulator* emulator) :
	BaseMapper(emulator)
{
	for (int i = 0; i < 0x8000; i++)
	{
		int offset = prgROMLength == 0x4000 ? i & 0xBFFF : i;
		addressSpace[0x2000 + i] = prgROM[offset];
	}
}

void Mapper::NROM::InitializeMemoryMap(CPU* cpu)
{
	cpu->memoryHandler.SetReadHandler(0x6000, 0xFFFF, [this](Word address) {
		return addressSpace[address - 0x6000];
		});
	cpu->memoryHandler.SetWriteHandler(0x6000, 0xFFFF, [this](Word address, Byte value) {
		addressSpace[address - 0x6000] = value;
		});
}

#include "Core.h"

void CPU::CPUCore::WriteIORegister(Word reg, Byte val)
{
	switch (reg)
	{
	case 0x4014: // OAM DMA
		emulator->PPU.PerformDMA(val);
		break;
	case 0x4016:
		emulator->Controller->Strobe(val == 1);
		break;
	}
	if (reg <= 0x401F) return; // APU write
	throw "NotImplementedException";
}

Byte CPU::CPUCore::ReadIORegister(Word reg)
{
	switch (reg)
	{
	case 0x4016:
		return (Word)emulator->Controller->ReadState() & 0x1;
	}
	return 0x00;
	//throw new NotImplementedException();
}
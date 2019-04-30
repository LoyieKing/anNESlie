#include "Core.h"

Byte PPU::PPUCore::ReadByte(Word adress)
{
	return memoryHandler.ReadByte(adress);
}

void PPU::PPUCore::WriteByte(Word adress, Byte value)
{
	memoryHandler.WriteByte(adress, value);
}

Byte PPU::PPUCore::ReadWord(Word adress)
{
	return memoryHandler.ReadWord(adress);
}

void PPU::PPUCore::WriteWord(Word adress, Byte value)
{
	memoryHandler.WriteWord(adress, value);
}

void PPU::PPUCore::WriteRegister(Byte reg, Byte val)
{
	reg &= 0xF;
	lastWrittenRegister = val & 0xFF;
	switch (reg)
	{
	case 0x0000:
		setPPUCTRL(val);
		return;
	case 0x0001:
		setPPUMASK(val);
		return;
	case 0x0002: return;
	case 0x0003:
		setOAMADDR(val);
		return;
	case 0x0004:
		setOAMDATA(val);
		return;
	case 0x005:
		setPPUSCROLL(val);
		return;
	case 0x0006:
		setPPUADDR(val);
		return;
	case 0x0007:
		setPPUDATA(val);
		return;
	}
	throw "NotImplementedException!";
}

Byte PPU::PPUCore::ReadRegister(Byte reg)
{
	reg &= 0xF;
	switch (reg)
	{
	case 0x0000: return lastWrittenRegister;
	case 0x0001: return lastWrittenRegister;
	case 0x0002:
		return getPPUSTATUS();
	case 0x0003:
		return getOAMADDR();
	case 0x0004:
		return getOAMDATA();
	case 0x0005: return lastWrittenRegister;
	case 0x0006: return lastWrittenRegister;
	case 0x0007:
		return getPPUDATA();
	}
	throw "NotImplementedException!";
}

Word PPU::PPUCore::GetVRAMMirror(Word addr)
{
	// HACK: C# version is QWORD here,maybe a bug?
	Word entry = (addr - 0x2000) % 0x400;
	Word table = (addr - 0x2000) / 0x400;
	return VRAMMirrorLookUp[emulator.Cartridge.MirroringMode][table] * 0x400 + entry;
}

void PPU::PPUCore::PerformDMA(Word from)
{
	from <<= 8;
	for(int i=0;i<0xFF;i++)
	{
		Byte oamAdress = Flag.getOAMAddress();
		oam[oamAdress] = emulator.CPU.ReadByte(from);
		from++;
		Flag.setOAMAddress(oamAdress + 1);
	}

	emulator.CPU.Cycle += 513 + emulator.CPU.Cycle % 2;
}

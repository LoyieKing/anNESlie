#include "PPU.h"


Byte PPU::ReadByte(Word address)
{
	return memoryHandler.ReadByte(address);
}

void PPU::WriteByte(Word address, Byte value)
{
	memoryHandler.WriteByte(address, value);
}

Word PPU::ReadWord(Word address)
{
	return memoryHandler.ReadWord(address);
}

//void PPU::WriteWord(Word address, Byte value)
//{
//	memoryHandler.WriteWord(address, value);
//}

void PPU::WriteRegister(Byte reg, Byte val)
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

Byte PPU::ReadRegister(Byte reg)
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


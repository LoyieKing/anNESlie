#include"PPU.h"

Word PPU::getV()
{
	return v;
}

void PPU::setV(Word val)
{
	v = val & 0x7FFF;
}

Byte PPU::getCoarseX()
{
	return v & 0x1F;
}

Byte PPU::getCoarseY()
{
	return (v >> 5) & 0x1F;
}

Byte PPU::getFineY()
{
	return (v >> 12) & 0x7;
}

void PPU::ReloadScrollX()
{
	v = (v & 0xFBE0) | (T & 0x041F);
}

void PPU::ReloadScrollY()
{
	v = (v & 0x841F) | (T & 0x7BE0);
}

void PPU::IncrementScrollX()
{
	if ((v & 0x001F) == 31) // if coarse X == 31
	{
		v &= ~0x001Fu; // coarse X = 0
		v ^= 0x0400; // switch horizontal nametable
	}
	else
		v += 1; // increment coarse X
}

void PPU::IncrementScrollY()
{

	if ((v & 0x7000) != 0x7000) // if fine Y < 7
		v += 0x1000; // increment fine Y
	else
	{
		v &= ~0x7000u; // fine Y = 0

		Word y = (v & 0x03E0) >> 5; // let y = coarse Y
		if (y == 29)
		{
			y = 0; // coarse Y = 0
			v ^= 0x0800;
		}
		// switch vertical nametable
		else if (y == 31)
			y = 0; // coarse Y = 0, nametable not switched
		else
			y += 1; // increment coarse Y
		v = (v & ~0x03E0u) | (y << 5); // put coarse Y back into v
	}
}

void PPU::setPPUCTRL(Byte val)
{
	Flag.NMIEnabled = (val & 0x80) > 0;
	Flag.IsMaster = (val & 0x40) > 0;
	Flag.TallSpritesEnabled = (val & 0x20) > 0;
	Flag.PatternTableAddress = (val & 0x10) > 0 ? 0x1000u : 0x0000;
	Flag.SpriteTableAddress = (val & 0x08) > 0 ? 0x1000u : 0x0000;
	Flag.VRAMIncrement = (val & 0x04) > 0 ? 32u : 1;
	// yyy NN YYYYY XXXXX
	// ||| || ||||| +++++--coarse X scroll
	// ||| || +++++--------coarse Y scroll
	// ||| ++--------------nametable select
	// +++-----------------fine Y scroll
	T = (T & 0xF3FF) | ((val & 0x3) << 10); // Bits 10-11 hold the base address of the nametable minus $2000
}

void PPU::setPPUMASK(Byte val)
{
	Flag.GrayscaleEnabled = (val & 0x1) > 0;
	Flag.DrawLeftBackground = (val & 0x2) > 0;
	Flag.DrawLeftSprites = (val & 0x4) > 0;
	Flag.DrawBackground = (val & 0x8) > 0;
	Flag.DrawSprites = (val & 0x10) > 0;
	Flag.EmphasizeRed = (val & 0x20) > 0;
	Flag.EmphasizeGreen = (val & 0x40) > 0;
	Flag.EmphasizeBlue = (val & 0x80) > 0;
}

Byte PPU::getPPUSTATUS()
{
	Flag.AddressLatch = false;
	Byte ret =
		((Flag.VBlankStarted ? 1 : 0) << 7) |
		((Flag.Sprite0Hit ? 1 : 0) << 6) |
		((Flag.SpriteOverflow ? 1 : 0) << 5) |
		(lastWrittenRegister & 0x1F);
	Flag.VBlankStarted = false;
	return ret;
}

void PPU::setPPUADDR(Word val)
{
	if (Flag.AddressLatch)
	{
		T = (T & 0xFF00) | val;
		Flag.setBusAddress(T);
		v = T;
	}
	else
		T = (T & 0x80FF) | ((val & 0x3F) << 8);
	Flag.AddressLatch ^= true;
}

void PPU::setPPUSCROLL(Word value)
{
	if (Flag.AddressLatch)
	{
		Flag.ScrollY = value;
		T = (T & 0x8FFF) | ((value & 0x7) << 12);
		T = (T & 0xFC1F) | (value & 0xF8) << 2;
	}
	else
	{
		Flag.ScrollX = value;
		X = value & 0x7;
		T = (T & 0xFFE0) | (value >> 3);
	}
	Flag.AddressLatch ^= true;
}

int readBufferi = 0;
Byte PPU::getPPUDATA()
{
	Word busAdress = Flag.getBusAddress();
	Byte ret = ReadByte(busAdress);
	if (busAdress < 0x3F00)
	{
		Byte temp = readBuffer;
		readBuffer = ret;
		ret = temp;
		readBufferi++;
	}
	else
	{
		// Palette read should also read VRAM into read buffer
		readBuffer = ReadByte(busAdress - 0x1000);
		readBufferi++;
	}
	Flag.setBusAddress(busAdress + Flag.VRAMIncrement);
	return ret;
}

void PPU::setPPUDATA(Byte value)
{
	Word busAdress = Flag.getBusAddress();
	Flag.BusData = value;
	WriteByte(busAdress, value);
	Flag.setBusAddress(busAdress + Flag.VRAMIncrement);
}

Byte PPU::getOAMADDR()
{
	return Flag.getOAMAddress();
}

void PPU::setOAMADDR(Byte value)
{
	Flag.setOAMAddress(value);
}

Byte PPU::getOAMDATA()
{
	return ((Byte*)oam)[Flag.getOAMAddress()];
}

void PPU::setOAMDATA(Byte value)
{
	Byte oamAdress = Flag.getOAMAddress();
	((Byte*)oam)[oamAdress] = value;
	Flag.setOAMAddress(oamAdress + 1);
}

#include "Emulator.h"

Emulator(const char* rom_path) :
	CPU(this),
	PPU(this),
	Cartbridge(rom_path)
{
	romPath = rom_path;
}
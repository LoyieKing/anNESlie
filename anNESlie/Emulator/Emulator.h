#pragma once

#include "../Types.h"
//#include"../CPU/Core.h"
//#include"../PPU/Core.h"
//#include"../ROM/Cartridge.h"
//#include"../Mappers/BaseMapper.h"
//#include"../Controllers/Controller.h"

class CPU;
class PPU;
namespace ROM { class Cartridge; }
namespace Mapper { class BaseMapper; }
namespace Controller { class Controller; }

class Emulator
{
private:
	const char* romPath;

public:
	CPU* cpu;
	PPU* ppu;
	ROM::Cartridge* Cartbridge;
	Mapper::BaseMapper* Mapper;
	Controller::Controller* Controller;

	Emulator(const char* rom_path);

	void MapperProcessCycle(int scanline, int cycle);

	Word GetVRAMMirror(Word addr);
	void PerformDMA(Word from);

	void Reset();
	void TickFromPPU();
	void TriggerInterrupt(InterruptType type);

};

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

	CPU* cpu;
	PPU* ppu;
	ROM::Cartridge* Cartridge;
	Mapper::BaseMapper* Mapper;
	Controller::Controller* Controller;


	void MapperProcessCycle(int scanline, int cycle);

	Word GetVRAMMirror(Word addr);
	void PerformDMA(Word from);

	void Reset();
	void TickFromPPU();
	void TriggerInterrupt(InterruptType type);

public:
	friend class CPU;
	friend class PPU;
	friend class Mapper::BaseMapper;

	Emulator(const char* rom_path);
	~Emulator();

	DWord const* RawBitmap;
	void ProcessFrame();

	void DumpMemoryCPU();
	void DumpMemoryPPU();
};

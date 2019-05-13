#pragma once

#include "../Types.h"

class CPU;
class PPU;
namespace ROM { class Cartridge; }
namespace Mapper { class BaseMapper; }
namespace Controller { class Controller; }
class Setting;

class Emulator
{
private:
	void MapperProcessCycle(int scanline, int cycle);

	Word GetVRAMMirror(Word addr);
	void PerformDMA(Word from);

	void TickFromPPU();
	void TriggerInterrupt(InterruptType type);

private:
	const char* romPath;

	CPU* cpu;
	PPU* ppu;
	ROM::Cartridge* cartridge;
	Mapper::BaseMapper* mapper;
	Controller::Controller* controller;

	Setting* setting;

public:
	friend class CPU;
	friend class PPU;
	friend class Mapper::BaseMapper;

	Emulator(const char* rom_path);
	~Emulator();

	Color ScreenOutput[GAME_HEIGHT * GAME_WIDTH];

	void Reset();
	void ProcessFrame();

	void DumpMemoryCPU();
	void DumpMemoryPPU();

	bool isNull();

	void KeyDown(int key);
	void KeyUp(int key);
};

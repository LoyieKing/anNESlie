#pragma once
#include"../CPU/Core.h"
#include"../PPU/Core.h"
#include"../ROM/Cartridge.h"
#include"../Mappers/BaseMapper.h"
#include"../Controllers/Controller.h"

class Emulator
{
private:
	const char* romPath;

public:
	CPU::CPUCore CPU;
	PPUCore PPU;
	ROM::Cartridge Cartbridge;
	Mapper::BaseMapper* Mapper;
	Controller::Controller* Controller;

	Emulator(const char* rom_path);

};

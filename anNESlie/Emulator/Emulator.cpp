#include "Emulator.h"
#include "../CPU/CPU.h"
#include"../PPU/PPU.h"
#include"../ROM/Cartridge.h"
#include "../Mappers/Mapper.h"
#include"../Mappers/BaseMapper.h"
#include"../Controllers/Controller.h"
#include "../Controllers/NES001Controller.h"
#include "Setting.h"

Emulator::Emulator(const char* rom_path)
{
	romPath = rom_path;
	this->cartridge = new ROM::Cartridge(rom_path);
	this->mapper = Mapper::LoadMapper(this, this->cartridge->getMapperNumber());
	this->cpu = new ::CPU(this);
	this->ppu = new ::PPU(this);

	this->setting = Setting::ReadSetting();
	this->controller = Controller::GetController(setting->controller);

	//RawBitmap = this->ppu->rawBitmap;


	romPath = rom_path;
}

Emulator::~Emulator()
{
	if (cartridge != nullptr)
	{
		delete this->cartridge;
		delete this->cpu;
		delete this->ppu;
		delete this->controller;
		delete this->mapper;
		delete this->setting;
		cartridge = nullptr;
	}
}

void Emulator::MapperProcessCycle(int scanline, int cycle)
{
	this->mapper->ProcessCycle(scanline, cycle);
}

Word Emulator::GetVRAMMirror(Word addr)
{
	// HACK: C# version is QWORD here,maybe a bug?
	Word entry = (addr - 0x2000) % 0x400;
	Word table = (addr - 0x2000) / 0x400;
	return this->ppu->VRAMMirrorLookUp[this->cartridge->getMirroringMode()][table] * 0x400 + entry;
}

void Emulator::PerformDMA(Word from)
{
	from <<= 8;
	for (int i = 0; i <= 0xFF; i++)
	{
		Byte oamAdress = this->ppu->Flag.getOAMAddress();
		((Byte*)this->ppu->oam)[oamAdress] = this->cpu->ReadByte(from);
		from++;
		this->ppu->Flag.setOAMAddress(oamAdress + 1);
	}
	this->cpu->cycle += 513 + this->cpu->cycle % 2;
}

void Emulator::Reset()
{
	this->cpu->Register.SP -= 3;
	this->cpu->Register.P.InterruptDisabled = true;
}

void Emulator::TickFromPPU()
{
	if (this->cpu->cycle-- > 0)
		return;
	this->cpu->ExecuteSingleInstruction();
}

void Emulator::TriggerInterrupt(InterruptType type)
{
	if (!this->cpu->Register.P.InterruptDisabled || type == NMI)
		this->cpu->interrupts[type] = true;
}

void Emulator::ProcessFrame()
{
	this->ppu->ProcessFrame();
}

void Emulator::DumpMemoryCPU()
{
	FILE* fpWrite = fopen("cpu.dump", "w");
	for (int i = 0; i < 0xFFFF; i++)
	{
		fprintf(fpWrite, "%d", (int)cpu->ReadByte(i));
	}
	fclose(fpWrite);
}

void Emulator::DumpMemoryPPU()
{
	FILE* fpWrite = fopen("ppu.dump", "w");
	for (int i = 0; i < 0xFFFF; i++)
	{
		fprintf(fpWrite, "%c", (char)ppu->ReadByte(i));
	}
	fclose(fpWrite);
}

bool Emulator::isNull()
{
	return cpu == nullptr;
}

void Emulator::KeyDown(int key)
{
	if (setting->keyBindings[key] == -1)
		return;
	controller->PressKey(setting->keyBindings[key]);
}

void Emulator::KeyUp(int key)
{
	if (setting->keyBindings[key] == -1)
		return;
	controller->ReleaseKey(setting->keyBindings[key]);
}


#include "Emulator.h"
#include "../CPU/CPU.h"
#include"../PPU/PPU.h"
#include"../ROM/Cartridge.h"
#include"../Mappers/BaseMapper.h"
#include"../Controllers/Controller.h"

Emulator::Emulator(const char* rom_path)
{
	this->cpu = new ::CPU(this);
	this->ppu = new ::PPU(this);
	this->Cartbridge = new ROM::Cartridge(rom_path);

	this->cpu->memoryHandler.SetReadHandler(0x2000, 0x3FFF,
		[this](int address) {
			return ppu->ReadRegister((address & 0x7) - 0x2000);
		});
	this->cpu->memoryHandler.SetWriteHandler(0x0000, 0x1FFF,
		[this](int address, Byte value) {
			ppu->WriteRegister((address & 0x7) - 0x2000, value);
		});


	this->Mapper->InitializeMemoryMap(this->cpu);
	this->Mapper->InitializeMemoryMap(this->ppu);


	// TODO: mapper and controller init
	this->Mapper = 0;
	this->Controller = 0;

	romPath = rom_path;
}

void Emulator::MapperProcessCycle(int scanline, int cycle)
{
	this->Mapper->ProcessCycle(scanline, cycle);
}

Word Emulator::GetVRAMMirror(Word addr)
{
	// HACK: C# version is QWORD here,maybe a bug?
	Word entry = (addr - 0x2000) % 0x400;
	Word table = (addr - 0x2000) / 0x400;
	return this->ppu->VRAMMirrorLookUp[this->Cartbridge->getMirroringMode()][table] * 0x400 + entry;
}

void Emulator::PerformDMA(Word from)
{
	from <<= 8;
	for (int i = 0; i < 0xFF; i++)
	{
		Byte oamAdress = this->ppu->Flag.getOAMAddress();
		this->ppu->oam[oamAdress] = this->cpu->ReadByte(from);
		from++;
		this->ppu->Flag.setOAMAddress(oamAdress + 1);
	}
	this->cpu->cycle += 513 + this->cpu->cycle % 2;
}

void Emulator::Reset()
{
	this->cpu->registerSP -= 3;
	this->cpu->setFlag_I(true);
}

void Emulator::TickFromPPU()
{
	if (this->cpu->cycle-- > 0)
		return;
	this->cpu->ExecuteSingleInstruction();
}

void Emulator::TriggerInterrupt(InterruptType type)
{
	if (!this->cpu->getFlag_I() || type == NMI)
		this->cpu->interrupts[type] = true;
}


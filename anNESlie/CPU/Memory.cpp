#include "Memory.h"

using namespace CPU;

Memory::Memory():memory(new char[CPU_MEMORY_SIZE]),memoryHandler(memory,CPU_MEMORY_SIZE)
{
	memoryHandler.SetReadHandler(0x0000, 0x1FFF, [this](int adress) {return memoryHandler.GetMemoryAdress()[adress & 0x07FF]; });
	memoryHandler.SetReadHandler(0x2000, 0x3FFF, [this](int adress) {return memoryHandler.GetMemoryAdress()[adress & 0x07FF]; });//TODO: _emulator.PPU.ReadRegister((addr & 0x7) - 0x2000));
	memoryHandler.SetReadHandler(0x4000, 0x4017, [this](int adress) {return memoryHandler.GetMemoryAdress()[adress & 0x07FF]; });//TODO: ReadIORegister

	memoryHandler.SetWriteHandler(0x0000, 0x1FFF, [this](int adress, char value) {memoryHandler.GetMemoryAdress()[adress & 0x07FF] = value; });
	memoryHandler.SetWriteHandler(0x2000, 0x3FFF, [this](int adress, char value) {memoryHandler.GetMemoryAdress()[adress & 0x07FF] = value; });//TODO: _emulator.PPU.ReadRegister((addr & 0x7) - 0x2000));
	memoryHandler.SetWriteHandler(0x4000, 0x4017, [this](int adress, char value) {memoryHandler.GetMemoryAdress()[adress & 0x07FF] = value; });//TODO: ReadIORegister
}

CPU::Memory::~Memory()
{
	if (memory == nullptr)
		return;
	delete[] memory;
	memory = nullptr;
}
